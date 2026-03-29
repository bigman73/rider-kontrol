#pragma once
#include <Arduino.h>
#include <BleKeyboard.h>

/**
  Setup bluetooth
 */
void setupBluetooth();

/**
  Handles the BLE Keyboard connection logic
 */
void handleBLEKeyboardConnection();

/**
 Sends a Bluetooth media key press

 @param mediaKey Media key codes to send over Bluetooth
*/
void sendBluetoothMediaKey(const MediaKeyReport mediaKey);

/**
 * @brief Sends a Bluetooth key press.
 *
 * This function sends the provided key codes as a key press over Bluetooth.
 *
 * @param keyCodes Pointer to the key codes array to send over Bluetooth.
 */
void sendBluetoothKey(const uint8_t* keyCodes);

/**
  Sends a Bluetooth key press that is related to map panning
  @param keyCodes Key codes to send over Bluetooth
 */
void sendPanningBluetoothKey(const uint8_t* keyCodes);

/**
 Sends a Bluetooth string

 @param input Input string to send over Bluetooth
*/
void sendBluetoothString(String input);
