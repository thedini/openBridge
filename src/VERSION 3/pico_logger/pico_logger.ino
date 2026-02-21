/*
 * OpenBridge VERSION 3 - NMEA2000 Data Logger (Raspberry Pi Pico)
 *
 * Raspberry Pi Pico based standalone data logger for NMEA2000 bus traffic.
 * Logs all messages to SD card in candump hex format.
 *
 * Hardware Requirements:
 * - Raspberry Pi Pico or Pico W
 * - MCP2515 CAN controller module (SPI)
 * - SD card module (SPI)
 * - Optional: DS3231 RTC module (I2C)
 *
 * Connections (SPI0):
 * MCP2515 CAN Module:
 * - VCC  → 3.3V
 * - GND  → GND
 * - CS   → GP17 (SPI0 CSn)
 * - SO   → GP16 (SPI0 RX/MISO)
 * - SI   → GP19 (SPI0 TX/MOSI)
 * - SCK  → GP18 (SPI0 SCK)
 * - INT  → GP20
 *
 * SD Card Module (SPI1):
 * - VCC  → 3.3V
 * - GND  → GND
 * - CS   → GP13 (SPI1 CSn)
 * - MISO → GP12 (SPI1 RX)
 * - MOSI → GP11 (SPI1 TX)
 * - SCK  → GP10 (SPI1 SCK)
 *
 * Status LED:
 * - LED  → GP25 (built-in LED)
 *
 * Output Format (candump compatible):
 * (timestamp) can0 CANID [DLC] DATA_BYTES
 *
 * Example:
 * (1738696007.167910)  can0  09F11224   [8]  2E 99 99 00 00 64 F6 FD
 */

#include <SPI.h>
#include <SD.h>
#include <mcp2515.h>
#include <TimeLib.h>

// ===== PIN CONFIGURATION =====
// MCP2515 CAN Controller (SPI0)
#define CAN_CS_PIN 17
#define CAN_INT_PIN 20

// SD Card Module (SPI1)
#define SD_CS_PIN 13

// Status LED
#define LED_PIN LED_BUILTIN  // GP25 on Pico

// ===== SPI CONFIGURATION =====
SPIClassRP2040 spi0(spi0, 16, 18, 19, 17);  // SPI0 for CAN
SPIClassRP2040 spi1(spi1, 12, 10, 11, 13);  // SPI1 for SD

// ===== CONFIGURATION =====
#define BUFFER_SIZE 512         // SD write buffer size (matches sector)
#define FLUSH_INTERVAL 1000     // Flush buffer every 1 second (ms)
#define MAX_FILE_SIZE 104857600 // 100 MB in bytes
#define FILE_ROTATE_INTERVAL 3600  // New file every hour (seconds)

// ===== GLOBAL VARIABLES =====
MCP2515 mcp2515(CAN_CS_PIN, &spi0);
File logFile;
char logBuffer[BUFFER_SIZE];
int bufferPosition = 0;
unsigned long lastFlush = 0;
unsigned long lastRotate = 0;
unsigned long messageCount = 0;
bool sdCardOK = false;
bool canOK = false;
char currentFilename[32];

