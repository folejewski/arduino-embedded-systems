// C++ code
//
// Print "Hello Arduino" once on startup
// Print "In the loop" every 500ms repeatedly
void setup()
{
  Serial.begin(9600);
  Serial.println("Hello Arduino");
}

void loop()
{
  Serial.println("In the loop");
  delay(500);
}