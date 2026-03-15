#pragma once

#include "constants.h"
#include <avdweb_Switch.h>

class ButtonDefinition {
public:
  Switch button;
  String name;
  ButtonKind kind;
  RiderKontrolAction action1;
  uint8_t keyCode1;
  RiderKontrolAction action2;
  uint8_t keyCode2;

  ButtonDefinition(uint8_t gpioPin,
                   String name,
                   ButtonKind buttonKind,
                   RiderKontrolAction action1,
                   uint8_t keyCode1,
                   RiderKontrolAction action2 = RiderKontrolAction::NA,
                   uint8_t keyCode2 = 0)
      : 
        // Set button as input pull up resistor mode
        // A button press lowers voltage of pin to 0V, no press sets the voltage to VCC.
        button(gpioPin, INPUT_PULLUP),
        name(name),
        kind(buttonKind),
        action1(action1),
        keyCode1(keyCode1),
        action2(action2),
        keyCode2(keyCode2)
        {}
};