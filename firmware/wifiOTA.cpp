#include "wifiOTA.h"
#include <WiFi.h>
#include <Preferences.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "constants.h"

unsigned int _lastOTAProgress = 0;
bool _otaLEDState = false;
bool _otaServiceStarted = false;
bool _otaEnabled = false;

/**
 * @brief Reads the ArduinoOTA hostname from NVS (namespace PREFS_NS_OTA).
 *
 * @return Stored hostname, or DEFAULT_OTA_HOSTNAME if the namespace is missing or the value is empty.
 */
 static String loadOtaHostname() {
  Preferences prefs;
  if (!prefs.begin(PREFS_NS_OTA, true)) {
    return String(DEFAULT_OTA_HOSTNAME);
  }
  String hostname = prefs.getString(PREFS_KEY_OTA_HOSTNAME, "");
  prefs.end();
  return hostname.isEmpty() ? String(DEFAULT_OTA_HOSTNAME) : hostname;
}

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

bool saveOta(const String& hostname, const String& password) {
  if (hostname.isEmpty()) {
    Serial.println("OTA hostname cannot be empty.");
    return false;
  }
  if (password.isEmpty()) {
    Serial.println("OTA password cannot be empty.");
    return false;
  }
  Preferences prefs;
  if (!prefs.begin(PREFS_NS_OTA, false)) {
    Serial.println("Preferences: could not open OTA namespace for write");
    return false;
  }
  prefs.putString(PREFS_KEY_OTA_HOSTNAME, hostname);
  prefs.putString(PREFS_KEY_OTA_PASSWORD, password);
  prefs.end();
  Serial.println("Saved OTA details to NVS.");
  if (_otaServiceStarted) {
    Serial.println("Reboot for the new OTA details to take effect.");
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

bool saveWifiCredentials(const String& ssid, const String& password) {
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

  ArduinoOTA.setPort(OTA_PORT);

  // Hostname: must be valid mDNS (letters, digits, hyphens only – no spaces).
  String otaHostname = loadOtaHostname();
  ArduinoOTA.setHostname(otaHostname.c_str());
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
  Serial.printf("OTA Ready, mDNS Hostname: %s, IP address: %s\n", otaHostname.c_str(), WiFi.localIP().toString());
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
    Serial.println("connect: no Wifi credentials stored in NVS. Use serial monitor wifi command to set ssid and password");
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

/**
 * @brief Prints @p secret with only the first character visible; remaining characters are '*'.
 *
 * @param secret String to mask; if empty, prints nothing.
 */
 static void printMaskedSerial(const String& secret) {
  if (secret.isEmpty()) {
    return;
  }
  Serial.print(secret.charAt(0));
  for (size_t i = 1; i < secret.length(); i++) {
    Serial.print('*');
  }
}

void printStoredWifiMasked() {
  String ssid;
  String pass;
  if (!loadWifiCredentials(ssid, pass)) {
    Serial.println("NVS: no credentials stored yet.");
    return;
  }
  Serial.print("Stored SSID: ");
  Serial.println(ssid);
  Serial.print("Stored password: ");
  if (pass.isEmpty()) {
    Serial.println("(empty / open network)");
  } else {
    printMaskedSerial(pass);
    Serial.println();
  }

  String otaHostname = loadOtaHostname();
  Serial.printf("OTA Hostname: %s\n", otaHostname);

  String otaPass = loadOtaPassword();
  Serial.print("OTA password (active): ");
  printMaskedSerial(otaPass);
  Serial.println();
}

bool saveOTAEnabled(const bool& enable) {
  Preferences prefs;
  if (!prefs.begin(PREFS_NS_OTA, false)) {
    Serial.printf("Preferences: could not open namespace for write %s\n", PREFS_NS_OTA);
    return false;
  }
  prefs.putBool(PREFS_KEY_WIFI_OTA_ENABLE, enable);
  prefs.end();
  Serial.println("Saved Wifi OTA enable state to NVS.");
  return true;
}

bool loadOtaEnabled() {
  Preferences prefs;
  if (!prefs.begin(PREFS_NS_OTA, true)) {
    return false;
  }
  bool p = prefs.getBool(PREFS_KEY_WIFI_OTA_ENABLE, false);
  prefs.end();
  return p;
}

void setupWifiOTA() {
  _otaEnabled = loadOtaEnabled();
  Serial.printf("OTA Enabled: %d\n", (int) _otaEnabled);

  if (_otaEnabled) {
    Serial.println("Setting up Wifi OTA...");
    String wifiSsid;
    String wifiPassword;
    if (loadWifiCredentials(wifiSsid, wifiPassword)) {
      attemptWifiConnect();
    } else {
      Serial.println("No WiFi credentials persisted in NVS yet - use serial commands.");
    }
  }
}
