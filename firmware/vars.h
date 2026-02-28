#pragma once

#include <Arduino.h>

// Program state (shared)
enum class ProgramState { Normal, Diag };

// Shared variables – declared here, defined in vars.cpp
extern bool _firstBLE;
extern ProgramState _programState;

// Global variables for the program.
extern int loopCount;
extern unsigned long _lastBlinkTime;
extern bool ledOn;
