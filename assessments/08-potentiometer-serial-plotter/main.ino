// C++ code
//
// Read a potentiometer value (0–1023) and print it to Serial.
// Designed to be visualised in the Serial Plotter.
const byte POTENTIOMETER_PIN = 2;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.println(analogRead(POTENTIOMETER_PIN));
  delay(500);
}