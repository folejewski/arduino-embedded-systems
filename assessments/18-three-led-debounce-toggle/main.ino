// C++ code
//
// Blink LED 1 continuously, toggle LED 2 and 3 with a debounced button.
// LED 1 blinks every 1000ms, button press cycles through LED 2/3 states.

const byte LED_1_PIN = 12;
const byte LED_2_PIN = 11;
const byte LED_3_PIN = 10;

const byte BUTTON_PIN = 2;

unsigned long lastTimeLED1Blinked = millis();
unsigned long blinkDelayLED1 = 1000;
byte LED1State = LOW; 

unsigned long lastTimeButtonChanged = millis();
unsigned long debounceDelay = 50;

int toggleLEDState = 1;

void toggleOtherLEDs() {
  toggleLEDState++;
  if (toggleLEDState > 3) toggleLEDState = 1;
  digitalWrite(LED_2_PIN, toggleLEDState == 2 ? HIGH : LOW);
  digitalWrite(LED_3_PIN, toggleLEDState == 3 ? HIGH : LOW);
}

byte buttonState = LOW;

void blinkLED1() {
  LED1State = !LED1State;
  digitalWrite(LED_1_PIN, LED1State);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  buttonState = digitalRead(BUTTON_PIN);
}

void loop() {
  //LED 1
  unsigned long timeNow = millis();
  if (timeNow - lastTimeLED1Blinked > blinkDelayLED1) {
    lastTimeLED1Blinked += blinkDelayLED1;
    blinkLED1();
  }
  // LED 2-3
  if (timeNow - lastTimeButtonChanged > debounceDelay) {
    byte newButtonState = digitalRead(BUTTON_PIN);
    if (newButtonState != buttonState) {
      lastTimeButtonChanged = timeNow;
      buttonState = newButtonState;
      if (buttonState == HIGH) {
        toggleOtherLEDs();
      }
    }
  }
}