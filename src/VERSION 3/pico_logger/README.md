# VERSION 3 - Raspberry Pi Pico Logger

This directory contains the Raspberry Pi Pico implementation of OpenBridge VERSION 3, the NMEA2000 data logger.

## Overview

Unlike the Teensy 4.1 version which has built-in CAN and SD card support, the Raspberry Pi Pico version uses external modules:
- **MCP2515 CAN Controller** - SPI-based CAN interface
- **SD Card Module** - SPI-based storage

This makes it more modular and potentially cheaper, though requires more wiring.

## Hardware Requirements

### Essential Components

1. **Raspberry Pi Pico or Pico W** (~$4-6)
   - Either standard Pico or Pico W works
   - WiFi capability not used in this version but available for future enhancement

2. **MCP2515 CAN Bus Module** (~$3-5)
   - Common red PCB module with TJA1050 transceiver
   - 8MHz crystal version (most common)
   - Includes CAN transceiver (no separate VP230 needed)

3. **SD Card Module** (~$1-2)
   - Standard SPI SD card reader module
   - Works with microSD or SD cards
   - 3.3V compatible

4. **MicroSD Card** (~$5-10)
   - Minimum 1GB, recommend 8GB+
   - FAT32 formatted
   - Class 10 or better

### Optional Components

- **DS3231 RTC Module** - For accurate timestamps
- **Breadboard or Perf Board** - For assembly
- **Enclosure** - For protection

### Total Cost

**Pico Version:** ~$15-25 (vs ~$35-45 for Teensy 4.1 version)

## Pin Connections

### MCP2515 CAN Module (SPI0)

```
MCP2515 Module    →    Raspberry Pi Pico
--------------         -----------------
VCC               →    3.3V (Pin 36)
GND               →    GND (Pin 38)
CS                →    GP17 (Pin 22)
SO (MISO)         →    GP16 (Pin 21)
SI (MOSI)         →    GP19 (Pin 25)
SCK               →    GP18 (Pin 24)
INT               →    GP20 (Pin 26)
```

### SD Card Module (SPI1)

```
SD Card Module    →    Raspberry Pi Pico
--------------         -----------------
VCC               →    3.3V (Pin 36)
GND               →    GND (Pin 38)
CS                →    GP13 (Pin 17)
MISO              →    GP12 (Pin 16)
MOSI              →    GP11 (Pin 15)
SCK               →    GP10 (Pin 14)
```

### NMEA2000 Bus Connection

```
MCP2515 Module    →    NMEA2000 Bus
--------------         ------------
CAN_H             →    CAN_H (Blue/White)
CAN_L             →    CAN_L (White/Blue)
```

### Power Options

**Option 1: USB Power (Development)**
- Use Pico's micro-USB port
- 5V from computer or USB charger

**Option 2: NMEA2000 Bus Power (Production)**
- NMEA2000 provides 12V on power wires
- Add buck converter: 12V → 5V
- Feed 5V into Pico's VSYS pin (Pin 39)
- Or use VBUS pin (Pin 40) with diode protection

### Complete Wiring Diagram

```
                                    NMEA2000 Bus
                                    ┌─────────┐
                                    │ CAN_H   │
                                    │ CAN_L   │
                                    │ 12V     │
                                    │ GND     │
                                    └────┬────┘
                                         │
                    ┌────────────────────┴──────────────┐
                    │                                   │
              ┌─────▼─────┐                      ┌─────▼──────┐
              │  MCP2515  │                      │ 12V→5V Buck│
              │ CAN Module│                      │  Converter │
              └─────┬─────┘                      └─────┬──────┘
                    │ SPI0                             │ 5V
                    │                                  │
        ┌───────────┴──────────────┬──────────────────┴────────┐
        │                          │                            │
   ┌────▼─────────────────────────▼──────────────────┐         │
   │         Raspberry Pi Pico                       │         │
   │                                                  │         │
   │  GP10-13: SPI1 → SD Card                       │◄────────┘
   │  GP16-20: SPI0 → MCP2515                       │
   │  GP25: LED                                      │
   │                                                  │
   └────┬───────────────────────────────────────────┘
        │ SPI1
   ┌────▼─────┐
   │ SD Card  │
   │  Module  │
   └──────────┘
```

## Software Installation

