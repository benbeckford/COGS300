#include "firmware.h"
#include "config.h"
#include "WiFi.h"

unsigned long last_connection = 0;

void setup() {
  firmware_setup();
}

void loop() {
  if (WiFi.status() == WL_DISCONNECTED && millis() - last_connection >= 10000) {
    last_connection = millis();
    WiFi.begin("ubcsecure", WPA2_AUTH_PEAP, CWL_USERNAME, CWL_USERNAME, CWL_PASSWORD);
    SERIAL_AT.println("Disconnected from ubcsecure, retrying...");
  }
  
  firmware_loop();
}