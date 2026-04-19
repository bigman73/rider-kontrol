#include "commands.h"
#include "constants.h"
#include "vars.h"
#include "utils.h"
#include "bluetooth.h"
#include "wifiOTA.h"

void performControllerReboot() {
  Serial.println("Rebooting...");
  delay(100);
  ESP.restart();
}

void toggleOTAEnabledState() {
  if (!_otaEnabled) {
    Serial.println("🟢 Enable OTA");
    _lastBlinkTime = 0;
    _otaEnabled = true;
    saveOTAEnabled(true);
    performControllerReboot();
  } else {
    Serial.println("🔴 Disable OTA");
    _otaEnabled = false;
    saveOTAEnabled(false);
    performControllerReboot();
  }
}

void processDiagMenu(String cmd, String arg1, String arg2) {
  if (cmd == COMMAND_DIAG_HELLO) {
    printFormattedSerialMessage("Welcome to %s!\n", FIRMWARE_NAME);
    printSerialMessage("Printing [Hello world] to BT keyboard");
    sendBluetoothString("Hello world");
  } else if (cmd == COMMAND_DIAG_UP) {
    printSerialMessage("Command: Up arrow");
    sendPanningBluetoothKey(DMD2_KEYCODE_UP_ARROW);
  } else if (cmd == COMMAND_DIAG_DOWN) {
    printSerialMessage("Command: Down arrow");
    sendPanningBluetoothKey(DMD2_KEYCODE_DOWN_ARROW);
  } else if (cmd == COMMAND_DIAG_LEFT) {
    printSerialMessage("Command: Left arrow");
    sendPanningBluetoothKey(DMD2_KEYCODE_LEFT_ARROW);
  } else if (cmd == COMMAND_DIAG_RIGHT) {
    printSerialMessage("Command: Right arrow");
    sendPanningBluetoothKey(DMD2_KEYCODE_RIGHT_ARROW);
  } else if (cmd == COMMAND_DIAG_ZOOM_IN) {
    printSerialMessage("Command: Zoom In");
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_IN);
  } else if (cmd == COMMAND_DIAG_ZOOM_IN_X) {
    printSerialMessage("Command: Zoom In x5");
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_IN);
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_IN);
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_IN);
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_IN);
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_IN);

  } else if (cmd == COMMAND_DIAG_ZOOM_OUT) {
    printSerialMessage("Command: Zoom Out");
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_OUT);
  } else if (cmd == COMMAND_DIAG_ZOOM_OUT_X) {
    printSerialMessage("Command: Zoom Out x5");
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_OUT);
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_OUT);
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_OUT);
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_OUT);
    sendPanningBluetoothKey(DMD2_KEYCODE_ZOOM_OUT);
  } else if (cmd == COMMAND_DIAG_CENTER) {
    printSerialMessage("Command: Center (Toggle follow)");
    sendBluetoothKey(DMD2_KEYCODE_CENTER);
    _firstTimePan = true;
  } else if (cmd == COMMAND_DIAG_SAT_LAYER) {
    printSerialMessage("Command: Toogle satelite layer");
    sendBluetoothKey(DMD2_KEYCODE_ONLINE_LAYER);
  } else if (cmd == COMMAND_DIAG_PLAY_MEDIA) {
    printSerialMessage("Command: Play/Pause Media");
    sendBluetoothMediaKey(DMD2_KEYCODE_PLAY_PAUSE);
  } else if (cmd == COMMAND_DIAG_PREV_MEDIA) {
    printSerialMessage("Command: Prev Track");
    sendBluetoothMediaKey(DMD2_KEYCODE_PREV_TRACK);
  } else if (cmd == COMMAND_DIAG_NEXT_MEDIA) {
    printSerialMessage("Command: Next Track");
    sendBluetoothMediaKey(DMD2_KEYCODE_NEXT_TRACK);
  } else if (cmd == COMMAND_DIAG_MUTE_MEDIA) {
    printSerialMessage("Command: Mute");
    sendBluetoothMediaKey(DMD2_KEYCODE_MUTE);
  } else if (cmd == COMMAND_DIAG_VERSION) {
    printFirmwareVersion();
  } else if (cmd == COMMAND_DIAG_CREDS) {
    printStoredWifiMasked();
  } else if (cmd == COMMAND_DIAG_OTASET) {
    if (arg1.isEmpty() || arg2.isEmpty()) {
      Serial.println("Usage: otaset <hostname> <password>");
      return;
    }
    saveOta(arg1, arg2);
  } else if (cmd == COMMAND_ENABLE_OTA) {
    toggleOTAEnabledState();
  } else if (cmd == COMMAND_DIAG_SETWIFI) {
      if (arg1.isEmpty()) {
        Serial.println("Usage: wifi <ssid> <password>");
        return;
      }

      if (saveWifiCredentials(arg1, arg2)) {
        Serial.print("Wifi credentials updated. SSID set to: ");
        Serial.print(arg1);
        Serial.println(", password updated.");
      }
  } else if (cmd == COMMAND_DIAG_REBOOT) {
    performControllerReboot();
  } else {
    printFormattedSerialMessage("Unknown diagnostics command: %s\n", cmd);
  }
}
