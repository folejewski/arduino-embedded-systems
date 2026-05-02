// C++ code
//
// Toggle an LED using a hardware interrupt on button release.
// Uses attachInterrupt() with FALLING edge detection.
const byte LED_PIN = 12;
const byte BUTTON_PIN = 2;

byte LEDState = LOW;
volatile bool buttonReleased = false;

void toggleLED()
{
  if (LEDState == LOW) {
    LEDState = HIGH;
  }
  else {
    LEDState = LOW;
  }
  digitalWrite(LED_PIN, LEDState);
}

void buttonReleasedInterrupt()
{
  buttonReleased = true;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonReleasedInterrupt, FALLING);
}

void loop() {
  if (buttonReleased) {
    buttonReleased = false;
    toggleLED();
  }
}