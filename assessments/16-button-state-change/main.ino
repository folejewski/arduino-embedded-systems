// C++ code
//
// Detect button press and release events using state change detection.
// Prints "Button pressed" or "Button released" to Serial only on change.

const byte BUTTON_PIN = 2;

byte buttonState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  byte newButtonState = digitalRead(BUTTON_PIN);
  if (newButtonState != buttonState)
  {
    buttonState = newButtonState;
    if (buttonState == HIGH)
    {
      Serial.println("Button pressed");
    }
    else
    {
      Serial.println("Button released");
    }
  }
}