// ===== FUNCTION PROTOTYPES =====
void handleCANMessage();
void logMessage(struct can_frame &frame);
void flushBuffer();
void rotateLogFile();
void createNewLogFile();
String getTimestamp();
String formatCANID(uint32_t canId);
void blinkError(int count);
void parsePGN(uint32_t canId, uint32_t &pgn, uint8_t &source, uint8_t &priority);

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Wait for Serial connection (optional, comment out for standalone)
  delay(2000);
  Serial.println("OpenBridge VERSION 3 - NMEA2000 Logger (Pico)");
  Serial.println("==============================================");

  // Initialize SPI busses
  spi0.begin();
  spi1.begin();

  // Initialize MCP2515 CAN Controller
  Serial.print("Initializing CAN controller...");
  mcp2515.reset();

  // Set CAN speed to 250kbps (NMEA2000 standard)
  if (mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ) == MCP2515::ERROR_OK) {
    Serial.println("OK");
    canOK = true;
  } else {
    Serial.println("FAILED!");
    Serial.println("Check MCP2515 connections and crystal frequency.");
    blinkError(5);
    canOK = false;
  }

  // Set to normal mode (listen)
  if (canOK) {
    if (mcp2515.setNormalMode() == MCP2515::ERROR_OK) {
      Serial.println("CAN controller in normal mode");
    } else {
      Serial.println("ERROR: Failed to set normal mode!");
      canOK = false;
    }
  }

  // Initialize SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS_PIN, spi1)) {
    Serial.println("FAILED!");
    Serial.println("Insert SD card and reset.");
    blinkError(3);
    sdCardOK = false;
    // Continue anyway to allow CAN monitoring via Serial
  } else {
    Serial.println("OK");
    sdCardOK = true;

    // Print SD card info
    uint32_t cardSize = SD.size64() / (1024 * 1024);
    Serial.print("SD Card Size: ");
    Serial.print(cardSize);
    Serial.println(" MB");

    // Create logs directory if it doesn't exist
    if (!SD.exists("logs")) {
      SD.mkdir("logs");
      Serial.println("Created logs/ directory");
    }

    // Create initial log file
    createNewLogFile();
  }

  // Set initial time (Unix timestamp)
  // In production, this would come from RTC or GPS
  setTime(1738696007);  // Example timestamp
  lastRotate = now();

  if (canOK) {
    Serial.println("\nListening on NMEA2000 bus...");
    Serial.println();
  }

  digitalWrite(LED_PIN, HIGH);  // Ready
}

// ===== MAIN LOOP =====
void loop() {
  // Check for CAN messages
  if (canOK) {
    handleCANMessage();
  }

  // Check if buffer needs flushing
  if (millis() - lastFlush > FLUSH_INTERVAL) {
    flushBuffer();
    lastFlush = millis();
  }

  // Check if log file needs rotation
  if (sdCardOK && (now() - lastRotate > FILE_ROTATE_INTERVAL)) {
    rotateLogFile();
  }

  // Blink LED to show activity
  if (messageCount % 100 == 0 && messageCount > 0) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}

