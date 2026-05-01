// C++ code
//
// Print "Hello" and the current time to Serial every 500ms.
// Uses millis() for non-blocking timing instead of delay().

unsigned long previousTimeSerialPrint = millis();
unsigned long timeIntervalSerialPrint = 500;

void setup() {
  Serial.begin(115200);
}

void loop() {
  unsigned long timeNow = millis();
  if (timeNow - previousTimeSerialPrint > timeIntervalSerialPrint) {
    Serial.println("Hello");
    Serial.println(millis());
    previousTimeSerialPrint += timeIntervalSerialPrint;
  }
}