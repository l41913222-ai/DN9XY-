# DN9XY Matrix Station (8-Module Edition)

This project is a versatile LED Matrix controller designed for amateur radio stations, powered by an **ESP-12F** and an **8x 8x8 MAX7219 LED matrix** chain. It provides a user-friendly web interface to manage your station data.

## Features
- **Web Interface:** Easily configure your Callsign, Locator, and Custom text via a web browser.
- **Multiple Display Modes:** - Time only (UTC/CEST toggle).
  - Callsign & Locator.
  - Custom text.
  - Automatic cycle mode (rotating through all).
- **Time Synchronization:** Simple one-click synchronization with your smartphone time.
- **Access Point Mode:** No router needed; the device creates its own Wi-Fi network.

## Hardware Setup
- **Microcontroller:** ESP-12F (or ESP8266 DevKit v1).
- **Display:** 8x 8x8 LED Matrix modules (MAX7219) connected in a chain.

### Wiring
| Matrix Pin | ESP-12F Pin |
| :--- | :--- |
| **DIN** | D7 (GPIO 13) |
| **CS** | D8 (GPIO 15) |
| **CLK** | D5 (GPIO 14) |

> **Important:** Always use an external 5V power supply for the LED matrix chain to avoid damaging the ESP-12F. Connect the GND of the power supply to the GND of the ESP-12F (Common Ground).

## Build Guide
1. **Connect the Matrices:** Connect all 8 modules in series using the ribbon cables. Ensure the arrows on the back point in the same direction (signal flow from DIN to DOUT).
2. **Connect the ESP:** Follow the wiring table above.
3. **Power:** Feed 5V to the matrix chain and ensure a common ground with the ESP.
4. **Flash:** Upload the provided code using the Arduino IDE (ensure `MD_Parola` and `MD_MAX72xx` libraries are installed).

## Usage
1. **Connect:** Search for the Wi-Fi network **"DN9XY_MATRIX"** (Password: `12345678`).
2. **Configure:** Open `http://192.168.4.1` in your browser.
3. **Sync:** Click the "Sync Handyzeit" button to synchronize the clock with your local device time.

## Troubleshooting
* **Mirrored/Scrambled Text:** Change `#define HARDWARE_TYPE` in the code from `FC16_HW` to `GENERIC_HW`.
* **Mirrored horizontally:** Uncomment `P.setZoneEffect(0, true, PA_FLIP_LR);` in the `setup()` function.
