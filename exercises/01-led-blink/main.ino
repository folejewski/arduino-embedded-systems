// C++ code
//
// Power on the LED for 1 seconds
// Power off the LED for 1 second
void setup()
{
  pinMode(13, OUTPUT);
}

void loop()
{
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}