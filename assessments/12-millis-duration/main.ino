// C++ code
//
// Demonstrate the millis() function by measuring elapsed time.
// Prints the duration of a 500ms delay to Serial.

void setup()
{
  Serial.begin(115200);
  unsigned long timeBegin = millis();
  delay(500);
  unsigned long timeEnd = millis();
  unsigned long duration = timeEnd - timeBegin;
  Serial.print("The duration is: ");
  Serial.println(duration);
}

void loop()
{
}