#include "followwall.h"
#include <Arduino.h>
#include "pins.h"
#include "standby.h"
#include "calibration.h"

FollowWall::FollowWall() {
  Standby();
  running = false;
  target = 20;
}

void FollowWall::loop() {
  if (!running) return;

  digitalWrite(LEFT_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(LEFT_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(LEFT_ULTRASONIC_TRIG, LOW);

  float dist = (pulseIn(LEFT_ULTRASONIC_ECHO, HIGH) / 2) / 29.1;
  float diff = (target - dist) / target;

  digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
  digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
  digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
  digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);

  if (diff > 0) {
    diff = diff > 0.5 ? 0.5 : diff;
    analogWrite(LEFT_MOTOR_POWER, (float) Calibration::leftMotorMaximum * (0.5 * (1.0 + diff)));
    analogWrite(RIGHT_MOTOR_POWER, (float) Calibration::rightMotorMaximum * 0.5);
  } else {
    diff = diff < -0.5 ? -0.5 : diff;
    analogWrite(LEFT_MOTOR_POWER, (float) Calibration::leftMotorMaximum * 0.5);
    analogWrite(RIGHT_MOTOR_POWER, (float) Calibration::rightMotorMaximum * (0.5 * (1.0 - diff)));
  }
}

void FollowWall::event(uint8_t * data, size_t len) {
  if (*data == 0x00) {
    analogWrite(LEFT_MOTOR_POWER, 0);
    analogWrite(RIGHT_MOTOR_POWER, 0);
    running = false;
  } else if (*data & 0x80) {
    target = *data & 0x7F;
    running = true;
  }
}