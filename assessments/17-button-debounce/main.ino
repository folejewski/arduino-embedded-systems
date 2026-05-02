// C++ code
//
// Detect button press and release with debouncing using millis().
// Ignores state changes within 30ms to filter out electrical noise.

const byte BUTTON_PIN = 2;

unsigned long lastTimeButtonChanged = millis();
unsigned long debounceDelay = 30;

byte buttonState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  unsigned long timeNow = millis();
  if (timeNow - lastTimeButtonChanged > debounceDelay)
  {
    byte newButtonState = digitalRead(BUTTON_PIN);
  	if (newButtonState != buttonState)
  	{
      lastTimeButtonChanged = timeNow;
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
}