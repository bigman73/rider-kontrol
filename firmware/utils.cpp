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

void parseSerialCommand(const String& line, String& outCmd, String& outRest, String& outFirst,
  String& outSecond) {
  outCmd = "";
  outRest = "";
  outFirst = "";
  outSecond = "";
  String s = line;
  s.trim();
  if (s.length() == 0) {
    return;
  }
  int sp = s.indexOf(' ');
  if (sp < 0) {
    outCmd = s;
    outCmd.toLowerCase();
    return;
  }
  outCmd = s.substring(0, sp);
  outCmd.toLowerCase();
  outRest = s.substring(sp + 1);
  outRest.trim();
  int sp2 = outRest.indexOf(' ');
  if (sp2 < 0) {
    outFirst = outRest;
  } else {
    outFirst = outRest.substring(0, sp2);
    outSecond = outRest.substring(sp2 + 1);
    outSecond.trim();
  }
}