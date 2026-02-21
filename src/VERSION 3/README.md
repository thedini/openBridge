# VERSION 3 - Source Code

This directory contains the complete Arduino firmware for OpenBridge VERSION 3, the Teensy 4.1-based NMEA2000 data logger.

## Directory Structure

```
VERSION 3/
├── nmea_logger/            # Main data logger firmware
│   └── nmea_logger.ino     # Complete logging implementation
├── examples/               # Example configurations and utilities
│   └── (coming soon)
└── README.md              # This file
```

## Quick Start

### 1. Hardware Setup

**Required:**
- Teensy 4.1 (must be 4.1 for SD card slot)
- VP230 CAN transceiver
- MicroSD card (FAT32 formatted)
- NMEA2000 connection cable

**Connections:**
```
Teensy 4.1          VP230           NMEA2000 Bus
-----------         -----           ------------
Pin 22 (CAN TX) →   TX
Pin 23 (CAN RX) →   RX
3.3V            →   VCC
GND             →   GND
                    CANH        →   CAN_H
                    CANL        →   CAN_L
```

### 2. Software Installation

**Required Arduino Libraries:**
1. **NMEA2000_Teensyx** - Teensy CAN library
   - Install via: Library Manager → Search "NMEA2000_Teensyx"
   - Or: https://github.com/ttlappalainen/NMEA2000_Teensyx

2. **NMEA2000** - Core NMEA2000 library
   - Install via: Library Manager → Search "NMEA2000"
   - Or: https://github.com/ttlappalainen/NMEA2000

3. **Time** - Time library
   - Install via: Library Manager → Search "Time"

**Teensy Configuration:**
- Board: Teensy 4.1
- USB Type: Serial
- CPU Speed: 600 MHz
- Optimize: Faster

### 3. Upload Firmware

1. Open `nmea_logger/nmea_logger.ino` in Arduino IDE
2. Select **Tools → Board → Teensy 4.1**
3. Select your Teensy's serial port
4. Click Upload
5. Open Serial Monitor (115200 baud) to see status

### 4. Prepare SD Card

1. Format microSD card as **FAT32**
2. Insert into Teensy 4.1 SD slot (bottom of board)
3. Power cycle the Teensy
4. Check Serial Monitor for "SD Card: OK"

## Configuration

### Basic Configuration

Edit these constants in `nmea_logger.ino`:

```cpp
#define LED_PIN 13              // Status LED pin
#define BUFFER_SIZE 512         // Write buffer size
#define FLUSH_INTERVAL 1000     // Flush every 1 second (ms)
#define MAX_FILE_SIZE 104857600 // 100 MB max file size
#define FILE_ROTATE_INTERVAL 3600  // Rotate every 1 hour (seconds)
```

### Timestamp Configuration

The firmware uses Unix timestamps. For accurate logging, you need to set the initial time:

**Option 1: Set in code (simple)**
```cpp
setTime(1738696007);  // Set to current Unix timestamp
```

**Option 2: Add RTC module (recommended)**
Connect DS3231 RTC module via I2C and modify the code to read from RTC.

**Option 3: GPS module**
Use GPS time for automatic, accurate timestamps.

## Output Format

The logger creates files in `logs/` directory with candump-compatible format:

```
logs/
├── 20251021_120000.log
├── 20251021_130000.log
└── 20251021_140000.log
```

### File Format

Each line contains one CAN message:

```
(1738696007.167910)  can0  09F11224   [8]  2E 99 99 00 00 64 F6 FD
```

Fields:
- `(timestamp)` - Unix time with microseconds
- `can0` - Interface (always can0)
- `09F11224` - 29-bit CAN ID in hex
- `[8]` - Data length (0-8 bytes)
- `2E 99...` - Data bytes in hex

### Parsing CAN ID

The 8-digit hex CAN ID contains:
```
Example: 09F11224

Breakdown:
  0 9 F 1 1 2 2 4
  │ └─┬─┘ └─┬─┘ └─ Source Address (0x24 = 36)
  │   │     └────── PGN (0x1F112 = 130306 = Wind Data)
  └───┴──────────── Priority (0)
```

## LED Status Codes

The onboard LED (pin 13) indicates system status:

- **Solid ON**: System ready, logging active
- **Blinking**: Message activity (toggles every 100 messages)
- **3 blinks + pause**: SD card initialization failed
- **4 blinks + pause**: Cannot create log file
- **5 blinks + pause**: CAN interface failed

