#include "followline.h"
#include <Arduino.h>
#include "pins.h"
#include "standby.h"
#include "calibration.h"

FollowLine::FollowLine() {
  Standby();
  running = false;
  baseSpeed = 150;
  correction = 60;
}

void FollowLine::loop() {
  if (!running) return;

  int leftSensor = digitalRead(LEFT_INFRARED);
  int rightSensor = digitalRead(RIGHT_INFRARED);

  if (leftSensor == LOW & rightSensor == LOW) {
    analogWrite(LEFT_MOTOR_FORWARD, baseSpeed);
    analogWrite(RIGHT_MOTOR_FORWARD, baseSpeed);
  }
  else if (leftSensor == LOW && rightSensor == HIGH) {
    analogWrite(LEFT_MOTOR_FORWARD, baseSpeed - correction);
    analogWrite(RIGHT_MOTOR_FORWARD, baseSpeed + correction);
  }
  else if (leftSensor == HIGH && rightSensor == LOW) {
    analogWrite(LEFT_MOTOR_FORWARD, baseSpeed + correction);
    analogWrite(RIGHT_MOTOR_FORWARD, baseSpeed - correction);
  }

  else {
    Standby();
  }
}

void FollowLine::event(uint8_t * data, size_t len) {
  if (*data == 0x00) {
    analogWrite(LEFT_MOTOR_POWER, 0);
    analogWrite(RIGHT_MOTOR_POWER, 0);
    running = false;
  } else if (*data & 0x80) {
    running = true;
  }
}
