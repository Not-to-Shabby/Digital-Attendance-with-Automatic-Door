# Digital-Attendance-with-Automatic-Door

An digital attendance using RFID technology with time logging capabilities and servo-controlled door mechanism. The system includes real-time monitoring, LCD display feedback, and SD card logging functionality.

## Hardware Requirements

- Arduino board (Uno or compatible)
- MFRC522 RFID reader module
- DS1302 RTC (Real-Time Clock) module
- 16x2 I2C LCD Display
- Micro SD card module
- Servo motor
- RFID cards/tags
- Jumper wires
- Breadboard

## Pin Connections

### RFID Module (MFRC522):
- SS (SDA) → Digital Pin 10
- RST → Digital Pin 9
- MOSI → MOSI (11)
- MISO → MISO (12)
- SCK → SCK (13)
- 3.3V → 3.3V
- GND → GND

### SD Card Module:
- CS → Digital Pin 4
- MOSI → MOSI (11)
- MISO → MISO (12)
- SCK → SCK (13)
- VCC → 5V
- GND → GND

### RTC Module (DS1302):
- DAT → Digital Pin 6
- CLK → Digital Pin 7
- RST → Digital Pin 8
- VCC → 5V
- GND → GND

### LCD Display (I2C):
- SDA → Arduino SDA
- SCL → Arduino SCL
- VCC → 5V
- GND → GND

### Servo Motor:
- Signal → Digital Pin 3
- VCC → 5V
- GND → GND

## Features

- RFID card reading and validation
- Real-time clock integration
- Access logging to SD card
- LCD feedback display
- Servo-controlled door mechanism
- Invalid card detection
- Detailed time-stamped logging
- Automatic door closure
- Real-time display updates

## Installation

1. Required Arduino Libraries:
   ```cpp
   #include <SPI.h>
   #include <MFRC522.h>
   #include <Servo.h>
   #include <SD.h>
   #include <Wire.h>
   #include <LiquidCrystal_I2C.h>
   #include <ThreeWire.h>
   #include <RtcDS1302.h>
   ```

2. Install all required libraries through Arduino IDE:
   - Go to Sketch → Include Library → Manage Libraries
   - Search and install each library listed above

3. Connect hardware components according to pin configuration
4. Format SD card as FAT32
5. Upload the code to Arduino

## System Operation

1. System Initialization:
   - RTC synchronization
   - SD card preparation
   - RFID reader activation
   - Servo positioning
   - LCD display setup

2. Normal Operation:
   - Displays "Ready for Card" and current time
   - Updates time display every second
   - Waits for RFID card presentation

3. Card Detection:
   - Reads card UID
   - Logs access attempt
   - Validates card
   - Controls door access
   - Provides visual feedback

## Access Log Format

The system creates a log file (access.txt) with the following format:
```
[Card UID], MM/DD/YYYY HH:MM:SS
```

## Customization Options

Modify these constants in the code to adjust system behavior:
```cpp
#define CLOSED_ANGLE 0      // Servo closed position
#define OPEN_ANGLE 90       // Servo open position
const long interval = 1000  // Time display update interval
```

## Security Features

- Authorized card UID verification
- Time-stamped access logging
- Automatic door closure
- Invalid card alerts
- Real-time monitoring capability

## Troubleshooting

1. RFID Reader Issues:
   - Check SPI connections
   - Verify power supply
   - Ensure correct SS and RST pins

2. SD Card Problems:
   - Verify card is formatted as FAT32
   - Check CS pin connection
   - Ensure proper SPI wiring
   - Power supply voltage check

3. RTC Issues:
   - Check battery voltage
   - Verify wire connections
   - Confirm correct pin assignments

4. LCD Display Problems:
   - Verify I2C address (default 0x27)
   - Check I2C connections
   - Adjust contrast if needed

5. Servo Motor Issues:
   - Check power supply
   - Verify signal pin connection
   - Adjust angle limits if needed

## Maintenance

- Regularly backup SD card logs
- Check RTC battery periodically
- Clean RFID reader surface
- Test servo operation
- Verify all connections
