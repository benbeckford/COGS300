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

int pauseTime = 300;

// ---- Encoder Variables ----
long leftCount = 0;
long rightCount = 0;

int lastLeftState = LOW;
int lastRightState = LOW;

// ---- Ultrasonic Function ----
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

// ---- Start Spin ----
void startSpin() {
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);

  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);

  analogWrite(LEFT_MOTOR_POWER, leftSpinSpeed);
  analogWrite(RIGHT_MOTOR_POWER, rightSpinSpeed);
}

// ---- Update Encoder Counts ----
void updateEncoders() {
  int leftState = digitalRead(LEFT_ENCODER);
  int rightState = digitalRead(RIGHT_ENCODER);

  if (leftState == HIGH && lastLeftState == LOW) {
    leftCount++;
  }

  if (rightState == HIGH && lastRightState == LOW) {
    rightCount++;
  }

  lastLeftState = leftState;
  lastRightState = rightState;
}

// ---- Spin Until RIGHT Encoder Increases By 1 ----
void spinUntilNextRightEncoder() {
  long startRightCount = rightCount;

  startSpin();

  while (true) {
    updateEncoders();

    if (rightCount > startRightCount) {
      break;
    }
  }

  stopMotors();
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

  pinMode(LEFT_ENCODER, INPUT);
  pinMode(RIGHT_ENCODER, INPUT);

  pinMode(FRONT_ULTRASONIC_TRIG, OUTPUT);
  pinMode(FRONT_ULTRASONIC_ECHO, INPUT);

  lastLeftState = digitalRead(LEFT_ENCODER);
  lastRightState = digitalRead(RIGHT_ENCODER);

  delay(5000);   // startup delay
}

// ---- Loop ----
void loop() {
  // Spin until RIGHT encoder increases by one
  spinUntilNextRightEncoder();

  // Measure distance
  long cm = readUltrasonicCM(FRONT_ULTRASONIC_TRIG, FRONT_ULTRASONIC_ECHO);

  // Print encoder + ultrasonic data
  Serial.print("Left Encoder: ");
  Serial.print(leftCount);
  Serial.print(" | Right Encoder: ");
  Serial.print(rightCount);
  Serial.print(" | Front cm: ");
  Serial.println(cm);

  // Pause before next spin
  delay(pauseTime);
}
