// C++ code
//
// Turn on the LED with a brightness level of 200 (out of 255) using PWM (analogWrite).
const byte LED_PIN = 11;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 200);
}

void loop()
{
}