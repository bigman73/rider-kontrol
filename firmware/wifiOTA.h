#pragma once

#include <Arduino.h>

void setupWifiOTA();

/**
 * @brief Prints stored WiFi SSID and masked passwords (WiFi and active OTA) to Serial.
 */
void printStoredWifiMasked();

/**
 * @brief Writes the ArduinoOTA details to NVS.
 *
 * @param hostname Non-empty OTA hostname to store.
 * @param password Non-empty OTA password to store.
 * @return true on success; false if arguments are empty or NVS write fails.
 * @note If OTA is already running, a reboot is required for the new OTA details to apply.
 */
bool saveOta(const String& hostname, const String& password);

/**
 * @brief Persists WiFi SSID and password to NVS under PREFS_NS_WIFI.
 *
 * @param ssid     Network name to store.
 * @param password Network password (may be empty for open networks).
 * @return true if the namespace opened for write and strings were saved; false on failure.
 */
 bool saveWifiCredentials(const String& ssid, const String& password);