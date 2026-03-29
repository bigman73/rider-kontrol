#pragma once

#include <Arduino.h>
#include <BleKeyboard.h>

// Program state (shared)
enum class ProgramState { Normal, Diag };

template <typename T, size_t N>
constexpr size_t arraySize(const T (&)[N]) { return N; }

// Firmware metadata
constexpr const char* FIRMWARE_VERSION = "0.0.5";
constexpr const char* FIRMWARE_NAME = "Rider Kontrol";

constexpr const char* BLUETOOTH_DEVICE = "Rider Kontrol";
constexpr const char* BLUETOOTH_MANUFACTURER = "bigman73";

// Define the GPIO pins on the ESP32-C3 Mini board
constexpr uint8_t GPIO_0 = 0;
constexpr uint8_t GPIO_1 = 1;
constexpr uint8_t GPIO_2 = 2;
constexpr uint8_t GPIO_3 = 3;
constexpr uint8_t GPIO_4 = 4;
constexpr uint8_t GPIO_5 = 5;
constexpr uint8_t GPIO_6 = 6;
constexpr uint8_t GPIO_7 = 7;
constexpr uint8_t GPIO_8 = 8;
constexpr uint8_t GPIO_10 = 10;

// Serial configuration
constexpr long SERIAL_BAUD_RATE = 115200;
constexpr unsigned long SERIAL_TIMEOUT_MSEC = 250;
constexpr bool SERIAL_DEBUG = false;

// LED timing
constexpr unsigned long LED_BLINK_INTERVAL_MSEC = 2000;
constexpr unsigned long LED_DIAG_BLINK_INTERVAL_MSEC = 300;
constexpr unsigned long EXT_LED_HEARTBEAT_CADENCE_MSEC = 5000;
constexpr unsigned long EXT_LED_HEARTBEAT_DURATION_MSEC = 100;
constexpr unsigned long EXT_LED_HEARTBEAT_COOLDOWN_MSEC = 2000;


// Pin assignments
// Note: On Lolin C3 Mini v2.1.0 the onboard LED is RGB color and uses pin 7
constexpr uint8_t ONBOARD_LED_PIN = GPIO_7;
constexpr uint8_t EXTERNAL_LED_PIN = GPIO_10;

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

constexpr uint8_t LED_BUTTON_PRESS_RED = 0;
constexpr uint8_t LED_BUTTON_PRESS_GREEN = 70;
constexpr uint8_t LED_BUTTON_PRESS_BLUE = 0;


constexpr uint8_t BLUETOOTH_BATT_LEVEL_DEFAULT = 100;


// Initial time to stablize before setup starts
constexpr unsigned long INITIAL_STABLILIZE_INTERVAL_MSEC = 100;

constexpr unsigned long DIAG_MAX_TIME_MSEC = 180000;

// DMD2 key codes - See README.md for more details
constexpr uint8_t DMD2_KEYCODE_LEFT_ARROW[] = { KEY_LEFT_ARROW };
constexpr uint8_t DMD2_KEYCODE_RIGHT_ARROW[] = { KEY_RIGHT_ARROW };
constexpr uint8_t DMD2_KEYCODE_UP_ARROW[] = { KEY_UP_ARROW };
constexpr uint8_t DMD2_KEYCODE_DOWN_ARROW[] = { KEY_DOWN_ARROW };
constexpr uint8_t DMD2_KEYCODE_CENTER[] = { '0' };
constexpr uint8_t DMD2_KEYCODE_ONLINE_LAYER[] = { '~' };
constexpr uint8_t DMD2_KEYCODE_ZOOM_IN[] = { '+' };
constexpr uint8_t DMD2_KEYCODE_ZOOM_OUT[] = { '-' };
constexpr uint8_t DMD2_KEYCODE_PLAY_PAUSE[] = {8, 0}; // KEY_MEDIA_PLAY_PAUSE
constexpr uint8_t DMD2_KEYCODE_NEXT_TRACK[] = {1, 0}; // KEY_MEDIA_NEXT_TRACK
constexpr uint8_t DMD2_KEYCODE_MUTE[] = {16, 0}; // KEY_MEDIA_MUTE

// -- Button constants
// Kinds of buttons
// Continous - A press invokes the action, then waits for an interval, then continues invoking the action as long as the button is pressed
// ShortLong - A short press and release invokes action 1 while long press invokes action 2
enum class ButtonKind { Continous, ShortLong };

// Actions that can be performed
enum class RiderKontrolAction { 
    ZoomIn, ZoomOut, 
    PlayPauseMedia, NextTrackMedia, MuteMedia, 
    PanUp, PanRight, PanDown, PanLeft, 
    ToggleFollow, ToggleLayer, 
    EnterDiagMode, NA };

// Define the number of buttons
constexpr int NUM_BUTTONS = 8;

constexpr int REPEAT_PUSH_INTERVAL1_MSEC = 650;
constexpr int REPEAT_PUSH_INTERVAL2_MSEC = 150;

constexpr const char* COMMAND_DIAG_HELLO = "hello";
constexpr const char* COMMAND_DIAG_UP = "up";
constexpr const char* COMMAND_DIAG_DOWN = "down";
constexpr const char* COMMAND_DIAG_RIGHT = "right";
constexpr const char* COMMAND_DIAG_LEFT = "left";
constexpr const char* COMMAND_DIAG_ZOOM_IN = "zin";
constexpr const char* COMMAND_DIAG_ZOOM_OUT = "zout";
constexpr const char* COMMAND_DIAG_ZOOM_IN_X = "zinx";
constexpr const char* COMMAND_DIAG_ZOOM_OUT_X = "zoutx";
constexpr const char* COMMAND_DIAG_CENTER = "ctr";
constexpr const char* COMMAND_DIAG_SAT_LAYER = "sat";
constexpr const char* COMMAND_DIAG_PLAY_MEDIA = "play";
constexpr const char* COMMAND_DIAG_NEXT_MEDIA = "next";
constexpr const char* COMMAND_DIAG_MUTE_MEDIA = "mute";
constexpr const char* COMMAND_DIAG_VERSION = "version";

// WIFI OTA
constexpr const int WIFI_CONNECT_TIMEOUT_MS = 20000;
// NVS namespace and keys for WiFi credentials (write once via another sketch or NVS tool).
constexpr const char* const PREFS_NS_WIFI = "rkwifi";
constexpr const char* const PREFS_KEY_SSID = "ssid";
constexpr const char* const PREFS_KEY_PASSWORD = "password";

constexpr const char* const PREFS_NS_OTA = "rkota";
constexpr const char* const PREFS_KEY_OTA_PASSWORD = "password";
constexpr const char* const DEFAULT_OTA_PASSWORD = "admin";