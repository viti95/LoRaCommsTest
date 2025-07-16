#include <Arduino.h>
#include "rcmonitor.h"            // https://github.com/aollin/racechrono-ble-diy-device
#include <WiFi.h>
#include <HTTPClient.h>

struct strmon monitors[] = {
    {"Altitud", "channel(device(gps), altitude)", 1.0}
};

void setup() {
  Serial.begin(115200);
  rcmonitorstart();
}


void loop() {
  static float value;
  if( rcmonitor() ) {
    value   = (monitorValues[0] * monitorMultipliers[0]);
    Serial.println(value);
  }
  delay(2000);
}
