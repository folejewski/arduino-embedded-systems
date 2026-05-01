// C++ code
//
// Blink two LEDs independently at different intervals using millis().
// LED 1 blinks every 470ms, LED 2 blinks every 1200ms.

const byte LED_1_PIN = 12;
const byte LED_2_PIN = 11;

unsigned long previousTimeLED1Blink = millis();
unsigned long timeIntervalLED1Blink = 470;

unsigned long previousTimeLED2Blink = millis();
unsigned long timeIntervalLED2Blink = 1200;

int LED1State = LOW;
int LED2State = LOW;

void setup() {
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
}

void loop() {
  unsigned long timeNow = millis();
  if (timeNow - previousTimeLED1Blink > timeIntervalLED1Blink) {
    LED1State = !LED1State;
    digitalWrite(LED_1_PIN, LED1State);
    previousTimeLED1Blink += timeIntervalLED1Blink;
  }
  
  if (timeNow - previousTimeLED2Blink > timeIntervalLED2Blink) {
    LED2State = !LED2State;
    digitalWrite(LED_2_PIN, LED2State);
    previousTimeLED2Blink += timeIntervalLED2Blink;
  }
}