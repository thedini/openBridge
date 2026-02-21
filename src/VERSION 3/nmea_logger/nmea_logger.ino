/*
 * OpenBridge VERSION 3 - NMEA2000 Data Logger
 *
 * Teensy 4.1 based standalone data logger for NMEA2000 bus traffic.
 * Logs all messages to SD card in candump hex format.
 *
 * Hardware Requirements:
 * - Teensy 4.1 (required for SD card slot)
 * - VP230 CAN transceiver
 * - MicroSD card (FAT32)
 * - Optional: DS3231 RTC module
 *
 * Connections:
 * - Pin 22 (CAN1 TX) → VP230 TX
 * - Pin 23 (CAN1 RX) → VP230 RX
 * - Pin 13 → LED (status indicator)
 *
 * Output Format (candump compatible):
 * (timestamp) can0 CANID [DLC] DATA_BYTES
 *
 * Example:
 * (1738696007.167910)  can0  09F11224   [8]  2E 99 99 00 00 64 F6 FD
 */

#include <NMEA2000_Teensyx.h>
#include <N2kMessages.h>
#include <SD.h>
#include <TimeLib.h>

// ===== CONFIGURATION =====
#define LED_PIN 13              // Status LED
#define SD_CHIP_SELECT BUILTIN_SDCARD  // Teensy 4.1 built-in SD
#define BUFFER_SIZE 512         // SD write buffer size (matches sector)
#define FLUSH_INTERVAL 1000     // Flush buffer every 1 second (ms)
#define MAX_FILE_SIZE 104857600 // 100 MB in bytes
#define FILE_ROTATE_INTERVAL 3600  // New file every hour (seconds)

// ===== GLOBAL VARIABLES =====
tNMEA2000_Teensyx NMEA2000;
File logFile;
char logBuffer[BUFFER_SIZE];
int bufferPosition = 0;
unsigned long lastFlush = 0;
unsigned long lastRotate = 0;
unsigned long messageCount = 0;
bool sdCardOK = false;
char currentFilename[32];

// ===== FUNCTION PROTOTYPES =====
void handleNMEA2000Msg(const tN2kMsg &N2kMsg);
void logMessage(const tN2kMsg &N2kMsg);
void flushBuffer();
void rotateLogFile();
void createNewLogFile();
String getTimestamp();
String formatCANID(unsigned long pgn, unsigned char source, unsigned char priority);
void blinkError(int count);

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Wait for Serial connection (optional, comment out for standalone)
  delay(2000);
  Serial.println("OpenBridge VERSION 3 - NMEA2000 Logger");
  Serial.println("========================================");

  // Initialize SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CHIP_SELECT)) {
    Serial.println("FAILED!");
    Serial.println("Insert SD card and reset.");
    blinkError(3);
    sdCardOK = false;
    // Continue anyway to allow CAN monitoring via Serial
  } else {
    Serial.println("OK");
    sdCardOK = true;

    // Print SD card info
    uint64_t cardSize = SD.sdfs.totalSize() / (1024 * 1024);
    uint64_t freeSize = (SD.sdfs.totalSize() - SD.sdfs.usedSize()) / (1024 * 1024);
    Serial.printf("SD Card Size: %llu MB\n", cardSize);
    Serial.printf("Free Space: %llu MB\n", freeSize);

    // Create logs directory if it doesn't exist
    if (!SD.exists("logs")) {
      SD.mkdir("logs");
      Serial.println("Created logs/ directory");
    }

    // Create initial log file
    createNewLogFile();
  }

  // Initialize NMEA2000 in listen-only mode
  Serial.println("\nInitializing CAN interface...");

  // Set to listen-only mode (no transmission)
  NMEA2000.SetMode(tNMEA2000::N2km_ListenOnly);
  NMEA2000.EnableForward(false);  // Disable forwarding
  NMEA2000.SetMsgHandler(handleNMEA2000Msg);

  // Open CAN interface
  if (NMEA2000.Open()) {
    Serial.println("CAN interface initialized successfully");
    Serial.println("Listening on NMEA2000 bus...");
    Serial.println();
  } else {
    Serial.println("ERROR: Failed to initialize CAN interface!");
    blinkError(5);
  }

  // Set initial time (Unix timestamp)
  // In production, this would come from RTC or GPS
  setTime(1738696007);  // Example timestamp
  lastRotate = now();

  digitalWrite(LED_PIN, HIGH);  // Ready
}

// ===== MAIN LOOP =====
void loop() {
  // Parse incoming NMEA2000 messages
  NMEA2000.ParseMessages();

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

// ===== MESSAGE HANDLER =====
void handleNMEA2000Msg(const tN2kMsg &N2kMsg) {
  messageCount++;

  // Log to SD card if available
  if (sdCardOK) {
    logMessage(N2kMsg);
  }

  // Also output to Serial for debugging
  // Comment out this section for standalone operation to save processing time
  Serial.print(getTimestamp());
  Serial.print("  can0  ");
  Serial.print(formatCANID(N2kMsg.PGN, N2kMsg.Source, N2kMsg.Priority));
  Serial.print("   [");
  Serial.print(N2kMsg.DataLen);
  Serial.print("]  ");

  for (int i = 0; i < N2kMsg.DataLen; i++) {
    if (N2kMsg.Data[i] < 16) Serial.print("0");
    Serial.print(N2kMsg.Data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

// ===== LOG MESSAGE TO SD CARD =====
void logMessage(const tN2kMsg &N2kMsg) {
  if (!logFile) return;

  // Format: (timestamp) can0 CANID [DLC] DATA
  char msgLine[128];
  char dataBytes[32] = "";

  // Build data bytes string
  for (int i = 0; i < N2kMsg.DataLen; i++) {
    char hexByte[4];
    sprintf(hexByte, "%02X ", N2kMsg.Data[i]);
    strcat(dataBytes, hexByte);
  }

  // Build complete log line
  sprintf(msgLine, "%s  can0  %s   [%d]  %s\n",
          getTimestamp().c_str(),
          formatCANID(N2kMsg.PGN, N2kMsg.Source, N2kMsg.Priority).c_str(),
          N2kMsg.DataLen,
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
    Serial.printf("Created new log file: %s\n", currentFilename);

    // Write header
    logFile.println("# OpenBridge VERSION 3 - NMEA2000 Data Log");
    logFile.printf("# Log started: %04d-%02d-%02d %02d:%02d:%02d\n",
                   year(t), month(t), day(t),
                   hour(t), minute(t), second(t));
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
    Serial.printf("File size limit reached (%lu bytes), rotating...\n", fileSize);
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
String formatCANID(unsigned long pgn, unsigned char source, unsigned char priority) {
  // Build 29-bit identifier
  unsigned long canID = 0;

  // Add priority (bits 28-26)
  canID |= ((unsigned long)priority & 0x07) << 26;

  // Add PGN (bits 25-8)
  canID |= (pgn & 0x3FFFF) << 8;

  // Add source address (bits 7-0)
  canID |= source & 0xFF;

  // Format as 8-digit hex
  char idStr[10];
  sprintf(idStr, "%08lX", canID);

  return String(idStr);
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
