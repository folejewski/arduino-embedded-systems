// C++ code
//
// Step 1: Read distance from ultrasonic sensor using interrupts and print to serial
// Step 2: Blink warning LED at a rate proportional to measured distance
// Step 3: Lock app when obstacle is too close, blink both LEDs at 300ms when locked
// Step 4: Debounce button by checking state in loop, unlock app on button release

const byte ECHO_PIN = 3;
const byte TRIGGER_PIN = 4;
const byte WARNING_LED_PIN = 11;
const byte ERROR_LED_PIN = 12;
const byte BUTTON_PIN = 2;
const double LOCK_DISTANCE = 10.0;

// ultrasonic
unsigned long lastTimeUltrasonicTrigger = millis();
unsigned long ultrasonicTriggerDelay = 60;

volatile unsigned long pulseInTimeBegin;
volatile unsigned long pulseInTimeEnd;
volatile bool newDistanceAvailable = false;

double previousDistance = 400.0;

// warning LED
unsigned long lastTimeWarningLEDBlinked = millis();
unsigned long warningLEDDelay = 500;
byte warningLEDState = LOW;

// error LED
unsigned long lastTimeErrorLEDBlinked = millis();
unsigned long errorLEDDelay = 300;
byte errorLEDState = LOW;

// push button
unsigned long lastTimeButtonChanged = millis();
unsigned long buttonDebounceDelay = 50;
byte buttonState;

bool isLocked = false;

void triggerUltrasonicSensor()
{
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
}

double getUltrasonicDistance()
{
  double durationMicros = pulseInTimeEnd - pulseInTimeBegin;
  double distance = durationMicros / 58.0;
  // durationMicros is measured using interrupt timestamps
  // distance in cm = duration / 58.0
  // for inches it would be duration / 148.0
  // HC-SR04 pulse travels to object and back
  if (distance > 400.0) 
  {
    return previousDistance;
  }
  distance = previousDistance * 0.6 + distance * 0.4;
  previousDistance = distance;
  return distance;
}

void echoPinInterrupt()
{
  if (digitalRead(ECHO_PIN) == HIGH) 
  { // pulse started
    pulseInTimeBegin = micros();
  }
  else 
  { // pulse stopped
    pulseInTimeEnd = micros();
    newDistanceAvailable = true;
  }
}

void toggleErrorLED()
{
  errorLEDState = (errorLEDState == HIGH) ? LOW : HIGH;
  digitalWrite(ERROR_LED_PIN, errorLEDState);
}

void toggleWarningLED()
{
  warningLEDState = (warningLEDState == HIGH) ? LOW : HIGH;
  digitalWrite(WARNING_LED_PIN, warningLEDState);
}

void setWarningLEDBlinkRateFromDistance(double distance)
{
  // 0 .. 400 cm -> 0 .. 1600 ms
  warningLEDDelay = distance * 4;
  Serial.println(warningLEDDelay);
}

void lock()
{
  if (!isLocked) {
    Serial.println("Locking");
    isLocked = true;
    warningLEDState = LOW;
    errorLEDState = LOW;
  }
}

void unlock()
{
  if (isLocked) {
    Serial.println("Unlocking");
    isLocked = false;
    errorLEDState = LOW;
    digitalWrite(ERROR_LED_PIN, errorLEDState);
  }
}

void setup() 
{
  Serial.begin(115200);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(WARNING_LED_PIN, OUTPUT);
  pinMode(ERROR_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  buttonState = digitalRead(BUTTON_PIN);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoPinInterrupt, CHANGE);
}

void loop() 
{
  unsigned long timeNow = millis();
  
  if (isLocked) {
    if (timeNow - lastTimeErrorLEDBlinked > errorLEDDelay)
    {
      lastTimeErrorLEDBlinked += errorLEDDelay;
      toggleErrorLED();
      toggleWarningLED();
    }
    if (timeNow - lastTimeButtonChanged > buttonDebounceDelay) 
    {
      byte newButtonState = digitalRead(BUTTON_PIN);
      if (newButtonState != buttonState) 
      {
        lastTimeButtonChanged = timeNow;
        buttonState = newButtonState;
        if (buttonState == LOW) 
        { // released
          unlock();
        }
      }
    }
  }
  else {
    if (timeNow - lastTimeWarningLEDBlinked > warningLEDDelay)
    {
      lastTimeWarningLEDBlinked += warningLEDDelay;
      toggleWarningLED();
    }
  }
  
  if (timeNow - lastTimeUltrasonicTrigger > ultrasonicTriggerDelay) 
  {
    lastTimeUltrasonicTrigger += ultrasonicTriggerDelay;
    triggerUltrasonicSensor();
  }
  
  if (newDistanceAvailable) 
  {
    newDistanceAvailable = false;
    double distance = getUltrasonicDistance();
    setWarningLEDBlinkRateFromDistance(distance);
    if (distance < LOCK_DISTANCE) {
      lock();
    }
    Serial.println(distance);
  }
}
