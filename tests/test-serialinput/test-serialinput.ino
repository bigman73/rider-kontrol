// LOLIN C3-mini Serial Input LED Test

#define LED_PIN 7     // Use 7 for LOLIN v2.1.0, 8 for generic C3 Super Mini
#define BRIGHTNESS 40  // 0-255. Keep low to protect your eyes!

void setup() {
  delay(100);

  // No special initialization is required for neopixelWrite
  Serial.begin(9600);

  Serial.println("Lolin C3 Mini Ready.");
  Serial.println("Type 'red', 'green', or 'blue' to change the LED:");
}

void loop() {
  if (Serial.available() > 0) {
    // Read input until a newline character is received
    String input = Serial.readStringUntil('\n');
    input.trim(); // Clean up spaces or carriage returns
    input.toLowerCase(); // Make it case-insensitive

    if (input == "red") {
      Serial.println("Changing to RED");
      neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0);
    } else if (input == "green") {
      Serial.println("Changing to GREEN");
      neopixelWrite(LED_PIN, 0, BRIGHTNESS, 0);
    } else if (input == "blue") {
      Serial.println("Changing to BLUE");
      neopixelWrite(LED_PIN, 0, 0, BRIGHTNESS);
    } else if (input == "off") {
      Serial.println("LED Off");
      neopixelWrite(LED_PIN, 0, 0, 0);
    } else {
      Serial.print("Unknown command: ");
      Serial.println(input);
    }
  }
}