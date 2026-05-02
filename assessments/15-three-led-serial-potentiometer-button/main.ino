// C++ code
//
// Control three LEDs: red blinks at Serial-controlled speed,
// yellow brightness set by potentiometer, green controlled by button.

const byte LED_1_PIN = 12;
const byte LED_2_PIN = 11;
const byte LED_3_PIN = 10;

const byte POTENTIOMETER_PIN = A2;
const byte BUTTON_PIN = 2;

unsigned long previousTimeLED1Blink = millis();
unsigned long timeIntervalLED1Blink = 470;

int LED1State = LOW;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // Red LED
  unsigned long timeNow = millis();
  if (Serial.available() > 0) 
  {
    int data = Serial.parseInt();
    if ((data >= 100) && (data <= 4000))
    {
      timeIntervalLED1Blink = data;
    }
  }
  if (timeNow - previousTimeLED1Blink > timeIntervalLED1Blink)
  {
    LED1State = !LED1State;
    digitalWrite(LED_1_PIN, LED1State);
    previousTimeLED1Blink += timeIntervalLED1Blink;
  }
  
  // Yellow LED
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  int intensity = potentiometerValue / 4;
  analogWrite(LED_2_PIN, intensity);
  
  // Green LED
  if (digitalRead(BUTTON_PIN) == HIGH)
  {
    digitalWrite(LED_3_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_3_PIN, LOW);
  }
}