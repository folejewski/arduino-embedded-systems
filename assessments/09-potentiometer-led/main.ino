// C++ code
//
// Control LED brightness with a potentiometer.
// Potentiometer value (0–1023) is divided by 4 to fit PWM range (0–255).
const byte POTENTIOMETER_PIN = 2;
const byte LED_PIN = 11;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  int LEDBrightness = potentiometerValue / 4;
  analogWrite(LED_PIN,LEDBrightness);
  delay(100);
}