### 1. Install Arduino IDE Support for Pico

Install the **Raspberry Pi Pico/RP2040** board support:

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add to "Additional Board Manager URLs":
   ```
   https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
   ```
4. Go to **Tools → Board → Board Manager**
5. Search for "pico"
6. Install **"Raspberry Pi Pico/RP2040" by Earle F. Philhower III**

### 2. Install Required Libraries

Install these libraries via **Library Manager** (Sketch → Include Library → Manage Libraries):

1. **MCP2515** by Autowp
   - Search: "MCP2515"
   - Version: Latest

2. **SD** (Built-in)
   - Should already be included with Pico board support

3. **Time** by Michael Margolis
   - Search: "Time"
   - Version: Latest

### 3. Configure Arduino IDE

Set these options under **Tools** menu:

- **Board:** "Raspberry Pi Pico"
- **Flash Size:** "2MB (Sketch: 1MB, FS: 1MB)"
- **CPU Speed:** "133 MHz" (or 125 MHz)
- **USB Stack:** "Pico SDK"
- **Port:** Select your Pico's COM port

### 4. Upload the Firmware

1. Open `pico_logger.ino` in Arduino IDE
2. Connect Pico via USB while holding BOOTSEL button
3. Select the Pico's port
4. Click Upload
5. After upload, open Serial Monitor (115200 baud) to see status

## Configuration

### Basic Settings

Edit these constants in `pico_logger.ino`:

```cpp
#define BUFFER_SIZE 512         // Write buffer size
#define FLUSH_INTERVAL 1000     // Flush every 1 second (ms)
#define MAX_FILE_SIZE 104857600 // 100 MB max file size
#define FILE_ROTATE_INTERVAL 3600  // Rotate every 1 hour (seconds)
```

### MCP2515 Crystal Frequency

Most MCP2515 modules use 8MHz crystal. If yours is different:

```cpp
// For 16MHz crystal:
mcp2515.setBitrate(CAN_250KBPS, MCP_16MHZ);

// For 8MHz crystal (default):
mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
```

### Pin Configuration

If you need different pins, edit these defines:

```cpp
// MCP2515 CAN Controller
#define CAN_CS_PIN 17      // Chip Select
#define CAN_INT_PIN 20     // Interrupt (currently unused)

// SD Card Module
#define SD_CS_PIN 13       // Chip Select
```

## Output Format

Same as Teensy version - candump compatible hex format:

```
(1738696007.167910)  can0  09F11224   [8]  2E 99 99 00 00 64 F6 FD
```

Files are saved in `logs/` directory on SD card with format:
```
logs/YYYYMMDD_HHMMSS.log
```

## LED Status Codes

The built-in LED (GP25) indicates system status:

- **Solid ON**: System ready, logging active
- **Blinking**: Message activity (toggles every 100 messages)
- **3 blinks + pause**: SD card initialization failed
- **4 blinks + pause**: Cannot create log file
- **5 blinks + pause**: CAN controller initialization failed

## Troubleshooting

### CAN Controller Not Initializing

**Symptoms:** 5 blinks on LED, "CAN controller: FAILED" message

**Solutions:**
1. Check MCP2515 wiring (especially CS, MOSI, MISO, SCK)
2. Verify crystal frequency setting matches your module (8MHz vs 16MHz)
3. Check 3.3V power supply to MCP2515
4. Try different CS pin if conflicts exist
5. Measure SPI signals with logic analyzer

### SD Card Not Detected

**Symptoms:** 3 blinks on LED, "SD card: FAILED" message

**Solutions:**
1. Ensure card is FAT32 formatted
2. Try different SD card (some are incompatible)
3. Check SD module wiring
4. Verify 3.3V power to SD module
5. Try slower SPI speed in code
6. Use smaller capacity card (<32GB)

### No CAN Messages Received

**Symptoms:** No serial output, log file empty

**Solutions:**
1. Verify NMEA2000 bus is active and has termination (120Ω)
2. Check CAN_H and CAN_L connections to MCP2515
3. Verify MCP2515 initialized successfully
4. Check that NMEA2000 bus is at 250kbps
5. Look for bus voltage (should see ~2.5V on CAN_H and CAN_L when idle)

### Incorrect Timestamps

**Symptoms:** Timestamps don't match real time

