// C++ code
//
// Blink an LED connected to analog pin A5 used as digital output.
// LED turns on and off every 500ms.
const byte LED_PIN = A5;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_PIN, LOW);
  delay(500);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
}