#include <esp_system.h>
#include <nvs.h>
#include <nvs_flash.h>

#define BUTTON_PIN 0  // GPIO pin for the button
#define HOLD_TIME 5000  // 5 seconds in milliseconds

bool specialMode = false;  // Default mode

void setup() {
  Serial.begin(115200);

  // Initialize NVS
  if (nvs_flash_init() != ESP_OK) {
    Serial.println("Failed to initialize NVS!");
    return;
  }

  // Read the mode from NVS
  specialMode = readModeFromNVS();
  Serial.printf("Current Mode: %s\n", specialMode ? "Special Mode" : "Normal Mode");

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Check if the button is held for 5 seconds
  if (isButtonHeld()) {
    Serial.println("Button held for 5 seconds. Toggling mode...");
    specialMode = !specialMode;  // Toggle the mode
    saveModeToNVS(specialMode);  // Save the new mode to NVS
    Serial.println("Rebooting to apply the new mode...");
    delay(1000);
    esp_restart();  // Reboot the ESP32
  }
}

void loop() {
  // Main logic based on the mode
  if (specialMode) {
    Serial.println("Running in Special Mode...");
  } else {
    Serial.println("Running in Normal Mode...");
  }
  delay(1000);
}

// Function to check if the button is held for 5 seconds
bool isButtonHeld() {
  unsigned long startTime = millis();
  while (digitalRead(BUTTON_PIN) == LOW) {
    if (millis() - startTime >= HOLD_TIME) {
      return true;
    }
  }
  return false;
}

// Function to save the mode to NVS
void saveModeToNVS(bool mode) {
  nvs_handle_t nvsHandle;
  if (nvs_open("storage", NVS_READWRITE, &nvsHandle) == ESP_OK) {
    nvs_set_u8(nvsHandle, "mode", mode ? 1 : 0);
    nvs_commit(nvsHandle);
    nvs_close(nvsHandle);
    Serial.println("Mode saved to NVS.");
  } else {
    Serial.println("Failed to open NVS!");
  }
}

// Function to read the mode from NVS
bool readModeFromNVS() {
  nvs_handle_t nvsHandle;
  uint8_t mode = 0;  // Default to Normal Mode
  if (nvs_open("storage", NVS_READONLY, &nvsHandle) == ESP_OK) {
    nvs_get_u8(nvsHandle, "mode", &mode);
    nvs_close(nvsHandle);
  } else {
    Serial.println("Failed to open NVS!");
  }
  return mode == 1;
}