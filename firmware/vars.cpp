#include "vars.h"

// Definitions (actual storage). Only this file should define these.
bool _firstBLE = true;
ProgramState _programState = ProgramState::Normal;
unsigned long _diagEnterTime = 0;
int _loopCount = 0;
unsigned long _lastBlinkTime = 0;
bool _ledOn = false;
bool _firstTimePan = true;
bool _externalLedHeartbeat = false;

unsigned long _lastButtonOnCheckTime = 0;
bool _firstButtonPush = false;
bool _isButtonPressed = false;
ButtonDefinition* _buttons[NUM_BUTTONS] = { nullptr };