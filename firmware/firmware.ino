#include "constants.h"
#include "vars.h"
#include "buttonDefinition.h"
#include <BleKeyboard.h>
#include <avdweb_Switch.h>

// Initialize the BLE keyboard
BleKeyboard bleKeyboard(
  BLUETOOTH_DEVICE, BLUETOOTH_MANUFACTURER, 
  BLUETOOTH_BATT_LEVEL_DEFAULT);

// TODO: Move to utility module
/**
 * Print the initialized message to the serial port.
 */
void printInitializedMessage() {
  Serial.print(FIRMWARE_NAME);
  Serial.print(" v");
  Serial.print(FIRMWARE_VERSION);
  Serial.println(" initialized");
}

// TODO: Move to utility module
/**
 * Print a message line to the serial port.
 * @param message The message to print.
 */
 void printSerialMessage(String message) {
  Serial.println(message);  // Print the message line.
}

/**
 * Print a debug message line to the serial port.
 * @param message The message to print.
 */
void printDebugMessage(String message) {
  printSerialMessage("DEBUG: " + message);
}

/**
  Controls the onboard LED
 */
void controlOnboardLED() {
  unsigned long now = millis();

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
  }

  if (_programState == ProgramState::Diag) {
     // Check if the LED diagnostics blink interval has elapsed.
     if (now - _lastBlinkTime >= LED_DIAG_BLINK_INTERVAL_MSEC) {
      // Toggle the led state.
      _ledOn = !_ledOn;
      // Set the onboard led colors.
      neopixelWrite(ONBOARD_LED_PIN, 
        _ledOn ? LED_DIAG_ON_RED : LED_DIAG_OFF_RED, 
        _ledOn ? LED_DIAG_ON_GREEN : LED_DIAG_OFF_GREEN, 
        _ledOn ? LED_DIAG_ON_BLUE : LED_DIAG_OFF_BLUE);

      // Set the external led state.
      // TODO: Just for testing, should be removed later. The external LED will have its own code to handle the blink or other application modes.
      //digitalWrite(EXTERNAL_LED_PIN, _ledOn ? HIGH : LOW);
      // Update the last blink time.
      _lastBlinkTime = now;
    }
  }
}

/*
  Controls the external LED
*/
void controlExternalLED() {
  // When button is pushed, override all other external LED logic
  if (_isButtonPressed) {
    digitalWrite(EXTERNAL_LED_PIN, HIGH);

    return;
  }

  unsigned long now = millis();

  if (_programState == ProgramState::Normal) {
    // TODO: Should blink the external LED for some MSEC every MSEC (e.g., 100 msec then rest 1900 msec)
  }

  if (_programState == ProgramState::Diag) {
    // TODO: Should blink the external LED fast for some MSEC every MSEC (e.g., 250 msec on off )
  }
}

/**
 * Control the LEDs.
 */
void controlLEDs() {
  controlOnboardLED();
  controlExternalLED();
}

// --------------------------------------------------------------------------

/**
  Setup bluetooth
 */
void setupBluetooth() {
  Serial.println("Starting BLE Keyboard");
  bleKeyboard.begin();
}

/**
  Setup the buttons, assign their GPIO pin, button kind and actions
  NOTE: GPIO_7 is not used, as it is the onboard LED
 */
void setupButtons() {
  // B1 - See image assets/RiderKontrolButtons.png
  _buttons[0] = new ButtonDefinition(GPIO_0, "B1", ButtonKind::Continous, RiderKontrolAction::ZoomIn);
  // B2
  _buttons[1] = new ButtonDefinition(GPIO_1, "B2", ButtonKind::Continous, RiderKontrolAction::ZoomOut);
  // B3
  _buttons[2] = new ButtonDefinition(GPIO_2, "B3", ButtonKind::ShortLong, RiderKontrolAction::PlayPauseMedia, RiderKontrolAction::NextTrackMedia);
  // B4
  _buttons[3] = new ButtonDefinition(GPIO_3, "B4", ButtonKind::ShortLong, RiderKontrolAction::ToggleFollow, RiderKontrolAction::EnterDiagMode);
  // B5
  _buttons[4] = new ButtonDefinition(GPIO_4, "B5", ButtonKind::Continous, RiderKontrolAction::PanUp);
  // B6
  _buttons[5] = new ButtonDefinition(GPIO_5, "B6", ButtonKind::Continous, RiderKontrolAction::PanRight);
  // B7
  _buttons[6] = new ButtonDefinition(GPIO_6, "B7", ButtonKind::Continous, RiderKontrolAction::PanDown);
  // B8
  _buttons[7] = new ButtonDefinition(GPIO_8, "B8", ButtonKind::Continous, RiderKontrolAction::PanLeft);
}

