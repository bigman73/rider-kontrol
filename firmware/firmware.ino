#include "constants.h"
#include "vars.h"
#include <avdweb_Switch.h>
#include "buttonDefinition.h"
#include "diagnostics.h"
#include "commands.h"
#include "utils.h"
#include "bluetooth.h"
#include "ledLogic.h"
#include "wifiOTA.h"

// --------------------------------------------------------------------------

/**
  Setup the buttons, assign their GPIO pin, button kind and actions
  NOTE: GPIO_7 is not used, as it is the onboard LED
 */
void setupButtons() {
  // B1 - See image assets/RiderKontrolButtons.png
  _buttons[0] = new ButtonDefinition(GPIO_0, "B1", ButtonKind::Continous, 
    RiderKontrolAction::ZoomIn, 
    DMD2_KEYCODE_ZOOM_IN
  );
  // B2
  _buttons[1] = new ButtonDefinition(GPIO_1, "B2", ButtonKind::Continous, 
    RiderKontrolAction::ZoomOut, 
    DMD2_KEYCODE_ZOOM_OUT
  );
  // B3
  _buttons[2] = new ButtonDefinition(GPIO_2, "B3", ButtonKind::ShortLong, 
    RiderKontrolAction::PlayPauseMedia, 
    DMD2_KEYCODE_PLAY_PAUSE,
    true,
    RiderKontrolAction::NextTrackMedia,
    DMD2_KEYCODE_NEXT_TRACK,
    true
  );
  // B4
  _buttons[3] = new ButtonDefinition(GPIO_3, "B4", ButtonKind::ShortLong, 
    RiderKontrolAction::ToggleFollow, 
    DMD2_KEYCODE_CENTER,
    false,
    RiderKontrolAction::EnableOTA
  );
  // B5
  _buttons[4] = new ButtonDefinition(GPIO_4, "B5", ButtonKind::Continous, 
    RiderKontrolAction::PanUp,
    DMD2_KEYCODE_UP_ARROW
  );
  // B6
  _buttons[5] = new ButtonDefinition(GPIO_5, "B6", ButtonKind::Continous, 
    RiderKontrolAction::PanRight,
    DMD2_KEYCODE_RIGHT_ARROW
  );
  // B7
  _buttons[6] = new ButtonDefinition(GPIO_6, "B7", ButtonKind::Continous, 
    RiderKontrolAction::PanDown,
    DMD2_KEYCODE_DOWN_ARROW
  );
  // B8
  _buttons[7] = new ButtonDefinition(GPIO_8, "B8", ButtonKind::Continous, 
    RiderKontrolAction::PanLeft,
    DMD2_KEYCODE_LEFT_ARROW
  );
}

/**
 * @brief Sets the program state to Diagnostics Mode and initializes mode-specific variables.
 *
 * This function changes the global program state to diagnostics, resets the blink and
 * enter time trackers, and prints a message indicating the mode change to serial.
 */
void setDiagnosticsMode() {
  _programState = ProgramState::Diag;
  _lastBlinkTime = 0;
  _diagEnterTime = millis();
  printSerialMessage("*** Program State: Diagnostics Mode");
}

/**
 * @brief Sets the program state to Normal Mode and initializes mode-specific variables.
 *
 * This function changes the global program state to Normal, resets the blink and
 * enter time trackers, and prints a message indicating the mode change to serial.
 */
void setNormalMode() {
  _programState = ProgramState::Normal;
  _lastBlinkTime = 0;
  printSerialMessage("*** Program State: Normal Mode");
}

/**
  Handles the commands coming on serial input
 */
void handleSerialInput() {
  if (Serial.available() > 0) {
    // Read input until a newline character is received
    String input = Serial.readStringUntil('\n');
    input.trim(); // Clean up spaces or carriage returns

    String cmd;
    String rest;
    String arg1;
    String arg2;
    parseSerialCommand(input, cmd, rest, arg1, arg2);
    if (cmd.length() == 0) {
      return;
    }

    if (cmd == COMMAND_MODE_DIAG) {
      setDiagnosticsMode();
    } else if (cmd == COMMAND_MODE_NORMAL) {
      setNormalMode();
    } else if (cmd == COMMAND_MODE_STATE) {
      printFormattedSerialMessage("Current program state: %u\n", _programState);
    } else if (_programState == ProgramState::Diag) {
      processDiagMenu(cmd, arg1, arg2);
    } else {
      printFormattedSerialMessage("Unknown command: %s\n", input);
    }
  }
}

/**
  Handles logic for diagnostics program state
 */
void handleDiagStateLogic() {
  unsigned long now = millis();
  
  if (now - _diagEnterTime >= DIAG_MAX_TIME_MSEC) {
    // Automatically exit diagnostics mode
    printDebugMessage("Auto exiting diagnostics mode, back to normal mode");
    _lastBlinkTime = 0;
    _diagEnterTime = 0;
    _programState = ProgramState::Normal;
  }
}

/**
  Handle the program logic, for all states
 */
void handleProgramStateLogic() {
  if (_programState == ProgramState::Diag) {
    handleDiagStateLogic();
  }
}

/**
  Single button on press event handler
 */
void handleButtonPress(ButtonDefinition* buttonDef) {
  printFormattedSerialMessage("=> handleButtonPress: %s,", buttonDef->name);
  printFormattedSerialMessage("kind: %u, action1: %u, action2: %u\n", 
    buttonDef->kind, 
    buttonDef->action1, 
    buttonDef->action2);

  _lastButtonPressed = millis();

  if (buttonDef->isCodes1Media) {
    sendBluetoothMediaKey(buttonDef->keyCodes1);
  } else {
    sendPanningBluetoothKey(buttonDef->keyCodes1);
  }
}

