#include "standby.h"
#include <Arduino.h>
#include "pins.h"

Standby::Standby() {
  digitalWrite(LEFT_MOTOR_POWER, LOW);
  digitalWrite(RIGHT_MOTOR_POWER, LOW);
}
