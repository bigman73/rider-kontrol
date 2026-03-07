#include <avdweb_Switch.h>

const int ledPin = 7;

// Define the size of the array
const int NUM_BUTTONS = 3;

constexpr int GPIO_0 = 0;
constexpr int GPIO_1 = 1;
constexpr int GPIO_2 = 2;

// Declare an array of Switch objects, one for each button, and initialize them
// All buttons are using internal pull up resistor. A button press lowers voltage of pin to 0V
Switch buttons[NUM_BUTTONS] = {
  Switch(GPIO_0, INPUT_PULLUP),
  Switch(GPIO_1, INPUT_PULLUP),
  Switch(GPIO_2, INPUT_PULLUP)
};

void setup() {
  delay(1000);

   //start serial connection
  Serial.begin(115200);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);

  neopixelWrite(ledPin, 0,0,0);

  Serial.println("~~ Test Debounced Buttons ~");
  Serial.printf("Number of buttons: %u\n", NUM_BUTTONS);
}

int lastOnCheck = 0;
bool firstPush = false;
const int REPEAT_PUSH_INTERVAL1_MSEC = 650;
const int REPEAT_PUSH_INTERVAL2_MSEC = 150;

void handleButtonPress(int buttonIndex) {
   Serial.printf("=> handleButtonPress: %u\n", buttonIndex);

}

void handleButtons() {
  for (int buttonIndex=0; buttonIndex < NUM_BUTTONS; buttonIndex++) {
    buttons[buttonIndex].poll();
  }

  int now = millis();

  for (int buttonIndex=0; buttonIndex < NUM_BUTTONS; buttonIndex++) {
    Switch button = buttons[buttonIndex];

    // Note: `pushed` is a one time transition on the trigger from unpushed to pushed, 
    //  unlike `on` which is a current state of button
    if (button.pushed()) { 
      Serial.printf("Button %u pushed\n", buttonIndex);
      neopixelWrite(ledPin, 70,70,0);
      // TODO: Keep lastOnCheck and firstPush per button
      lastOnCheck = now;
      firstPush = true;
      handleButtonPress(buttonIndex);
      return;
    } else if (button.on() && firstPush && (now - lastOnCheck > REPEAT_PUSH_INTERVAL1_MSEC)) {
      // The button long press, interval 1
      firstPush = false;
      neopixelWrite(ledPin, 0,70,70);
      Serial.printf("Button %u still pushed - 1st time\n", buttonIndex);
      handleButtonPress(buttonIndex);
      return;
    } else if (button.on() && !firstPush && (now - lastOnCheck > REPEAT_PUSH_INTERVAL2_MSEC)) {
      // The button long press, interval 2
      lastOnCheck = now;
      neopixelWrite(ledPin, 70,0,70);
      Serial.printf("Button %u still pushed\n", buttonIndex);
      handleButtonPress(buttonIndex);
      return;
    }

    if (button.released()) {
      Serial.printf("Button %u released\n", buttonIndex);
      neopixelWrite(ledPin, 0,0,0);
      return;
    }
  }
}

void loop() {
  handleButtons();
}