/**
  Handles logic for continous button press/release
 */
void handleContinousButton(ButtonDefinition* buttonDef) {
  int now = millis();
  Switch* button = &buttonDef->button;

// Note: `pushed` is a one time transition on the trigger from unpushed to pushed,
  //  unlike `on` which is a current state of button
  if (button->pushed()) {
    _lastButtonOnCheckTime = now;
    _firstButtonPush = true;
    _isButtonPressed = true;

    handleButtonPress(buttonDef);
    return;
  } else if (button->on() &&
              _firstButtonPush &&
              (now - _lastButtonOnCheckTime > REPEAT_PUSH_INTERVAL1_MSEC)) {
    // The button long press, interval 1
    _lastButtonOnCheckTime = now;
    _firstButtonPush = false;

    handleButtonPress(buttonDef);
    return;
  } else if (button->on() &&
              !_firstButtonPush &&
              (now - _lastButtonOnCheckTime > REPEAT_PUSH_INTERVAL2_MSEC)) {
    // The button long press, interval 2
    _lastButtonOnCheckTime = now;

    handleButtonPress(buttonDef);
    return;
  }

  if (button->released()) {
    printFormattedSerialMessage("Continous Button %s released\n", buttonDef->name);

    _isButtonPressed = false;
    _firstButtonPush = false;

    return;
  }
}

/**
  Handles logic for short-long button press/release
 */
void handleShortLongButton(ButtonDefinition* buttonDef) {
  int now = millis();
  Switch* button = &buttonDef->button;

  if (button->pushed()) {
    _lastButtonPressed = now;
    _isButtonPressed = true;
  }

  if (button->released() && !button->on() && 
      (now - _lastButtonPressed < SHORT_BUTTON_DURATION_MSEC)) {
    printFormattedSerialMessage("=> handleShortLongButton SHORT: %s,", buttonDef->name);
    printFormattedSerialMessage("kind: %u, action1: %u, action2: %u\n", 
      buttonDef->kind, 
      buttonDef->action1, 
      buttonDef->action2
    );
      
    _lastButtonPressed = 0;
    _isButtonPressed = false;

    if (buttonDef->isCodes1Media) {
      sendBluetoothMediaKey(buttonDef->keyCodes1);
    } else {
      // Special case: ToggleFollow disables the panning mode
      if (buttonDef->action1 == RiderKontrolAction::ToggleFollow) {
        printSerialMessage("FirstTimePan: set to true");
        _firstTimePan = true;
      }

      sendBluetoothKey(buttonDef->keyCodes1);
    }
  } else if (button->released() && !button->on()) {
    printFormattedSerialMessage("=> handleShortLongButton LONG: %s,", buttonDef->name);
    printFormattedSerialMessage("kind: %u, action1: %u, action2: %u\n", 
      buttonDef->kind, 
      buttonDef->action1, 
      buttonDef->action2
    );

    _isButtonPressed = false;

    if (buttonDef->isCodes1Media) {
      sendBluetoothMediaKey(buttonDef->keyCodes2);
    } else if (buttonDef->action2 == RiderKontrolAction::EnableOTA) {
      if ((now - _lastButtonPressed > OTA_BUTTON_DURATION_MSEC)) {
        // OTA Toggle is a special case, requires a very long press
        toggleOTAEnabledState();
      } else {
        Serial.println("OTA Toggle requires a very long press. Try again.");
      }
    } else {
      sendBluetoothKey(buttonDef->keyCodes2);
    }

    _lastButtonPressed = 0;
  }
}

/**
  Handles buttons press/release logic
 */
void handleButtons() {
  // Poll all button state
  for (int buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++) {
    ButtonDefinition* buttonDef = _buttons[buttonIndex];
    if (buttonDef != nullptr) {
      buttonDef->button.poll();
    }
  }

  for (int buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++) {
    ButtonDefinition* buttonDef = _buttons[buttonIndex];
    if (buttonDef == nullptr) {
      continue;
    }

    if (buttonDef->kind == ButtonKind::Continous) {
      handleContinousButton(buttonDef);
    } else {
      handleShortLongButton(buttonDef);
    }
  }
}



// ===================================================================================

/**
 * The setup function runs once when you press reset or power the board.
 */
 void setup() {
  delay(INITIAL_STABILIZE_INTERVAL_MSEC); // Let the chip stabilize

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  // initialize digital pin EXTERNAL_LED_PIN as an output.
  pinMode(EXTERNAL_LED_PIN, OUTPUT);

  neopixelWrite(ONBOARD_LED_PIN, LED_OFF_RED, LED_OFF_GREEN, LED_OFF_BLUE);

  Serial.begin(SERIAL_BAUD_RATE);
  delay(SERIAL_TIMEOUT_MSEC);  // Wait for serial port to connect and initialize

  printFirmwareVersion();

  // TODO: Make a diagnostics command
  printPartitionInfo();

  setupButtons();

  setupBluetooth();

  setupWifiOTA();
}

/**
 * The loop function runs over and over again forever.
 */
void loop() {
  controlLEDs();

  handleSerialInput();

  handleProgramStateLogic();

  handleBLEKeyboardConnection();

  handleButtons();
}
