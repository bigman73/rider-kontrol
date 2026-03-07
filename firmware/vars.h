#pragma once

#include <Arduino.h>

// Program state (shared)
enum class ProgramState { Normal, Diag };

// Shared global variables – declared here, defined in vars.cpp
extern bool _firstBLE;
extern ProgramState _programState;
extern unsigned long _diagEnterTime;
extern int _loopCount;
extern unsigned long _lastBlinkTime;
extern bool _ledOn;
extern bool _firstTimePan;

extern unsigned long _lastButtonOnCheckTime;
extern bool _firstButtonPush;
extern bool _isButtonPressed;