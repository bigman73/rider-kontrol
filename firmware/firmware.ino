#define SERIAL_BAUD_RATE 9600
#define SERIAL_TIMEOUT_MSEC 250
#define BLINK_INTERVAL_MSEC 500

#define FIRMWARE_VERSION "0.1.0"
#define FIRMWARE_NAME "Rider Kontrol"
#define FIRMWARE_INITIALIZED_MESSAGE FIRMWARE_NAME " v" FIRMWARE_VERSION " initialized"

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

  // Check if the blink interval has elapsed.
  if (now - lastBlinkTime >= BLINK_INTERVAL_MSEC) {
    // Toggle the led state.
    ledOn = !ledOn;
    // Set the led state.
    digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
    // Update the last blink time.
    lastBlinkTime = now;

    // Print the loop count if the led is on.
    if (ledOn) {
      printDebugMessage("Blink " + String(loopCount));
      loopCount++;
    }
  }
}

/**
 * The setup function runs once when you press reset or power the board.
 */
 void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

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
