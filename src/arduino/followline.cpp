// ---- Pin Definitions ----
#define LEFT_MOTOR_POWER 6
#define LEFT_MOTOR_FORWARD 5
#define LEFT_MOTOR_BACKWARD 7

#define RIGHT_MOTOR_POWER 3
#define RIGHT_MOTOR_FORWARD 4
#define RIGHT_MOTOR_BACKWARD 2

#define LEFT_INFRARED   A4
#define RIGHT_INFRARED  A5
#define MIDDLE_INFRARED A3

int baseSpeed = 70;
int correction = 40;

void stopMotors() {
  analogWrite(LEFT_MOTOR_POWER, 0);
  analogWrite(RIGHT_MOTOR_POWER, 0);

  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
}

void forward(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  // Direction forward
  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);

  // Speed control
  analogWrite(LEFT_MOTOR_POWER, leftSpeed);
  analogWrite(RIGHT_MOTOR_POWER, rightSpeed);
}

void backward(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain (leftSpeed, 0, 255);
  rightSpeed = constrain (rightSpeed, 0, 255);

  digitalWrite(LEFT_MOTOR_FORWARD, LOW);
  digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
  digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);

  analogWrite(LEFT_MOTOR_POWER, leftSpeed);
  analogWrite(RIGHT_MOTOR_POWER, rightSpeed);
}

void setup() {
  pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
  pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
  pinMode(LEFT_MOTOR_POWER, OUTPUT);

  pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
  pinMode(RIGHT_MOTOR_POWER, OUTPUT);

  // Use pullups so the pins don't float
  pinMode(LEFT_INFRARED, INPUT_PULLUP);
  pinMode(MIDDLE_INFRARED, INPUT_PULLUP);
  pinMode(RIGHT_INFRARED, INPUT_PULLUP);

  stopMotors();
}

void loop() {
  int leftSensor   = digitalRead(LEFT_INFRARED);
  int middleSensor = digitalRead(MIDDLE_INFRARED);
  int rightSensor  = digitalRead(RIGHT_INFRARED);

  // LOW = tape detected, HIGH = no tape
  bool leftOnTape   = (leftSensor == LOW);
  bool middleOnTape = (middleSensor == LOW);
  bool rightOnTape  = (rightSensor == LOW);

  if (!leftOnTape && !middleOnTape && !rightOnTape) {
    backward(baseSpeed, baseSpeed);
  }
  else {
    // Steering priority: edges first
    if (leftOnTape && !rightOnTape) {
      // tape under left -> steer left
      forward(baseSpeed - correction, baseSpeed + correction);
    }
    else if (rightOnTape && !leftOnTape) {
      // tape under right -> steer right
      forward(baseSpeed + correction, baseSpeed - correction);
    }
    else {
      // middle only OR multiple sensors see tape -> go straight
      forward(baseSpeed, baseSpeed);
    }
  }

  delay(10);
}
