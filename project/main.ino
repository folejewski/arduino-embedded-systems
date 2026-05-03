// C++ code
//
// Step 1: Read distance from ultrasonic sensor using interrupts and print to serial

const byte ECHO_PIN = 3;
const byte TRIGGER_PIN = 4;

unsigned long lastTimeUltrasonicTrigger = millis();
unsigned long ultrasonicTriggerDelay = 60;

volatile unsigned long pulseInTimeBegin;
volatile unsigned long pulseInTimeEnd;
volatile bool newDistanceAvailable = false;

double previousDistance = 400.0;

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

void setup() 
{
  Serial.begin(115200);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoPinInterrupt, CHANGE);
}

void loop() 
{
  unsigned long timeNow = millis();

  if (timeNow - lastTimeUltrasonicTrigger > ultrasonicTriggerDelay) 
  {
    lastTimeUltrasonicTrigger += ultrasonicTriggerDelay;
    triggerUltrasonicSensor();
  }

  if (newDistanceAvailable) 
  {
    newDistanceAvailable = false;
    double distance = getUltrasonicDistance();
    Serial.println(distance);
  }
}
