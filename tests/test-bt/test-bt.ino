/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */

// #define USE_NIMBLE
#include <BleKeyboard.h>

BleKeyboard bleKeyboard("Rider Kontrol", "bigman73", 100);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    Serial.println("Connected to BT");
    Serial.println("Sending 'Hello World'..");
    bleKeyboard.print("Hello world");

    delay(1000);
  }
  Serial.println("Waiting 10 seconds...");
  delay(10000);
}