/**
 * The setup function runs once when you press reset or power the board.
 */
 void setup() {
  delay(INITIAL_STABLILIZE_INTERVAL_MSEC); // Let the chip stabilize

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  // initialize digital pin EXTERNAL_LED_PIN as an output.
  pinMode(EXTERNAL_LED_PIN, OUTPUT);

  neopixelWrite(ONBOARD_LED_PIN, LED_OFF_RED, LED_OFF_GREEN, LED_OFF_BLUE);

  Serial.begin(SERIAL_BAUD_RATE);
  delay(SERIAL_TIMEOUT_MSEC);  // Wait for serial port to connect and initialize

  setupButtons();

  setupBluetooth();

  printInitializedMessage();
}

/**
  Handles the commands coming on serial input
 */
void handleSerialInput() {
  if (Serial.available() > 0) {
    // Read input until a newline character is received
    String input = Serial.readStringUntil('\n');
    input.trim(); // Clean up spaces or carriage returns
    input.toLowerCase(); // Make it case-insensitive

    // TODO: Add more commands
    if (input == "diag") {
      _programState = ProgramState::Diag;
      _lastBlinkTime = 0;
      _diagEnterTime = millis();
      Serial.printf("Timestamp of entry: %u\n", _diagEnterTime);
      printDebugMessage("Program State: Diagnostics Mode");
    } else if (input == "normal") {
      _programState = ProgramState::Normal;
      _lastBlinkTime = 0;
      printDebugMessage("Program State: Normal Mode");
    } else if (input == "state") {
      Serial.printf("Current program state: %u\n", _programState);
    } else {
      if (_programState == ProgramState::Diag) {
        if (input == "hello") {
          printDebugMessage("Command: Hello World BT test");
          bleKeyboard.print("Hello world");
        } else if (input == "up") {
          printDebugMessage("Command: Up arrow");
          if (_firstTimePan) {
            _firstTimePan = false;
            bleKeyboard.write(DMD2_KEYCODE_UP_ARROW);
            printDebugMessage("First time pan");
          }

          bleKeyboard.write(DMD2_KEYCODE_UP_ARROW);
        }
        else if (input == "down") {
          printDebugMessage("Command: Down arrow");
          if (_firstTimePan) {
            _firstTimePan = false;
            bleKeyboard.write(DMD2_KEYCODE_DOWN_ARROW);
            printDebugMessage("First time pan");
          }

          bleKeyboard.write(DMD2_KEYCODE_DOWN_ARROW);
        }
        else if (input == "left") {
          printDebugMessage("Command: Left arrow");
          if (_firstTimePan) {
            _firstTimePan = false;
            bleKeyboard.write(DMD2_KEYCODE_LEFT_ARROW);
            printDebugMessage("First time pan");
          }

          bleKeyboard.write(DMD2_KEYCODE_LEFT_ARROW);
        }
        else if (input == "right") {
          printDebugMessage("Command: Right arrow");
          if (_firstTimePan) {
            _firstTimePan = false;
            bleKeyboard.write(DMD2_KEYCODE_RIGHT_ARROW);
            printDebugMessage("First time pan");
          }

          bleKeyboard.write(DMD2_KEYCODE_RIGHT_ARROW);
        }
        else if (input == "zin") {
          printDebugMessage("Command: Zoom In");
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_IN);
        }
        else if (input == "zinx") {
          printDebugMessage("Command: Zoom In x5");
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_IN);
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_IN);
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_IN);
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_IN);
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_IN);
        }
        else if (input == "zout") {
          printDebugMessage("Command: Zoom Out");
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_OUT);
        }
        else if (input == "zoutx") {
          printDebugMessage("Command: Zoom Out x5");
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_OUT);          
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_OUT);          
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_OUT);
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_OUT);
          bleKeyboard.write(DMD2_KEYCODE_ZOOM_OUT);
        }
        else if (input == "ctr") {
          printDebugMessage("Command: Center (Toggle follow)");
          bleKeyboard.write(DMD2_KEYCODE_CENTER);
          _firstTimePan = true;
        }
        else if (input == "onl") {
          printDebugMessage("Command: Toogle online layer");
          bleKeyboard.write(DMD2_KEYCODE_ONLINE_LAYER);
        }
        else if (input == "play") {
          printDebugMessage("Command: Play/Pause Media");
          bleKeyboard.write(DMD2_KEYCODE_PLAY_PAUSE);
        }
        else if (input == "next") {
          printDebugMessage("Command: Next Track");
          bleKeyboard.write(DMD2_KEYCODE_NEXT_TRACK);
        }
        else if (input == "mute") {
          printDebugMessage("Command: Mute");
          bleKeyboard.write(DMD2_KEYCODE_MUTE);
        }
        else {
          Serial.printf("Unknown command: %s\n", input);
        }
      } else {
        // Normal

        if (input == "todo") {
          // TODO
        } else {
          Serial.printf("Unknown command: %s\n", input);
        }
      }
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
    Serial.println("Auto exiting diagnostics mode, back to normal mode");
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
  Serial.printf("=> handleButtonPress: %s,", buttonDef->name);
  Serial.printf("kind: %u, action1: %u, action2: %u\n", 
    buttonDef->kind, 
    buttonDef->action1, 
    buttonDef->action2);

  if (buttonDef->action1 == RiderKontrolAction::PanUp) {
    printDebugMessage("Action: Pan Up");
    if (_firstTimePan) {
      _firstTimePan = false;
      bleKeyboard.write(DMD2_KEYCODE_UP_ARROW);
    }

    bleKeyboard.write(DMD2_KEYCODE_UP_ARROW);
  } else if (buttonDef->action1 == RiderKontrolAction::PanRight) {
    printDebugMessage("Action: Pan Right");
    if (_firstTimePan) {
      _firstTimePan = false;
      bleKeyboard.write(DMD2_KEYCODE_RIGHT_ARROW);
    }

    bleKeyboard.write(DMD2_KEYCODE_RIGHT_ARROW);
  } else if (buttonDef->action1 == RiderKontrolAction::PanDown) {
    printDebugMessage("Action: Pan Down");
    if (_firstTimePan) {
      _firstTimePan = false;
      bleKeyboard.write(DMD2_KEYCODE_DOWN_ARROW);
    }

    bleKeyboard.write(DMD2_KEYCODE_DOWN_ARROW);
  } else if (buttonDef->action1 == RiderKontrolAction::PanLeft) {
    printDebugMessage("Action: Pan Left");
    if (_firstTimePan) {
      _firstTimePan = false;
      bleKeyboard.write(DMD2_KEYCODE_LEFT_ARROW);
    }

    bleKeyboard.write(DMD2_KEYCODE_LEFT_ARROW);
  } else if (buttonDef->action1 == RiderKontrolAction::ToggleFollow) {
    printDebugMessage("Action: Pan Left");
    if (_firstTimePan) {
      _firstTimePan = false;
      bleKeyboard.write(DMD2_KEYCODE_LEFT_ARROW);
      printDebugMessage("First time pan");
    }

    bleKeyboard.write(DMD2_KEYCODE_LEFT_ARROW);
  }

    // TOOD: Handle other actions
}