**Solutions:**
1. Timestamps start from hardcoded value (1738696007)
2. Add RTC module for persistent time
3. If using Pico W, can sync time via NTP
4. Time resets on every power cycle without RTC

### Buffer Overruns / Missing Messages

**Symptoms:** Gaps in message sequence, corrupted data

**Solutions:**
1. Increase `BUFFER_SIZE` (multiples of 512)
2. Decrease `FLUSH_INTERVAL` for more frequent writes
3. Use faster (Class 10 or UHS) SD card
4. Disable Serial debug output in `handleCANMessage()`
5. Overclock Pico to 250 MHz if needed

## Performance

**Expected Performance:**
- Maximum CAN message rate: ~5,000 messages/second
- Typical NMEA2000 load: 100-500 messages/second
- SD write latency: <20ms with buffering
- Pico has plenty of headroom for this application

**Storage Usage:**
- Same as Teensy version
- ~60 bytes per message (formatted)
- At 200 msg/sec: ~1 MB per minute = ~60 MB per hour
- 8GB SD card: ~130 hours of continuous logging

## Advantages vs Teensy 4.1 Version

**Cost:**
- Pico version: ~$15-25
- Teensy version: ~$35-45
- **Savings: ~40-50%**

**Availability:**
- Pico more widely available
- MCP2515 modules very common
- SD modules everywhere

**Modularity:**
- Can swap CAN or SD modules independently
- Easy to breadboard/prototype
- More pin flexibility

## Disadvantages vs Teensy 4.1 Version

**Complexity:**
- More wiring required
- Two separate SPI devices to configure
- External modules vs integrated

**Performance:**
- Slower CPU (133 MHz vs 600 MHz)
- SPI overhead for CAN access
- Lower max message throughput

**Size:**
- Larger footprint with separate modules
- More difficult to fit in small enclosure

## Future Enhancements

Potential improvements specific to Pico version:

- **Pico W WiFi Features:**
  - Web interface for log browsing
  - Real-time streaming to network
  - NTP time synchronization
  - Remote configuration

- **Dual Core:**
  - Use second core for SD writes
  - Parallel processing of CAN and storage

- **Multiple CAN Buses:**
  - Add second MCP2515 on different SPI pins
  - Log from multiple buses simultaneously

- **Power Management:**
  - Sleep modes during low activity
  - Battery operation with auto-wake

## Comparison: Pico vs Teensy 4.1

| Feature | Pico Version | Teensy 4.1 |
|---------|--------------|------------|
| **Cost** | $15-25 | $35-45 |
| **CPU** | 133 MHz RP2040 | 600 MHz ARM M7 |
| **CAN** | External MCP2515 | Built-in (3x) |
| **SD Card** | External module | Built-in slot |
| **Wiring** | Complex | Simple |
| **Max Messages/sec** | ~5,000 | >10,000 |
| **WiFi Option** | Yes (Pico W) | No (without module) |
| **Availability** | Excellent | Good |
| **Board Size** | Small + modules | Medium |

## Related Documentation

- [Main VERSION 3 Documentation](../../../docs/version3.md) - Overview
- [Teensy 4.1 Version](../nmea_logger/) - Alternative implementation
- [CAN Protocol](../../../docs/can_protocol.md) - Understanding NMEA2000
- [Bill of Materials](../../../docs/bom.md) - Component sources

## Example: Complete Shopping List

For a complete Pico-based VERSION 3 logger:

1. Raspberry Pi Pico (or Pico W) - $4-6
2. MCP2515 CAN Bus Module (8MHz) - $3-5
3. SD Card Module - $1-2
4. 8GB microSD Card (Class 10) - $5
5. Breadboard or perf board - $2-5
6. Jumper wires - $2-5
7. (Optional) DS3231 RTC - $2-4
8. (Optional) Buck converter 12V→5V - $1-3
9. (Optional) Enclosure - $5-10

**Total: $20-40** depending on options

## Contributing

When contributing to the Pico version:

1. Test with real NMEA2000 hardware
2. Verify compatibility with different MCP2515 modules
3. Test with various SD card brands/speeds
4. Maintain candump format compatibility
5. Update this README

## License

OpenBridge is an open-source project. See repository root for license information.

---

**Happy Logging with Raspberry Pi Pico!**