## Analyzing Logged Data

### Import into candump tools

The log format is compatible with Linux CAN utilities:

```bash
# View log with candump formatting
cat logs/20251021_120000.log | grep -v "^#"

# Filter by PGN (example: Wind Data = 0x1F112)
grep "09F112" logs/20251021_120000.log

# Count messages
grep -c "can0" logs/20251021_120000.log
```

### Import into Wireshark

1. Convert log to Wireshark format (pcap)
2. Use `text2pcap` or custom converter
3. Analyze with DBC files for NMEA2000

### Python Analysis

```python
import re

with open('logs/20251021_120000.log', 'r') as f:
    for line in f:
        if line.startswith('#'):
            continue  # Skip header

        # Parse line
        match = re.match(r'\((\d+\.\d+)\)\s+(\w+)\s+(\w+)\s+\[(\d)\]\s+(.*)', line)
        if match:
            timestamp, interface, can_id, dlc, data = match.groups()
            # Process message...
```

## Performance

**Expected Performance:**
- Maximum throughput: >10,000 messages/second
- Typical NMEA2000 bus load: 100-500 messages/second
- Write latency: <10ms (with buffering)

**Storage Usage:**
- ~60 bytes per message (formatted)
- At 200 msg/sec: ~1 MB per minute
- At 200 msg/sec: ~60 MB per hour
- 16GB SD card: ~260 hours of continuous logging

## Troubleshooting

### SD Card Not Detected

**Symptoms:** Serial shows "SD card: FAILED"

**Solutions:**
1. Ensure card is FAT32 formatted
2. Try different SD card (some are incompatible)
3. Check card is fully inserted
4. Use Class 10 or better card
5. Try smaller capacity card (<32GB)

### No Messages Logged

**Symptoms:** Log file empty or no new entries

**Solutions:**
1. Verify NMEA2000 bus is active
2. Check CAN transceiver connections
3. Verify bus termination (120Ω)
4. Check Serial Monitor for CAN initialization errors
5. Ensure firmware is in listen-only mode

### Incorrect Timestamps

**Symptoms:** Timestamps don't match real time

**Solutions:**
1. Set correct time in setup()
2. Add RTC module for persistent time
3. Use GPS for automatic time sync
4. After power cycle, time resets to compiled default

### Buffer Overruns

**Symptoms:** Missing messages under heavy load

**Solutions:**
1. Increase `BUFFER_SIZE` (multiples of 512)
2. Decrease `FLUSH_INTERVAL` for more frequent writes
3. Use faster SD card (Class 10 or UHS)
4. Disable Serial debug output

## Advanced Features

### Multiple CAN Interfaces

Teensy 4.1 has 3 CAN interfaces. To log from multiple buses:

```cpp
// Add CAN2 and CAN3
tNMEA2000_Teensyx NMEA2000_2(&CAN2);
tNMEA2000_Teensyx NMEA2000_3(&CAN3);

// In setup():
NMEA2000_2.SetMode(tNMEA2000::N2km_ListenOnly);
NMEA2000_2.SetMsgHandler(handleNMEA2000Msg);
NMEA2000_2.Open();
```

Update log format to include interface identifier.

### Filtering Messages

To log only specific PGNs:

```cpp
void handleNMEA2000Msg(const tN2kMsg &N2kMsg) {
  // Only log wind data (PGN 130306)
  if (N2kMsg.PGN != 130306) return;

  logMessage(N2kMsg);
}
```

### Compression

For long-term logging, implement compression:
- Use miniz library for zlib compression
- Compress on-the-fly or compress old files
- Trade CPU time for storage space

## Examples

See the `examples/` directory for:
- Log analysis scripts
- Configuration templates
- Integration examples
- Converter utilities

## Contributing

To contribute to VERSION 3:

1. Test changes with real NMEA2000 hardware
2. Verify log format remains candump-compatible
3. Document any new features
4. Update this README
5. Submit pull request

## Related Documentation

- [VERSION 3 Overview](../../docs/version3.md) - Complete documentation
- [CAN Protocol](../../docs/can_protocol.md) - Understanding the protocol
- [Bill of Materials](../../docs/bom.md) - Required hardware

## License

OpenBridge is an open-source project. See repository root for license information.

---

**Happy Logging!**