void handleContinousButton(ButtonDefinition* buttonDef) {
  int now = millis();
  Switch* button = &buttonDef->button;

// Note: `pushed` is a one time transition on the trigger from unpushed to pushed,
  //  unlike `on` which is a current state of button
  if (button->pushed()) {
    // TODO: Constant for RGB colors, move RGB logic to interrupt handler, button press should override program state
    // TODO: Introduce LED state of button push?
    //neopixelWrite(ONBOARD_LED_PIN, 70,70,0);
    // TODO: Keep lastOnCheck and _firstButtonPush per button
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
    Serial.printf("Continous Button %s released\n", buttonDef->name);

    _isButtonPressed = false;
    _firstButtonPush = false;
    return;
  }
}

void handleShortLongButton(ButtonDefinition* buttonDef) {
  // TODO: Impl
  // Switch* button = &buttonDef->button;

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

/**
  Handles the BLE Keyboard connection logic
 */
void handleBLEKeyboardConnection() {
  if (_firstBLE && bleKeyboard.isConnected()) {
    _firstBLE = false;
    Serial.println("Connected as a BT Keyboard");
  }

  if (!_firstBLE && !bleKeyboard.isConnected()) {
    _firstBLE = true;
    Serial.println("Disconnected as a BT Keyboard");
  }
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
