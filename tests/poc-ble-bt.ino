#include <BLEDevice.h>
#include <BluetoothSerial.h>

// Define the button pin
#define BUTTON_PIN 0  // Replace with the GPIO pin connected to your button

// Create instances for BLE and Bluetooth Serial
BluetoothSerial SerialBT;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Configure button pin with pull-up resistor

  Serial.begin(115200);
  delay(100);  // Small delay for stability

  // Check button state at startup
  if (digitalRead(BUTTON_PIN) == LOW) {  // Button pressed
    startClassicBluetooth();
  } else {  // Button not pressed
    startBLE();
  }
}

void loop() {
  // Add your main logic here
}

void startClassicBluetooth() {
  Serial.println("Starting in Classic Bluetooth mode...");
  if (!SerialBT.begin("ESP32_BT")) {  // Initialize Classic Bluetooth
    Serial.println("Failed to start Classic Bluetooth!");
  } else {
    Serial.println("Classic Bluetooth started. You can pair with ESP32_BT.");
  }
}

void startBLE() {
  Serial.println("Starting in BLE mode...");
  BLEDevice::init("ESP32_BLE");

  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService("12345678-1234-1234-1234-1234567890ab");

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    "87654321-4321-4321-4321-0987654321ba",
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setValue("Hello from BLE!");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  Serial.println("BLE started. You can connect to ESP32_BLE.");
}