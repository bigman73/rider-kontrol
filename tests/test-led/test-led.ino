// LOLIN C3-mini Onboard RGB LED Test
// v2.1.0 uses GPIO 7. If this doesn't work, try GPIO 8.

#define LED_PIN 7     // Use 7 for LOLIN v2.1.0, 8 for generic C3 Super Mini
#define BRIGHTNESS 20  // 0-255. Keep low to protect your eyes!

void setup() {
  // No special initialization is required for neopixelWrite
  Serial.begin(9600);
}

void loop() {
  Serial.println("Red");
  neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0);
  delay(1000);

  Serial.println("Green");
  neopixelWrite(LED_PIN, 0, BRIGHTNESS, 0);
  delay(1000);

  Serial.println("Blue");
  neopixelWrite(LED_PIN, 0, 0, BRIGHTNESS);
  delay(1000);

  Serial.println("Off");
  neopixelWrite(LED_PIN, 0, 0, 0);
  delay(1000);
}