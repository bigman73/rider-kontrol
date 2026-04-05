#include "ledLogic.h"
#include <Arduino.h>
#include "constants.h"
#include "vars.h"
#include "utils.h"
#include "wifiOTA.h"

/**
  Controls the onboard LED
 */
 void controlOnboardLED() {
  unsigned long now = millis();

  if (_otaEnabled) {
    // Check if the LED blink interval has elapsed.
    if (now - _lastBlinkTime >= LED_OTA_BLINK_INTERVAL_MSEC) {
      // Toggle the led state.
      _ledOn = !_ledOn;
      // Set the onboard led colors for OTA.
      neopixelWrite(ONBOARD_LED_PIN, 
        _ledOn ? LED_ON_OTA_RED : LED_OFF_OTA_RED, 
        _ledOn ? LED_ON_OTA_GREEN : LED_OFF_OTA_GREEN, 
        _ledOn ? LED_ON_OTA_BLUE : LED_OFF_OTA_BLUE);

      // Set the external led state.
      _lastBlinkTime = now;
    }
  }

  if (_programState == ProgramState::Normal) {
    // Check if the LED blink interval has elapsed.
    if (now - _lastBlinkTime >= LED_BLINK_INTERVAL_MSEC) {
      // Toggle the led state.
      _ledOn = !_ledOn;
      // Set the onboard led colors.
      neopixelWrite(ONBOARD_LED_PIN, 
        _ledOn ? LED_ON_RED : LED_OFF_RED, 
        _ledOn ? LED_ON_GREEN : LED_OFF_GREEN, 
        _ledOn ? LED_ON_BLUE : LED_OFF_BLUE);

      // Set the external led state.
      // TODO: Just for testing, should be removed later. The external LED will have its own code to handle the blink or other application modes.
      //digitalWrite(EXTERNAL_LED_PIN, _ledOn ? HIGH : LOW);
      // Update the last blink time.
      _lastBlinkTime = now;

      // Print the loop count if the led is on.
      if (_ledOn) {
        printDebugMessage("Blink " + String(_loopCount));
        _loopCount++;
      }
    }
  } else if (_programState == ProgramState::Diag) {
     // Check if the LED diagnostics blink interval has elapsed.
     if (now - _lastBlinkTime >= LED_DIAG_BLINK_INTERVAL_MSEC) {
      // Toggle the led state.
      _ledOn = !_ledOn;
      // Set the onboard led colors.
      neopixelWrite(ONBOARD_LED_PIN, 
        _ledOn ? LED_DIAG_ON_RED : LED_DIAG_OFF_RED, 
        _ledOn ? LED_DIAG_ON_GREEN : LED_DIAG_OFF_GREEN, 
        _ledOn ? LED_DIAG_ON_BLUE : LED_DIAG_OFF_BLUE);

      // Update the last blink time.
      _lastBlinkTime = now;
    }
  }
}

/**
  Blink heartbeat of external LED
 */
void heartbeatExternalLED() {
  unsigned long now = millis();

  if (now - _lastButtonPressed < EXT_LED_HEARTBEAT_COOLDOWN_MSEC) {
    // Only heartbeat after some cooling after button pressed
    return;
  }

  unsigned long currentTimeWindow = now % EXT_LED_HEARTBEAT_CADENCE_MSEC;
  if (!_externalLedHeartbeat && currentTimeWindow >=0 && currentTimeWindow <= EXT_LED_HEARTBEAT_DURATION_MSEC) {
    // Blink the external LED for some MSEC every MSEC (e.g., 100 msec then rest 4900 msec)
    _externalLedHeartbeat = true;
    digitalWrite(EXTERNAL_LED_PIN, HIGH);
  } else if (_externalLedHeartbeat && currentTimeWindow > EXT_LED_HEARTBEAT_DURATION_MSEC) {
    _externalLedHeartbeat = false;
    digitalWrite(EXTERNAL_LED_PIN, LOW);
  }
}

/**
  Blink OTA on external LED
 */
 void otaExternalLED() {
  unsigned long now = millis();

  unsigned long currentTimeWindow = now % EXT_LED_OTA_CADENCE_MSEC;
  if (currentTimeWindow >=0 && currentTimeWindow <= EXT_LED_OTA_DURATION_MSEC) {
    // Blink the external LED for some MSEC every MSEC (e.g., 150 msec then rest 150 msec)
    _externalLedHeartbeat = true;
    digitalWrite(EXTERNAL_LED_PIN, HIGH);
  } else if (currentTimeWindow > EXT_LED_OTA_DURATION_MSEC) {
    _externalLedHeartbeat = false;
    digitalWrite(EXTERNAL_LED_PIN, LOW);
  }
}

/*
  Controls the external LED
*/
void controlExternalLED() {
  // When button is pushed, override and disable all other external LED logic
  if (_isButtonPressed) {
      // Detect a change, from not pressed to pressed - need to turn on the LED
    if (!_externalLEDButtonOn) {
      digitalWrite(EXTERNAL_LED_PIN, HIGH);
      _externalLEDButtonOn = true;
    }

    return;
  }

  // Detect a change, from pressed to not pressed - need to turn off the LED
  if (_externalLEDButtonOn && !_isButtonPressed) {
    _externalLEDButtonOn = false;
    digitalWrite(EXTERNAL_LED_PIN, LOW);
  }

  if (_otaEnabled) {
    otaExternalLED();
    return;
  }

  if (_programState == ProgramState::Normal) {
    heartbeatExternalLED();
  } else if (_programState == ProgramState::Diag) {
    digitalWrite(EXTERNAL_LED_PIN, _ledOn ? HIGH : LOW);
  }
}

/**
 * Control the LEDs.
 */
void controlLEDs() {
  controlOnboardLED();
  controlExternalLED();
}