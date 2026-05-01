// C++ code
//
// Turn an LED on or off based on a button press.
// LED is on while button is pressed, off when released.
const byte BUTTON_PIN = 2;
const byte LED_PIN = 11;

void setup()
{
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  if (digitalRead(BUTTON_PIN) == HIGH)
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else 
  {
    digitalWrite(LED_PIN, LOW);
  }
  delay(500);
}