// ===== CAN MESSAGE HANDLER =====
void handleCANMessage() {
  struct can_frame frame;

  if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
    messageCount++;

    // Only process extended frames (NMEA2000 uses 29-bit IDs)
    if (frame.can_id & CAN_EFF_FLAG) {
      // Log to SD card if available
      if (sdCardOK) {
        logMessage(frame);
      }

      // Also output to Serial for debugging
      // Comment out this section for standalone operation to save processing time
      Serial.print(getTimestamp());
      Serial.print("  can0  ");
      Serial.print(formatCANID(frame.can_id));
      Serial.print("   [");
      Serial.print(frame.can_dlc);
      Serial.print("]  ");

      for (int i = 0; i < frame.can_dlc; i++) {
        if (frame.data[i] < 16) Serial.print("0");
        Serial.print(frame.data[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
  }
}

// ===== LOG MESSAGE TO SD CARD =====
void logMessage(struct can_frame &frame) {
  if (!logFile) return;

  // Format: (timestamp) can0 CANID [DLC] DATA
  char msgLine[128];
  char dataBytes[32] = "";

  // Build data bytes string
  for (int i = 0; i < frame.can_dlc; i++) {
    char hexByte[4];
    sprintf(hexByte, "%02X ", frame.data[i]);
    strcat(dataBytes, hexByte);
  }

  // Build complete log line
  sprintf(msgLine, "%s  can0  %s   [%d]  %s\n",
          getTimestamp().c_str(),
          formatCANID(frame.can_id).c_str(),
          frame.can_dlc,
          dataBytes);

  // Add to buffer
  int msgLen = strlen(msgLine);
  if (bufferPosition + msgLen >= BUFFER_SIZE) {
    flushBuffer();  // Buffer full, flush first
  }

  strcat(logBuffer, msgLine);
  bufferPosition += msgLen;
}

// ===== FLUSH BUFFER TO SD =====
void flushBuffer() {
  if (!sdCardOK || !logFile || bufferPosition == 0) return;

  logFile.write(logBuffer, bufferPosition);
  logFile.flush();

  // Clear buffer
  memset(logBuffer, 0, BUFFER_SIZE);
  bufferPosition = 0;
}

// ===== CREATE NEW LOG FILE =====
void createNewLogFile() {
  if (!sdCardOK) return;

  // Close existing file
  if (logFile) {
    flushBuffer();
    logFile.close();
  }

  // Generate filename: logs/YYYYMMDD_HHMMSS.log
  time_t t = now();
  sprintf(currentFilename, "logs/%04d%02d%02d_%02d%02d%02d.log",
          year(t), month(t), day(t),
          hour(t), minute(t), second(t));

  // Open new file
  logFile = SD.open(currentFilename, FILE_WRITE);

  if (logFile) {
    Serial.print("Created new log file: ");
    Serial.println(currentFilename);

    // Write header
    logFile.println("# OpenBridge VERSION 3 - NMEA2000 Data Log (Raspberry Pi Pico)");
    logFile.print("# Log started: ");
    logFile.print(year(t));
    logFile.print("-");
    if (month(t) < 10) logFile.print("0");
    logFile.print(month(t));
    logFile.print("-");
    if (day(t) < 10) logFile.print("0");
    logFile.print(day(t));
    logFile.print(" ");
    if (hour(t) < 10) logFile.print("0");
    logFile.print(hour(t));
    logFile.print(":");
    if (minute(t) < 10) logFile.print("0");
    logFile.print(minute(t));
    logFile.print(":");
    if (second(t) < 10) logFile.print("0");
    logFile.println(second(t));
    logFile.println("# Format: (timestamp) interface CANID [DLC] DATA");
    logFile.println();
    logFile.flush();
  } else {
    Serial.println("ERROR: Could not create log file!");
    blinkError(4);
  }

  lastRotate = now();
}

// ===== ROTATE LOG FILE =====
void rotateLogFile() {
  if (!sdCardOK) return;

  // Check file size
  unsigned long fileSize = logFile.size();

  if (fileSize >= MAX_FILE_SIZE) {
    Serial.print("File size limit reached (");
    Serial.print(fileSize);
    Serial.println(" bytes), rotating...");
    createNewLogFile();
  } else if (now() - lastRotate >= FILE_ROTATE_INTERVAL) {
    Serial.println("Time-based rotation, creating new file...");
    createNewLogFile();
  }
}

// ===== GET TIMESTAMP STRING =====
String getTimestamp() {
  // Format: (seconds.microseconds)
  unsigned long timestamp = now();
  unsigned long micros_part = micros() % 1000000;

  char timestampStr[32];
  sprintf(timestampStr, "(%lu.%06lu)", timestamp, micros_part);

  return String(timestampStr);
}

// ===== FORMAT CAN ID =====
// Builds the 29-bit CAN identifier in hex format
// NMEA2000 29-bit ID structure:
// Bits 28-26: Priority (3 bits)
// Bits 25-8:  PGN (18 bits)
// Bits 7-0:   Source Address (8 bits)
String formatCANID(uint32_t canId) {
  // Remove the extended frame flag bit
  uint32_t id = canId & 0x1FFFFFFF;

  // Format as 8-digit hex
  char idStr[10];
  sprintf(idStr, "%08lX", id);

  return String(idStr);
}

// ===== PARSE PGN FROM CAN ID =====
void parsePGN(uint32_t canId, uint32_t &pgn, uint8_t &source, uint8_t &priority) {
  // Remove extended frame flag
  uint32_t id = canId & 0x1FFFFFFF;

  // Extract components
  priority = (id >> 26) & 0x07;       // Bits 28-26
  pgn = (id >> 8) & 0x3FFFF;          // Bits 25-8
  source = id & 0xFF;                 // Bits 7-0
}

// ===== BLINK ERROR CODE =====
void blinkError(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
  delay(1000);
}
