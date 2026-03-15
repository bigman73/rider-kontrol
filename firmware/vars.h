#pragma once

#include "constants.h"
#include "buttonDefinition.h"
#include <Arduino.h>
#include <avdweb_Switch.h>

// Shared global variables – declared here, defined in vars.cpp
extern bool _firstBLE;
extern ProgramState _programState;
extern unsigned long _diagEnterTime;
extern int _loopCount;
extern unsigned long _lastBlinkTime;
extern bool _ledOn;
extern bool _firstTimePan;
extern bool _externalLedHeartbeat;

extern unsigned long _lastButtonOnCheckTime;
extern bool _firstButtonPush;
extern bool _isButtonPressed;

// Declare an array of pointers to ButtonDefinition objects, one for each button
extern ButtonDefinition* _buttons[NUM_BUTTONS];