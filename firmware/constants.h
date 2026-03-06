#pragma once

#include <Arduino.h>
#include <BleKeyboard.h>

// Serial configuration
constexpr long SERIAL_BAUD_RATE = 115200;
constexpr unsigned long SERIAL_TIMEOUT_MSEC = 250;

// LED timing
constexpr unsigned long LED_BLINK_INTERVAL_MSEC = 2000;
constexpr unsigned long LED_DIAG_BLINK_INTERVAL_MSEC = 500;

// Pin assignments
// Note: On Lolin C3 Mini v2.1.0 the onboard LED is RGB color and uses pin 7
constexpr uint8_t ONBOARD_LED_PIN = 7;
constexpr uint8_t EXTERNAL_LED_PIN = 10;

// LED color values
constexpr uint8_t LED_ON_RED = 20;
constexpr uint8_t LED_ON_GREEN = 20;
constexpr uint8_t LED_ON_BLUE = 20;
constexpr uint8_t LED_OFF_RED = 0;
constexpr uint8_t LED_OFF_GREEN = 20;
constexpr uint8_t LED_OFF_BLUE = 0;

constexpr uint8_t LED_DIAG_ON_RED = 40;
constexpr uint8_t LED_DIAG_ON_GREEN = 0;
constexpr uint8_t LED_DIAG_ON_BLUE = 40;
constexpr uint8_t LED_DIAG_OFF_GREEN = 0;
constexpr uint8_t LED_DIAG_OFF_RED = 0;
constexpr uint8_t LED_DIAG_OFF_BLUE = 40;

// Firmware metadata
constexpr const char* FIRMWARE_VERSION = "0.0.2";
constexpr const char* FIRMWARE_NAME = "Rider Kontrol";

constexpr const char* BLUETOOTH_DEVICE = "Rider Kontrol";
constexpr const char* BLUETOOTH_MANUFACTURER = "bigman73";
constexpr uint8_t BLUETOOTH_BATT_LEVEL_DEFAULT = 100;


// Initial time to stablize before setup starts
constexpr unsigned long INITIAL_STABLILIZE_INTERVAL_MSEC = 100;

constexpr unsigned long DIAG_MAX_TIME_MSEC = 180000;

// DMD2 key codes - See README.md for more details
constexpr uint8_t DMD2_KEYCODE_LEFT_ARROW = KEY_LEFT_ARROW;
constexpr uint8_t DMD2_KEYCODE_RIGHT_ARROW = KEY_RIGHT_ARROW;
constexpr uint8_t DMD2_KEYCODE_UP_ARROW = KEY_UP_ARROW;
constexpr uint8_t DMD2_KEYCODE_DOWN_ARROW = KEY_DOWN_ARROW;
constexpr uint8_t DMD2_KEYCODE_CENTER = '0';
constexpr uint8_t DMD2_KEYCODE_ONLINE_LAYER = '~';
constexpr uint8_t DMD2_KEYCODE_ZOOM_IN = '+';
constexpr uint8_t DMD2_KEYCODE_ZOOM_OUT = '-';
constexpr const uint8_t* DMD2_KEYCODE_PLAY_PAUSE = KEY_MEDIA_PLAY_PAUSE;
constexpr const uint8_t* DMD2_KEYCODE_NEXT_TRACK = KEY_MEDIA_NEXT_TRACK;
constexpr const uint8_t* DMD2_KEYCODE_MUTE = KEY_MEDIA_MUTE;
