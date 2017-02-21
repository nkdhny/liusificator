#include <Multiservo.h>

#define POT_PIN A0

#define LED_MANUAL_PIN  8
#define LED_AUTO_PIN    9

#define BUT_OFF_PIN     0
#define BUT_MANUAL_PIN  2
#define BUT_AUTO_PIN    3

#define NEUTRAL_MODE    0
#define MANUAL_MODE     1
#define AUTO_MODE       2

#define LOW_ANGLE 10
#define HIGH_ANGLE 170
#define MAX_ANGLE 180

#define LEFT_SERVO 0
#define RIGHT_SERVO 17

#define AUTO_MODE_ITERATIONS 10

#define POLARITY 1

int mode = NEUTRAL_MODE;
int autoModeIterationsBeforeChange = 0;
bool autoModeGoingToLow = true;
Multiservo sLeft;
Multiservo sRight;
int servoAngle = LOW_ANGLE;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_MANUAL_PIN, OUTPUT);
  pinMode(LED_AUTO_PIN, OUTPUT);
  
  pinMode(BUT_OFF_PIN, INPUT_PULLUP);
  pinMode(BUT_MANUAL_PIN, INPUT_PULLUP);
  pinMode(BUT_AUTO_PIN, INPUT_PULLUP);

  sLeft.attach(LEFT_SERVO);
  sRight.attach(RIGHT_SERVO);
}

int readMode() {
  if (!digitalRead(BUT_OFF_PIN)) {
    return NEUTRAL_MODE;
  }
  if (!digitalRead(BUT_MANUAL_PIN)) {
    return MANUAL_MODE;
  }
  if (!digitalRead(BUT_AUTO_PIN)) {
    return AUTO_MODE;
  }

  return -1;
}

boolean adjustMode() {
  int newMode = readMode();
  boolean modeNotChanged = ( (mode == newMode) || (newMode == -1) );
  if (newMode >= 0) {
    mode = newMode;
  }

  return !modeNotChanged;
}

void showNeutralMode() {
  digitalWrite(LED_MANUAL_PIN, LOW);
  digitalWrite(LED_AUTO_PIN, LOW);
}

void showManualMode() {
  digitalWrite(LED_MANUAL_PIN, HIGH);
  digitalWrite(LED_AUTO_PIN, LOW);
}

void showAutoMode() {
  digitalWrite(LED_MANUAL_PIN, LOW);
  digitalWrite(LED_AUTO_PIN, HIGH);
}

void onModeChanged() {
  Serial.print("Mode changed: ");
  Serial.print(mode);
  Serial.print('\n');
}

int readAngle() {
  int level = analogRead(POT_PIN);
  int angle = (int)(level / 1023.0 * (HIGH_ANGLE - LOW_ANGLE));
  return constrain(angle, LOW_ANGLE, HIGH_ANGLE);
}

void setAngle(int angle) {
  if (angle != servoAngle) {
    if (POLARITY == 1) {
      sLeft.write(angle);
      sRight.write(MAX_ANGLE - angle);
    } else {
      sRight.write(angle);
      sLeft.write(MAX_ANGLE - angle);
    }
    servoAngle = angle;
  }
}

void actManual() {
  int angle = readAngle();
  setAngle(angle);
}

void actNeutral() {
  setAngle(LOW_ANGLE);
}

void actAuto() {
  if (autoModeIterationsBeforeChange > 0) {
    // Мы все еще спускаемся, но не хотим блокировать поток 
    // позволяя опрашивать кнопки
    autoModeIterationsBeforeChange--;
  } else {
    autoModeIterationsBeforeChange = AUTO_MODE_ITERATIONS;
    autoModeGoingToLow = !autoModeGoingToLow;
    if (!autoModeGoingToLow) {
      int angle = readAngle();
      setAngle(angle);
    } else {
      setAngle(LOW_ANGLE);
    }
  }

  delay(100);
}

void loop() {
  
  boolean modeChanged = adjustMode();
  
  
  if (modeChanged) {
    onModeChanged();
  }

  switch (mode) {
    case MANUAL_MODE:
      showManualMode();
      actManual();
      break;
    case AUTO_MODE:
      showAutoMode();
      actAuto();
      break;
    default:
      showNeutralMode();
      actNeutral();
      break;
  }
}
