// C++ code
//
// Create a circuit.
//Make the LED blink - on for 1 second, off for 1 second.
const byte LED_PIN = 11;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    digitalWrite(LED_PIN, HIGH);
  	delay(1000);
    digitalWrite(LED_PIN, LOW);
  	delay(1000);
}