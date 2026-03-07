// constants won't change. They're used here to set pin numbers:
const int buttonPin = 1;  // the number of the pushbutton pin
const int ledPin = 7;

// variables will change:
int buttonState = LOW;  // variable for reading the pushbutton status

void setup() {
   //start serial connection
  Serial.begin(115200);

  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input with pull up resistor:
  pinMode(buttonPin, INPUT_PULLUP);

  neopixelWrite(ledPin, 0,0,0);

  Serial.println("~~ Test Buttons ~");
}

int lastButtonState = -1;

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    lastButtonState = buttonState;
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      neopixelWrite(ledPin, 0,0,0);
    } else {
      neopixelWrite(ledPin, 0,70,0);
    }

    Serial.print("Button state: ");
    Serial.println(buttonState);
  }

  delay(100);
}
