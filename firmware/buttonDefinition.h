#pragma once

#include "constants.h"
#include <avdweb_Switch.h>

class ButtonDefinition {
public:
  Switch button;
  String name;
  ButtonKind kind;
  RiderKontrolAction action1;
  const uint8_t* keyCodes1;
  bool isCodes1Media;
  RiderKontrolAction action2;
  const uint8_t* keyCodes2;
  bool isCodes2Media;

  ButtonDefinition(uint8_t gpioPin,
                   String name,
                   ButtonKind buttonKind,
                   RiderKontrolAction action1,
                   const uint8_t* keyCodes1,
                   bool isCodes1Media = false,
                   RiderKontrolAction action2 = RiderKontrolAction::NA,
                   const uint8_t* keyCodes2 = nullptr,
                   bool isCodes2Media = false)
      : 
        // Set button as input pull up resistor mode
        // A button press lowers voltage of pin to 0V, no press sets the voltage to VCC.
        button(gpioPin, INPUT_PULLUP),
        name(name),
        kind(buttonKind),
        action1(action1),
        keyCodes1(keyCodes1),
        isCodes1Media(isCodes1Media),
        action2(action2),
        keyCodes2(keyCodes2),
        isCodes2Media(isCodes2Media)

          {}
};