// C++ code
//
// Step 1: Read distance from ultrasonic sensor using interrupts and print to serial
// Step 2: Blink warning LED at a rate proportional to measured distance
// Step 3: Lock app when obstacle is too close, blink both LEDs at 300ms when locked
// Step 4: Debounce button by checking state in loop, unlock app on button release
// Step 5: Setup LCD screen and print initialization message
// Step 6: Print distance and warning on LCD, show error message when locked
// Step 7: Setup IR remote receiver and map button codes
// Step 8: Handle IR remote commands, unlock app with PLAY button
// Step 9: Toggle distance unit with EQ button and save to EEPROM
// Step 10: Add LCD screen modes, toggle with UP/DOWN, reset settings with OFF

#include <LiquidCrystal.h>
#include <IRremote.h>
#include <EEPROM.h>

const byte ECHO_PIN = 3;
const byte TRIGGER_PIN = 4;
const byte WARNING_LED_PIN = 11;
const byte ERROR_LED_PIN = 12;
const byte BUTTON_PIN = 2;
const byte LCD_RS_PIN = A5;
const byte LCD_E_PIN = A4;
const byte LCD_D4_PIN = 6;
const byte LCD_D5_PIN = 7;
const byte LCD_D6_PIN = 8;
const byte LCD_D7_PIN = 9;
const byte IR_RECEIVE_PIN = 5;

// IR button mapping
const byte IR_BUTTON_PLAY = 5;
const byte IR_BUTTON_OFF = 0;
const byte IR_BUTTON_EQ = 13;
const byte IR_BUTTON_UP = 10;
const byte IR_BUTTON_DOWN = 8;

const double LOCK_DISTANCE = 10.0;
const double WARNING_DISTANCE = 50.0;

const byte DISTANCE_UNIT_CM = 0;
const byte DISTANCE_UNIT_IN = 1;
const float CM_TO_INCHES = 0.393701;
const byte EEPROM_ADDRESS_DISTANCE_UNIT = 50;

const byte LCD_MODE_DISTANCE = 0;
const byte LCD_MODE_SETTINGS = 1;

LiquidCrystal lcd(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

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

byte distanceUnit = DISTANCE_UNIT_CM;
byte lcdMode = LCD_MODE_DISTANCE;

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
  //Serial.println(warningLEDDelay);
}

void lock()
{
  if (!isLocked) 
  {
    //Serial.println("Locking");
    isLocked = true;
    warningLEDState = LOW;
    errorLEDState = LOW;
  }
}

void unlock()
{
  if (isLocked) 
  {
    //Serial.println("Unlocking");
    isLocked = false;
    errorLEDState = LOW;
    digitalWrite(ERROR_LED_PIN, errorLEDState);
    lcd.clear();
  }
}

void printDistanceOnLCD(double distance)
{
  if (isLocked) 
  {
    lcd.setCursor(0, 0);
    lcd.print("!!! Obstacle !!!     ");
    lcd.setCursor(0, 1);
    lcd.print("Press to unlock.     ");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    if (distanceUnit == DISTANCE_UNIT_IN) 
    {
      lcd.print(distance * CM_TO_INCHES);
      lcd.print(" in       ");
    }
    else {
      lcd.print(distance);
      lcd.print(" cm     ");
    }

    lcd.setCursor(0, 1);
    if (distance > WARNING_DISTANCE) 
    {
      lcd.print("No obstacle.       ");
    }
    else 
    {
      lcd.print("!! Warning !!       ");
    }
  }
}

void toggleDistanceUnit()
{
  if (distanceUnit == DISTANCE_UNIT_CM)
  {
    distanceUnit = DISTANCE_UNIT_IN;
  }
  else
  {
    distanceUnit = DISTANCE_UNIT_CM;
  }
  EEPROM.write(EEPROM_ADDRESS_DISTANCE_UNIT, distanceUnit);
}

void toggleLCDScreen()
{
  switch (lcdMode)
  {
    case LCD_MODE_DISTANCE:
    {
      lcdMode = LCD_MODE_SETTINGS;
      break;
    }
    case LCD_MODE_SETTINGS:
    {
      lcdMode = LCD_MODE_DISTANCE;
      break;
    }
    default:
    {
      lcdMode = LCD_MODE_DISTANCE;
    }
  }

  lcd.clear();

  if (lcdMode == LCD_MODE_SETTINGS)
  {
    lcd.setCursor(0, 0);
    lcd.print("Press on OFF to");
    lcd.setCursor(0, 1);
    lcd.print("reset settings.");
  }
}

void resetSettingsToDefault()
{
  if (lcdMode == LCD_MODE_SETTINGS)
  {
    distanceUnit = DISTANCE_UNIT_CM;
    EEPROM.write(EEPROM_ADDRESS_DISTANCE_UNIT, distanceUnit);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Settings have");
    lcd.setCursor(0, 1);
    lcd.print("been reset.");
  }
}

void handleIRCommand(long command)
{
  switch (command)
  {
    case IR_BUTTON_PLAY:
    {
      unlock();
      break;
    }
    case IR_BUTTON_OFF:
    {
      resetSettingsToDefault();
      break;
    }
    case IR_BUTTON_EQ:
    {
      toggleDistanceUnit();
      break;
    }
    case IR_BUTTON_UP:
    {
      toggleLCDScreen();
      break;
    }
    case IR_BUTTON_DOWN:
    {
      toggleLCDScreen();
      break;
    }
    default:
    {
      // do nothing
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  lcd.begin(16,2);
  IrReceiver.begin(IR_RECEIVE_PIN);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(WARNING_LED_PIN, OUTPUT);
  pinMode(ERROR_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  buttonState = digitalRead(BUTTON_PIN);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoPinInterrupt, CHANGE);
  
  distanceUnit = EEPROM.read(EEPROM_ADDRESS_DISTANCE_UNIT);
  if (distanceUnit == 255)
  {
    distanceUnit = DISTANCE_UNIT_CM;
  }
  
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
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
  else 
  {
    if (timeNow - lastTimeWarningLEDBlinked > warningLEDDelay)
    {
      lastTimeWarningLEDBlinked += warningLEDDelay;
      toggleWarningLED();
    }
  }
  
  if (IrReceiver.decode()) 
  {
    IrReceiver.resume();
    long command = IrReceiver.decodedIRData.command;
    handleIRCommand(command);
    //Serial.println(command);
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
    if (lcdMode == LCD_MODE_DISTANCE)
    {
      printDistanceOnLCD(distance);
    }
    if (distance < LOCK_DISTANCE) 
    {
      lock();
    }
    //Serial.println(distance);
  }
}