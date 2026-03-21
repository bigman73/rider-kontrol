# Development Environment Setup
## ESP32 Board
Install ESP32 board by Espressif, follow this [Getting Started document](
https://randomnerdtutorials.com/getting-started-esp32-c3-super-mini/)

Verified version: v2.0.17

> NOTE: DO NOT upgrade to 3.x, as it has breaking changes

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
## Pin Layout

// TODO: Image of buttons with labels for each button

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

## Soldering / Physical Connections
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

## Button Mapping

| Description | Usage Short | Usage Long |
|------------------------------------|--|--|
| Button 1 (+)      |Zoom In - Key [+]|Zoom In 10x - Repeated Key [+] 10 times|
| Button 2 (-) |Zoom Out - Key [-] |Zoom In 10x - Repeated Key [-] 10 times|
| Button 3 (P) |Play/Pause Music - Key [TODO]||
| Button 4 (C) |Center Map - Key [0]|Buttons mapping change|
| Button 5 ⬅️   | Scroll map left - Key [TODO]||
| Button 6 ➡️   | Scroll map right - Key [TODO] ||
| Button 7 ⬆️   | Scroll map up - Key [TODO]||
| Button 8 ⬇️   | Scroll map down - Key [TODO]||


## DMD2 Remote Controller Setup

The default key mappings for Map View, in DMD2, are conflicting with their other key mappings

1. Pair and connect a BT keyboard (such as Logitech MX) to the DMD2 device (or Android with DMD2 ) app.
2. Open the Setting menu (Cog icon, bottom menu)
3. Menu -> Setup Remote Controller -> Map View
4. Click on menu item Function 5 - `Follow Toggle` - This will be used as `Center` button
5. On the keyboard, press the '0' key (left of backspace). Key code should be set to 7
6. Similarly, menu item Function 6 - `Online Layer`, on the keyboard, press the '~' key. Key code => 68
7. Function 7 | `Zoom In` - Key '+' => Key Code: 70
8. Function 8 | `Zoom Out` - Key '-' => Key Code: 69