#include "calibrateforwards.h"
#include <Arduino.h>
#include "pins.h"
#include "standby.h"
#include "calibration.h"

CalibrateForwards::CalibrateForwards() {
  Standby();
}

void CalibrateForwards::event(uint8_t * data, size_t len) {
  switch (data[0]) {
    case 0x00:
      analogWrite(LEFT_MOTOR_POWER, 0);
      analogWrite(RIGHT_MOTOR_POWER, 0);
      return;
    case 0x11:
      if (len == 3) {
        Calibration::leftMotorMaximum = data[1];
        Calibration::rightMotorMaximum = data[2];
      }

      return;
    case 0xFF:
      if (len == 3) {
        digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
        digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
        analogWrite(LEFT_MOTOR_POWER, data[1]);

        digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
        analogWrite(RIGHT_MOTOR_POWER, data[2]);
      }

      return;
    default:
      break;
  }
}