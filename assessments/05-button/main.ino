// C++ code
//
// Read a button state and print the result to Serial.
// Prints "Button is pressed" or "Button is not pressed" every 500ms.
const byte BUTTON_PIN = 2;

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
}

void loop()
{
  if (digitalRead(BUTTON_PIN) == HIGH)
  {
    Serial.println("Button is pressed.");
  }
  else 
  {
    Serial.println("Button is not pressed.");
  }
  delay(500);
}