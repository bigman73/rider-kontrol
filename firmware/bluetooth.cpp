#include "bluetooth.h"
#include "vars.h"
#include "utils.h"

// Initialize the BLE keyboard
BleKeyboard bleKeyboard(
  BLUETOOTH_DEVICE, BLUETOOTH_MANUFACTURER, 
  BLUETOOTH_BATT_LEVEL_DEFAULT);

  
void setupBluetooth() {
  printSerialMessage("Starting BLE Keyboard");
  bleKeyboard.begin();
}

void handleBLEKeyboardConnection() {
  if (_firstBLE && bleKeyboard.isConnected()) {
    _firstBLE = false;
    printSerialMessage("✅ Connected as a BT Keyboard");
  }

  if (!_firstBLE && !bleKeyboard.isConnected()) {
    _firstBLE = true;
    printSerialMessage("❌ Disconnected as a BT Keyboard");
  }
}

void sendBluetoothKey(const uint8_t* keyCodes) {
  if (!keyCodes) {
    printSerialMessage("Error (sendBluetoothKey): keyCodes is null");
  }

  // Use the buffer overload
  bleKeyboard.write(keyCodes, 1);
}

void sendBluetoothMediaKey(const MediaKeyReport mediaKey) {
  bleKeyboard.write(mediaKey);
}

void sendPanningBluetoothKey(const uint8_t* keyCodes) {
  if (_firstTimePan) {
    printSerialMessage("FirstTimePan");
    _firstTimePan = false;
    sendBluetoothKey(keyCodes);
  }

  sendBluetoothKey(keyCodes);
}

void sendBluetoothString(const String input) {
  bleKeyboard.print(input);
}