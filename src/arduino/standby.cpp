#include "standby.h"
#include <Arduino.h>
#include "pins.h"

Standby::Standby() {
  digitalWrite(LEFT_MOTOR_POWER, 0);
  digitalWrite(RIGHT_MOTOR_POWER, 0);
}
