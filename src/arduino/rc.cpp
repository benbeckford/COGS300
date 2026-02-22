#include "rc.h"
#include <Arduino.h>
#include "pins.h"
#include "standby.h"

RemoteControl::RemoteControl() {
  Standby();
}

void RemoteControl::event(uint8_t * data, size_t len) {
  if (len != 2) return;

  uint8_t motor = *data & 0xF0;
  uint8_t dir = *data & 0xF;

  digitalWrite(motor ? RIGHT_MOTOR_FORWARD : LEFT_MOTOR_FORWARD, dir ? HIGH : LOW);
  digitalWrite(motor ? RIGHT_MOTOR_BACKWARD : LEFT_MOTOR_BACKWARD, dir ? LOW : HIGH);
  analogWrite(motor ? RIGHT_MOTOR_POWER : LEFT_MOTOR_POWER, data[1]);
}
