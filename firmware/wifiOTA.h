#pragma once

#include <Arduino.h>

extern bool _otaEnabled;

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

 /**
 * @brief Persists WiFi OTA enabled stated.
 *
 * @param enable     Enabled value to store.
 * @return true if the namespace opened for write and value was saved; false on failure.
 */
bool saveOTAEnabled(const bool& enable);

/**
 * @brief Reads the OTA enable value from NVS (namespace PREFS_NS_OTA).
 *
 * @return Stored OTA enable state, false if the namespace is missing or the value is empty.
 */
bool loadOtaEnabled();