# Development Environment Setup

## Arduino IDE
Arudino IDE was used to upload the firmware for this project. Cursor was used for coding.
Those are arbitrary choices, other IDEs could be used.

Verified version: 2.3.8
Install [Arduino IDE](https://www.arduino.cc/en/software/)


## ESP32 Board
Install ESP32 board by Espressif, follow this [Getting Started document](
https://randomnerdtutorials.com/getting-started-esp32-c3-super-mini/)

Verified version: v2.0.17

> NOTE: DO NOT upgrade to 3.x, as it has breaking changes

## Arduino IDE Configuration

### Tools
Click the top menu `Tools` item, then verify or modify each sub-menu item per the table below

| Sub Menu Item  | Value      | Description                                                                 |
|------------|------------|-----------------------------------------------------------------------------|
| Board      | `esp32` -> `ESP32C3 Dev Module` | The board to use with Lolin C3 Mini microcontroller<br>⚠️ WARNING: Do not use the `Lolin C3 Mini` board configuration as it has the wrong partition size |
| Port       | `/dev/cu.usbmodem101`       | The port to connect to the development board<br>ℹ️ NOTE: Value could be different, per hosting machine |
| USB CDC On Boot | `Enabled`  | Enables serial communications through USB |
| CPU Frequency    | `160Mhz (Wifi)`        | The CPU frequency of the micontroller |
| Core Debug Level | `None`        | The level for logs output by the core<br>ℹ️ NOTE: Could be set to `Info` or other values for debugging|
| Erase All Flash Before Sketch Upload | `Disabled` | Normally not needed |
| Flash Frequency | `40Mhz` | The frequency of flash<br>⚠️ WARNING: A critical value that if set too high will fail to upload firmwares |
| Flash Mode | `DIO` | Dual I/O flash mode. Default value<br>⚠️ WARNING: A critical value, do not modify |
| Flash size | `4MB (32Mb)`  | Default value |
| JTAG Adapter  | `Disabled` | JTAG debugging is not used |
| Partition Schema | `Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)` | The schema for partitioning the microcontroller storage<br>⚠️ WARNING: A critical and *non-default* value. Without it the firmware would not fit |
| Upload Speed | `115200` | The upload speed in baud. Setting it above might lead to failure to upload firmwares<br>⚠️ WARNING: A critical and *non default* value |


## Required Arduino Libraries

### ESP32-BLE-Keyboard
> The library provides BLE Keyboard support for ESP32 boards such as the Lolin C3 Mini.

1. Install the library from [ESP32-BLE-Keyboard Github](https://github.com/T-vK/ESP32-BLE-Keyboard#installation).

2. Perform manual changes to `/Users/YOUR_USER/Documents/Arduino/libraries/ESP32_BLE_Keyboard/BleKeyboard.cpp`:
1. Line #130 - Change to:
```
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
```

### Switch
> The library provides debounced push button functionality.

Install the library `Switch` by Albert Van Dalen. 

Github: [avdweb_Switch](https://github.com/avdwebLibraries/avdweb_Switch)


# Configuration and settings
## Lolin C3 mini - Board layout

<img src="https://raw.githubusercontent.com/bigman73/rider-kontrol/main/assets/c3_mini_v2.1.0_1_16x16.jpg" alt="Button Layout" width="300" />

<img src="https://raw.githubusercontent.com/bigman73/rider-kontrol/main/assets/c3_mini_v2.1.0_2_16x16.jpg" alt="Button Layout" width="300" />

## Button Layout
<img src="https://raw.githubusercontent.com/bigman73/rider-kontrol/main/assets/RiderKontrolButtons.png" alt="Button Layout" width="400" />

## Button Mapping

| Name | Description | Role | Usage Short | Usage Long |
|------------------------------------|--|--|--|--|
| B1 | Button 1 | `+`    | Zoom In       | Zoom In Continous - repeated until released|
| B2 | Button 2 | `-`    | Zoom Out      | Zoom Out Continuous |
| B3 | Button 3 | `P`     | Play/Pause Music|Next Track|
| B4 | Button 4 | `C`     | Center Map / Toggle follow mode|OTA mode|
| B5 | Button 5 | ⬆️   | Pan Map Up    | Pan Map Up Continous|
| B6 | Button 6 | ➡️   | Pan Map Right | Pan Map Right Continous|
| B7 | Button 7 | ⬇️   | Pan Map Down  | Pan Map Down Continous|
| B8 | Button 8 | ⬅️   | Pan Map Left  | Pan Map Up Continous|

## Pin Layout

| GPIO Pin Number | Description |
|------------|------------------------------------|
| 0          | Button 1 (+) |
| 1          | Button 2 (-) |
| 2          | Button 3 (P) |
| 3          | Button 4 (C) |
| 4          | Button 5 ⬆️ | 
| 5          | Button 6 ➡️ |
| 6          | Button 7 ⬇️                          |
| 8          | Button 8 ⬅️ |
| 7          | Onboard RGB LED |
| 10         | External LED |

## Physical Connections / Soldering ⚡️
| From | To | Description |
|--------------|---------------------------|--|
| B1 negative leg (any [1]) | B2 negative leg (any [1]) | Button negative bus |
| B2 negative leg | B3 negative leg (any [1], [3]) | Button negative bus |
| B3 negative leg [3] | B4 negative leg (any [1]) | Button negative bus |
| B3 negative leg [3] | B8 negative leg (any [1]) | Button negative bus |
| B8 negative leg | B5 negative leg (any [1]) | Button negative bus |
| B5 negative leg | B6 negative leg (any [1]) | Button negative bus |
| B6 negative leg | B7 negative leg (any [1]) | Button negative bus |
| B6 negative leg | C3 GND | Negative bus connection to C3 |
| USB GND (Black wire) [4] | C3 GND PIN | External power source GND (0V) |
| USB Power (Red wire) [4] | C3 Vbus PIN | External power source Vbus (5V) |
| B1 positive leg [2] | GPIO Pin #0 | Button 1 signal |
| B2 positive leg [2] | GPIO Pin #1 | Button 2 signal |
| B3 positive leg [2] | GPIO Pin #2 | Button 3 signal |
| B4 positive leg [2] | GPIO Pin #3 | Button 4 signal |
| B5 positive leg [2] | GPIO Pin #4 | Button 5 signal |
| B6 positive leg [2] | GPIO Pin #5 | Button 6 signal |
| B7 positive leg [2] | GPIO Pin #6 | Button 7 signal |
| B8 positive leg [2] | GPIO Pin #8 | Button 8 signal |
| External LED Negative wire (black) | C3 GND | External LED ground |
| External LED Positive wire (red) | GPIO Pin #10 | External LED signal |

> Notes

[1] The chosen leg will be the negative. Buttons have no polarity, but consistency is important

[2] The button leg, which is not the chosen negative leg in [1]

[3] B3 has 3 connections - from B2, B4 and to B8

[4] The white and green USB cable wires are data lines and are not used; they should be cut


## DMD2 Remote Controller Setup

The default key mappings for Map View, in DMD2, are conflicting with their other key mappings

1. Pair and connect a BT keyboard (such as [Logitech MX Keys S](https://www.logitech.com/en-us/shop/p/mx-keys-s)) to the DMD2 device (e.g., T665), or Android with DMD2 app.
2. In DMD2 App, open the Settings dialog (⚙️ Cog icon, bottom menu)
3. Menu -> `Setup Remote Controller` -> `Map View`
4. Click on menu item Function 5 - `Follow Toggle`; This will be used as `Center` button
5. On the keyboard, press the `0` key (left of backspace). Key code should be set to `7`
6. Similarly:
  * Function 6 - `Online Layer`, on the keyboard, press the `~` key. Key code should show `68`
  * Function 7 - `Zoom In`; Key `+` => Key code: `70`
  * Function 8 - `Zoom Out`; Key `-` => Key code: `69`