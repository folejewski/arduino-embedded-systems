// C++ code
//
// Make the LED blink - on for 2 seconds, off for 1 second.
// Print "Power on" / "Power off" to Serial on each state change.
const byte LED_PIN = 13;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Power on");
  delay(2000);
  digitalWrite(LED_PIN, LOW);
  Serial.println("Power off");
  delay(1000);
}