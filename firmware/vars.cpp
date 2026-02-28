#include "vars.h"

// Definitions (actual storage). Only this file should define these.
bool _firstBLE = true;
ProgramState _programState = ProgramState::Normal;

int loopCount = 0;
unsigned long _lastBlinkTime = 0;
bool ledOn = false;