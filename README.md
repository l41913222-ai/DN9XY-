# ESP8266 Amateur Radio UTC Clock (MAX7219)

This project realizes a compact, precise station clock designed for amateur radio shacks. It is powered by an **ESP8266 (NodeMCU / ESP8285)** and a **MAX7219 8x32 LED matrix display**. The clock displays the exact **UTC time** and a customizable **amateur radio callsign**.

## Features
* **Access Point Configuration:** No hardcoded Wi-Fi credentials in the source code! On the first boot (or if no network is found), the ESP8266 hosts its own Wi-Fi Access Point (Captive Portal). You can easily configure your local Wi-Fi via your smartphone or PC.
* **UTC Time Display:** Tailored specifically for amateur radio operations, displaying Coordinated Universal Time (UTC) without automatic daylight saving time shifts.
* **Callsign Integration:** Displays your personal amateur radio callsign alongside or alternating with the time.
* **NTP Synchronization:** Automatically fetches the exact time from the internet via Network Time Protocol, making it maintenance-free and highly accurate.

## Hardware Wiring

Connect the MAX7219 matrix display to your ESP8266 using the following hardware SPI pins:

| MAX7219 Pin | ESP8266 Pin | Description |
| :--- | :--- | :--- |
| **VCC** | 5V (VIN) | Power Supply |
| **GND** | GND | Ground |
| **DIN** | D7 (GPIO13) | Data Input (Hardware MOSI) |
| **CLK** | D5 (GPIO14) | Clock Line (Hardware SCK) |
| **CS** | D8 (GPIO15) | Chip Select |

## Required Libraries (Arduino IDE)
To compile this project, make sure to install the following libraries via the Library Manager:
* `MD_Parola` & `MD_MAX72XX` (Display control)
* `WiFiManager` (For the captive portal Wi-Fi configuration)

## Installation & Setup
1. Upload the code to your ESP8266 board.
2. Once booted, connect to the Wi-Fi network hosted by the ESP8266 (e.g., `ESP8266-Clock-Setup`).
3. A configuration page should open automatically in your browser.
4. Select your home Wi-Fi network, enter your password, and save.
5. The clock will restart, connect to your network, sync via NTP, and start displaying the UTC time and your callsign.
