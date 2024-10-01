# Bluetooth Usage with Seeed Studio XIAO nRF52840

This repository demonstrates Bluetooth features for Seeed Studio XIAO nRF52840 and XIAO nRF52840 Sense boards. For detailed documentation, visit the official [Seeed Studio XIAO BLE Sense Bluetooth Usage Guide](https://wiki.seeedstudio.com/XIAO-BLE-Sense-Bluetooth-Usage/).

## Table of Contents
1. [Getting Started](#getting-started)
2. [Requirements](#requirements)
3. [Installation](#installation)
4. [Examples](#examples)
5. [License](#license)
6. [Support](#support)

## Getting Started

Learn to control hardware using Bluetooth with the XIAO nRF52840 series. 

## Requirements

- **Hardware:**
  - Seeed Studio XIAO nRF52840 or XIAO nRF52840 Sense
  - USB Type-C Cable
  - Smartphone/PC with Bluetooth

- **Software:**
  - Arduino IDE
  - ArduinoBLE library
  - nRF Connect (Android) or LightBlue (iOS)

## Installation

1. Install the ArduinoBLE library via the Library Manager or by adding the ZIP file.
2. Refer to the [Seeed Wiki](https://wiki.seeedstudio.com/XIAO-BLE-Sense-Bluetooth-Usage/) for details.

## Examples

### Example 1: Control LED
- **Upload:** [BLE_LED_Control.ino](examples/BLE_LED_Control/BLE_LED_Control.ino)
- **Usage:** Send `1` to turn on the LED, `0` to turn it off.

### Example 2: 24GHz Radar
- **Upload:** [SleepRadar.ino](examples/SleepRadar/SleepRadar.ino)
- **Usage:** Detect movements and send data via Bluetooth.

### Example 3: Two XIAO Boards
- **Peripheral Code:** [BLE_Peripheral.ino](examples/BLE_TwoBoards/BLE_Peripheral.ino)
- **Central Code:** [BLE_Central.ino](examples/BLE_TwoBoards/BLE_Central.ino)
- **Usage:** Control an LED on one board from the other.

## Support

For issues or questions, visit the Seeed Studio [support channels](https://wiki.seeedstudio.com/XIAO-BLE-Sense-Bluetooth-Usage/).
