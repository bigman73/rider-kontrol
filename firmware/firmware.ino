#include "constants.h"

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

// Global variables for the program.
int loopCount = 0;
unsigned long lastBlinkTime = 0;
bool ledOn = false;

/**
 * Handle the toggle of the led.
 */
void handleToggleLed() {
  unsigned long now = millis();

  // Check if the LED blink interval has elapsed.
  if (now - lastBlinkTime >= LED_BLINK_INTERVAL_MSEC) {
    // Toggle the led state.
    ledOn = !ledOn;
    // Set the onboard led colors.
    neopixelWrite(ONBOARD_LED_PIN, 
      ledOn ? LED_ON_RED : LED_OFF_RED, 
      ledOn ? LED_ON_GREEN : LED_OFF_GREEN, 
      ledOn ? LED_ON_BLUE : LED_OFF_BLUE);

    // Set the external led state.
    // TODO: Just for testing, should be removed later. The external LED will have its own code to handle the blink or other application modes.
    digitalWrite(EXTERNAL_LED_PIN, ledOn ? HIGH : LOW);
    // Update the last blink time.
    lastBlinkTime = now;

    // Print the loop count if the led is on.
    if (ledOn) {
      printDebugMessage("Blink " + String(loopCount));
      loopCount++;
    }
  }
}

// --------------------------------------------------------------------------

/**
 * The setup function runs once when you press reset or power the board.
 */
 void setup() {
  delay(100); // Let the C3 stabilize

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  // initialize digital pin EXTERNAL_LED_PIN as an output.
  pinMode(EXTERNAL_LED_PIN, OUTPUT);

  neopixelWrite(ONBOARD_LED_PIN, LED_OFF_RED, LED_OFF_GREEN, LED_OFF_BLUE);

  Serial.begin(SERIAL_BAUD_RATE);
  delay(SERIAL_TIMEOUT_MSEC);  // Wait for serial port to connect and initialize

  printInitializedMessage();
}

/**
 * The loop function runs over and over again forever.
 */
void loop() {
  handleToggleLed();
}
