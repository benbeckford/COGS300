// ---- Pin Definitions ----
#define LEFT_MOTOR_POWER 6
#define LEFT_MOTOR_FORWARD 5
#define LEFT_MOTOR_BACKWARD 7

#define RIGHT_MOTOR_POWER 3
#define RIGHT_MOTOR_FORWARD 4
#define RIGHT_MOTOR_BACKWARD 2

#define LEFT_ENCODER 12
#define RIGHT_ENCODER 13

#define FRONT_ULTRASONIC_TRIG 11
#define FRONT_ULTRASONIC_ECHO 10

// ---- Adjustable Settings ----
int leftSpinSpeed  = 130;
int rightSpinSpeed = 170;

int forwardLeftSpeed  = 100;
int forwardRightSpeed = 100;

int pauseTime = 300;

int forwardTimeAfterDetection = 800;
int leftResetPulses = 4;

// ---- Detection Settings ----
const int OBJECT_DISTANCE_THRESHOLD_CM = 50;
const int MIN_OBJECT_PULSES = 3;
const int MAX_SCAN_PULSES = 80;
const int PEAK_TOLERANCE_CM = 1;

// ---- Stop threshold ----
const int STOP_DISTANCE_CM = 10;

// ---- Encoder Variables ----
long leftCount = 0;
long rightCount = 0;

int lastLeftState = LOW;
int lastRightState = LOW;

// ---- Ultrasonic ----
long readUltrasonicCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);
  if (duration == 0) return -1;

  return duration / 58;
}

// ---- Stop Motors ----
void stopMotors() {
  analogWrite(LEFT_MOTOR_POWER, 0);
  analogWrite(RIGHT_MOTOR_POWER, 0);

  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
}

// ---- Emergency Stop ----
void stopForever() {
  stopMotors();
  Serial.println("OBJECT TOO CLOSE - FINAL STOP");

  while(true);
}

// ---- Spin Right ----
void startSpinRight() {
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);

  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);

  analogWrite(LEFT_MOTOR_POWER, leftSpinSpeed);
  analogWrite(RIGHT_MOTOR_POWER, rightSpinSpeed);
}

// ---- Spin Left ----
void startSpinLeft() {
  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);

  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);

  analogWrite(LEFT_MOTOR_POWER, leftSpinSpeed);
  analogWrite(RIGHT_MOTOR_POWER, rightSpinSpeed);
}

// ---- Move Forward ----
void moveForward() {
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);

  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);

  analogWrite(LEFT_MOTOR_POWER, forwardLeftSpeed);
  analogWrite(RIGHT_MOTOR_POWER, forwardRightSpeed);
}

// ---- Move Forward For Fixed Time ----
void moveForwardForTime(int durationMs) {

  Serial.print("MOVING FORWARD FOR ");
  Serial.print(durationMs);
  Serial.println(" ms");

  unsigned long start = millis();

  moveForward();

  while (millis() - start < durationMs) {

    long cm = readUltrasonicCM(FRONT_ULTRASONIC_TRIG, FRONT_ULTRASONIC_ECHO);

    Serial.print("Forward Distance: ");
    Serial.println(cm);

    if (cm > 0 && cm <= STOP_DISTANCE_CM) {
      stopForever();
    }

    delay(30);
  }

  stopMotors();
  Serial.println("FORWARD DONE");
}

// ---- Encoder Update ----
void updateEncoders() {
  int leftState = digitalRead(LEFT_ENCODER);
  int rightState = digitalRead(RIGHT_ENCODER);

  if (leftState == HIGH && lastLeftState == LOW) leftCount++;
  if (rightState == HIGH && lastRightState == LOW) rightCount++;

  lastLeftState = leftState;
  lastRightState = rightState;
}

// ---- Spin One Tick ----
void spinUntilNextRightEncoder() {
  long startRight = rightCount;

  startSpinRight();

  while (true) {
    updateEncoders();
    if (rightCount > startRight) break;
  }

  stopMotors();
}

// ---- Spin Left Pulses ----
void spinLeftPulses(long pulses) {

  long start = rightCount;

  startSpinLeft();

  while (true) {
    updateEncoders();
    if ((rightCount - start) >= pulses) break;
  }

  stopMotors();
}

// ---- Scan ----
bool scanForObjectAndCenterIfFound() {

  bool inObject = false;

  long objectStartPulse = -1;
  long objectEndPulse = -1;

  long nearestDistance = 100000;
  long peakStartPulse = -1;
  long peakEndPulse = -1;

  Serial.println("---- NEW SCAN ----");

  for (int scanPulse = 1; scanPulse <= MAX_SCAN_PULSES; scanPulse++) {

    spinUntilNextRightEncoder();

    long cm = readUltrasonicCM(FRONT_ULTRASONIC_TRIG, FRONT_ULTRASONIC_ECHO);

    Serial.print("Pulse ");
    Serial.print(scanPulse);
    Serial.print(" | Distance ");
    Serial.println(cm);

    if (cm > 0 && cm <= STOP_DISTANCE_CM) {
      stopForever();
    }

    bool objectSeen = (cm > 0 && cm <= OBJECT_DISTANCE_THRESHOLD_CM);

    if (objectSeen) {

      if (!inObject) {
        inObject = true;
        objectStartPulse = scanPulse;
        nearestDistance = cm;
        peakStartPulse = scanPulse;
        peakEndPulse = scanPulse;
      }

      objectEndPulse = scanPulse;

      if (cm < nearestDistance - PEAK_TOLERANCE_CM) {
        nearestDistance = cm;
        peakStartPulse = scanPulse;
        peakEndPulse = scanPulse;
      }

      else if (cm <= nearestDistance + PEAK_TOLERANCE_CM) {
        peakEndPulse = scanPulse;
      }
    }

    else {

      if (inObject) {

        long width = objectEndPulse - objectStartPulse + 1;

        if (width >= MIN_OBJECT_PULSES) {

          long peakMid = (peakStartPulse + peakEndPulse) / 2;
          long pulsesBack = scanPulse - peakMid;

          Serial.println("OBJECT FOUND");

          spinLeftPulses(pulsesBack);

          Serial.println("CENTERED");

          return true;
        }

        inObject = false;
      }
    }

    delay(pauseTime);
  }

  Serial.println("NO OBJECT FOUND");
  return false;
}

// ---- Setup ----
void setup() {

  Serial.begin(9600);

  pinMode(LEFT_MOTOR_POWER, OUTPUT);
  pinMode(RIGHT_MOTOR_POWER, OUTPUT);
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);

  pinMode(LEFT_ENCODER, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER, INPUT_PULLUP);

  pinMode(FRONT_ULTRASONIC_TRIG, OUTPUT);
  pinMode(FRONT_ULTRASONIC_ECHO, INPUT);

  lastLeftState = digitalRead(LEFT_ENCODER);
  lastRightState = digitalRead(RIGHT_ENCODER);

  stopMotors();
  delay(5000);
}

// ---- Loop ----
void loop() {

  bool objectFound = scanForObjectAndCenterIfFound();

  if (objectFound) {

    moveForwardForTime(forwardTimeAfterDetection);

    delay(300);

    Serial.print("SPINNING LEFT ");
    Serial.print(leftResetPulses);
    Serial.println(" PULSES");

    spinLeftPulses(leftResetPulses);

    delay(300);
  }

  else {
    delay(300);
  }
}
