#pragma once
#include <Arduino.h>

/**
 * Print a message line to the serial port.
 * @param message The message to print.
 */
void printSerialMessage(String message);

/**
 * Print a formatted message line to the serial port.

 * @param message The message template to print.
 * @param args Arguments
 */
template<typename... Args>
void printFormattedSerialMessage(const String& message, Args... args) {
  Serial.printf(message.c_str(), args...);  // Print the message line.
}

/**
 * Print a debug message line to the serial port.
 * @param message The message to print.
 */
void printDebugMessage(String message);

/**
 * Print the firmware version message to the serial port.
 */
void printFirmwareVersion();