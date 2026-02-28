#pragma once

#include <Arduino.h>

// Serial configuration
constexpr long SERIAL_BAUD_RATE = 9600;
constexpr unsigned long SERIAL_TIMEOUT_MSEC = 250;

// LED timing
constexpr unsigned long LED_BLINK_INTERVAL_MSEC = 2000;

// Pin assignments
// Note: On Lolin C3 Mini v2.1.0 the onboard LED is RGB color and uses pin 7
constexpr uint8_t ONBOARD_LED_PIN = 7;
constexpr uint8_t EXTERNAL_LED_PIN = 10;

// LED color values
constexpr uint8_t LED_ON_RED = 20;
constexpr uint8_t LED_OFF_RED = 20;
constexpr uint8_t LED_ON_GREEN = 0;
constexpr uint8_t LED_OFF_GREEN = 20;
constexpr uint8_t LED_ON_BLUE = 20;
constexpr uint8_t LED_OFF_BLUE = 0;

// Firmware metadata
constexpr const char* FIRMWARE_VERSION = "0.1.0";
constexpr const char* FIRMWARE_NAME = "Rider Kontrol";

constexpr const char* BLUETOOTH_DEVICE = "Rider Kontrol";
constexpr const char* BLUETOOTH_MANUFACTURER = "bigman73";
constexpr uint8_t BLUETOOTH_BATT_LEVEL_DEFAULT = 100;


// Initial time to stablize before setup starts
constexpr uint8_t INITIAL_STABLILIZE_INTERVAL_MSEC = 100;

