// C++ code
//
// Count button presses using a hardware interrupt with debouncing.
// Prints the current count to Serial on each press.
const byte BUTTON_PIN = 2;

volatile unsigned long lastTimeButtonPressed = millis();
unsigned long debounceDelay = 50;

volatile bool buttonPressed = false;

int counter = 0;

void buttonPressedInterrupt()
{
  unsigned long timeNow = millis();
  if (timeNow - lastTimeButtonPressed > debounceDelay)
  {
    lastTimeButtonPressed = timeNow;
    buttonPressed = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressedInterrupt, FALLING);
}

void loop() {
  if (buttonPressed) {
    buttonPressed = false;
    counter++;
    Serial.print("Counter is: ");
    Serial.println(counter);
  }
}