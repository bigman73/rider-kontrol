#pragma once

#include "constants.h"
#include <avdweb_Switch.h>

class ButtonDefinition {
public:
  Switch button;
  String name;
  ButtonKind kind;
  RiderKontrolAction action1;
  RiderKontrolAction action2;

  ButtonDefinition(uint8_t gpioPin,
                   String name,
                   ButtonKind buttonKind,
                   RiderKontrolAction action1,
                   RiderKontrolAction action2 = RiderKontrolAction::NA)
      : 
        // Set button as input pull up resistor mode
        // A button press lowers voltage of pin to 0V, no press sets the voltage to VCC.
        button(gpioPin, INPUT_PULLUP),
        name(name),
        kind(buttonKind),
        action1(action1),
        action2(action2) {}
};