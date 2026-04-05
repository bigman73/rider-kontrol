#pragma once
#include <Arduino.h>



/**
  Process the diagnostics menu commands

  * @param cmd The command.
 */
 void processDiagMenu(String cmd, String arg1, String arg2);
 
/**
 * @brief Reboots the controller by printing a message, delaying, and calling ESP.restart().
 *
 * This function will print a "Rebooting..." message to serial, wait for 100 milliseconds,
 * and then restart the ESP device using ESP.restart().
 * 
 * @note This function will not return, as ESP.restart() reboots the microcontroller.
 */
void performControllerReboot();