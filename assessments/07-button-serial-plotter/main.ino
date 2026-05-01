// C++ code
//
// Read a button state and print the value (0 or 1) to Serial.
// Designed to be visualised in the Serial Plotter.
const byte BUTTON_PIN = 2;

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
}

void loop()
{
  Serial.println(digitalRead(BUTTON_PIN));
  delay(500);
}