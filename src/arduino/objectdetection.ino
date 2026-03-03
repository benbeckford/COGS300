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
int leftSpinSpeed  = 80;
int rightSpinSpeed = 100;

int spinTime  = 100;   // how long it spins each tick (ms)
int pauseTime = 300;   // how long it pauses (ms) ← adjust this

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

// ---- Setup ----
void setup() {
  Serial.begin(9600);

  pinMode(LEFT_MOTOR_POWER, OUTPUT);
  pinMode(RIGHT_MOTOR_POWER, OUTPUT);
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);

  pinMode(FRONT_ULTRASONIC_TRIG, OUTPUT);
  pinMode(FRONT_ULTRASONIC_ECHO, INPUT);
}

// ---- Loop ----
void loop() {

  // 1️⃣ Spin
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);

  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);

  analogWrite(LEFT_MOTOR_POWER, leftSpinSpeed);
  analogWrite(RIGHT_MOTOR_POWER, rightSpinSpeed);

  delay(spinTime);

  // 2️⃣ Stop
  stopMotors();

  // 3️⃣ Measure distance
  long cm = readUltrasonicCM(FRONT_ULTRASONIC_TRIG, FRONT_ULTRASONIC_ECHO);

  Serial.print("Front cm: ");
  Serial.println(cm);

  // 4️⃣ Pause before next tick
  delay(pauseTime);
}
