## Setup
### ESP32 Board
Install ESP32 board by Espressif, follow this document:
https://randomnerdtutorials.com/getting-started-esp32-c3-super-mini/

Verified: v2.0.17
NOTE: DO NOT upgrade to 3.x, as it has breaking changes

## Required Arduino Libraries

### ESP32-BLE-Keyboard
Install the library from: https://github.com/T-vK/ESP32-BLE-Keyboard#installation 

Manual changes to `/Users/YOUR_USER/Documents/Arduino/libraries/ESP32_BLE_Keyboard/BleKeyboard.cpp`:
1. Line #130 - Change to:
```
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
```

### NimBLE-Arduino

Install the library NimBLE-Arduino by h2zero

## Pin Layout

// TODO: Image of buttons with labels for each button

| Pin Number | Description |
|------------|------------------------------------|
| 0          | Button 1 (+) |
| 1          | Button 2 (-) |
| 2          | Button 3 (P) |
| 3          | Button 4 (C) |
| 4          | Button 5 ⬅️ |
| 5          | Button 6 ➡️ |
| 6          | Button 7 ⬆️ |
| 8          | Button 8 ⬇️ |
| 7          | Onboard RGB LED |
| 10         | External LED |

# Button Mapping

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

