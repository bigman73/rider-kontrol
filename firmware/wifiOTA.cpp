#include "wifiOTA.h"
#include <arduino.h>
#include <WiFi.h>
#include <Preferences.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "constants.h"

int _lastOTAProgress = 0;
bool _otaLEDState = false;
bool _otaServiceStarted = false;

/**
 * @brief Reads the ArduinoOTA password from NVS (namespace PREFS_NS_OTA).
 *
 * @return Stored password, or DEFAULT_OTA_PASSWORD if the namespace is missing or the value is empty.
 */
 static String loadOtaPassword() {
  Preferences prefs;
  if (!prefs.begin(PREFS_NS_OTA, true)) {
    return String(DEFAULT_OTA_PASSWORD);
  }
  String p = prefs.getString(PREFS_KEY_OTA_PASSWORD, "");
  prefs.end();
  return p.isEmpty() ? String(DEFAULT_OTA_PASSWORD) : p;
}

/**
 * @brief Writes the ArduinoOTA password to NVS.
 *
 * @param password Non-empty OTA password to store.
 * @return true on success; false if password is empty or NVS write fails.
 * @note If OTA is already running, a reboot is required for the new password to apply.
 */
 static bool saveOtaPassword(const String& password) {
  if (password.isEmpty()) {
    Serial.println("OTA password cannot be empty.");
    return false;
  }
  Preferences prefs;
  if (!prefs.begin(PREFS_NS_OTA, false)) {
    Serial.println("Preferences: could not open OTA namespace for write");
    return false;
  }
  prefs.putString(PREFS_KEY_OTA_PASSWORD, password);
  prefs.end();
  Serial.println("Saved OTA password to NVS.");
  if (_otaServiceStarted) {
    Serial.println("Reboot for the new OTA password to take effect.");
  }
  return true;
}

/**
 * @brief Loads WiFi credentials (SSID and password) from NVS (Non-Volatile Storage) preferences.
 *
 * This function opens the NVS namespace defined by PREFS_NS_WIFI in read-only mode and attempts
 * to retrieve the SSID and password strings associated with PREFS_KEY_SSID and PREFS_KEY_PASSWORD.
 *
 * @param[out] outSsid      String to receive the loaded WiFi SSID.
 * @param[out] outPassword  String to receive the loaded WiFi password.
 * @return true if both SSID (and password, if required) are found; false otherwise.
 *
 * @details
 * - If the namespace or the SSID key is missing, the function prints an error message and returns false.
 * - The password may be empty if using an open network.
 * - Callers must check the return value and handle cases where credentials are missing.
 */
 static bool loadWifiCredentials(String& outSsid, String& outPassword) {
  Preferences prefs;
  if (!prefs.begin(PREFS_NS_WIFI, true)) {
    Serial.println("Preferences: could not open namespace \"" + String(PREFS_NS_WIFI) + "\"");
    return false;
  }
  outSsid = prefs.getString(PREFS_KEY_SSID, "");
  outPassword = prefs.getString(PREFS_KEY_PASSWORD, "");
  prefs.end();
  return !outSsid.isEmpty();
}

/**
 * @brief Persists WiFi SSID and password to NVS under PREFS_NS_WIFI.
 *
 * @param ssid     Network name to store.
 * @param password Network password (may be empty for open networks).
 * @return true if the namespace opened for write and strings were saved; false on failure.
 */
 static bool saveWifiCredentials(const String& ssid, const String& password) {
  Preferences prefs;
  if (!prefs.begin(PREFS_NS_WIFI, false)) {
    Serial.println("Preferences: could not open namespace for write");
    return false;
  }
  prefs.putString(PREFS_KEY_SSID, ssid);
  prefs.putString(PREFS_KEY_PASSWORD, password);
  prefs.end();
  Serial.println("Saved WiFi credentials to NVS.");
  return true;
}

/**
 * @brief Configures mDNS, ArduinoOTA (password from NVS), and starts the OTA server.
 *
 * No-op if already started. Sets _otaServiceStarted when begin() succeeds.
 * @note Call once after WiFi is connected; hostname must be valid for mDNS.
 */
 static void startOtaService() {
  if (_otaServiceStarted) {
    return;
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname: must be valid mDNS (letters, digits, hyphens only – no spaces).
  const char* otaHostname = "rider-kontrol";
  ArduinoOTA.setHostname(otaHostname);
  if (!MDNS.begin(otaHostname)) {
    Serial.println("mDNS failed to start");
  }

  String otaPass = loadOtaPassword();
  ArduinoOTA.setPassword(otaPass.c_str());

  ArduinoOTA
      .onStart([]() {
        neopixelWrite(ONBOARD_LED_PIN, 30, 0, 30);
        _otaLEDState = true;
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else
          type = "filesystem";
        Serial.println("OTA: Start updating " + type);
        _lastOTAProgress = 0;
      })
      .onEnd([]() {
        Serial.println("\nEnd OTA");
        neopixelWrite(ONBOARD_LED_PIN, 0, 30, 30);
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
        if (progress - _lastOTAProgress > 100) {
          _lastOTAProgress = progress;
          _otaLEDState = !_otaLEDState;
          if (_otaLEDState) {
            neopixelWrite(ONBOARD_LED_PIN, 30, 0, 30);
          } else {
            neopixelWrite(ONBOARD_LED_PIN, 5, 0, 5);
          }
        }
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        neopixelWrite(ONBOARD_LED_PIN, 30, 0, 0);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
      });

  ArduinoOTA.begin();
  _otaServiceStarted = true;
  Serial.printf("OTA Ready. Version: %s\n", FIRMWARE_VERSION);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * @brief Connects to WiFi using credentials from NVS and starts OTA on success.
 *
 * @return true if connected within WIFI_CONNECT_TIMEOUT_MS and OTA was started; false otherwise.
 */
 static bool attemptWifiConnect() {
  String wifiSsid;
  String wifiPassword;
  if (!loadWifiCredentials(wifiSsid, wifiPassword)) {
    Serial.println("connect: no Wifi credentials stored in NVS. Use serial monitor to set ssid and password");
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(100);
  WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());

  uint32_t start = millis();
  Serial.printf("Connecting to Wifi (SSID=%s): ", wifiSsid.c_str());
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < WIFI_CONNECT_TIMEOUT_MS) {
    delay(200);
    Serial.print('.');
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi: connection failed (timeout). Check SSID/password.");
    return false;
  }

  Serial.print("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());
  startOtaService();
  return true;
}


void setupWifiOTA() {
  Serial.println("Setting up Wifi OTA...");

  String wifiSsid;
  String wifiPassword;
  if (loadWifiCredentials(wifiSsid, wifiPassword)) {
    attemptWifiConnect();
  } else {
    Serial.println("No WiFi credentials persisted in NVS yet - use serial commands.");
  }
}