#include "utils.h"
#include "constants.h"

void printSerialMessage(String message) {
  Serial.println(message);  // Print the message line.
}

void printDebugMessage(String message) {
  if (SERIAL_DEBUG) {
    printSerialMessage("DEBUG: " + message);
  }
}

void printFirmwareVersion() {
  printFormattedSerialMessage("--= Firmware: %s v%s =--\n", FIRMWARE_NAME, FIRMWARE_VERSION);
}
