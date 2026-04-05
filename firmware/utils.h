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

/**
 * @brief Parses one serial line into a command token and argument fragments.
 *
 * @param line      Raw input line (typically newline-stripped by caller).
 * @param outCmd    First word, lowercased (command name).
 * @param outRest   Full text after the first space (trimmed); use for passwords with spaces.
 * @param outFirst  First word of outRest.
 * @param outSecond Remainder of outRest after the first word (trimmed).
 */
 void parseSerialCommand(const String& line, String& outCmd, String& outRest, String& outFirst,
  String& outSecond);