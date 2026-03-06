#include "constants.h"
#include "vars.h"
#include <BleKeyboard.h>

BleKeyboard bleKeyboard(
  BLUETOOTH_DEVICE, BLUETOOTH_MANUFACTURER, 
  BLUETOOTH_BATT_LEVEL_DEFAULT);

/**
 * Print the initialized message to the serial port.
 */
void printInitializedMessage() {
  Serial.print(FIRMWARE_NAME);
  Serial.print(" v");
  Serial.print(FIRMWARE_VERSION);
  Serial.println(" initialized");
}

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
 * Handle the toggle of the led.
 */
void handleToggleLed() {
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
      digitalWrite(EXTERNAL_LED_PIN, _ledOn ? HIGH : LOW);
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
      digitalWrite(EXTERNAL_LED_PIN, _ledOn ? HIGH : LOW);
      // Update the last blink time.
      _lastBlinkTime = now;
    }
  }
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
 * The loop function runs over and over again forever.
 */
void loop() {
  handleToggleLed();

  handleSerialInput();

  handleProgramStateLogic();

  if (_firstBLE && bleKeyboard.isConnected()) {
    _firstBLE = false;
    Serial.println("Connected as a BT Keyboard");
  }

  if (!_firstBLE && !bleKeyboard.isConnected()) {
    _firstBLE = true;
    Serial.println("Disconnected as a BT Keyboard");
  }
}
