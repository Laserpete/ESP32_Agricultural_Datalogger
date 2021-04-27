#include <Arduino.h>

#include "Sensors.h"

void setupSensors() {
  setupHTU21D();
  setupBME280();
  setupVeml7700();
  setupMHZ16();
  setupDS18B20();
  pinMode(VOLTAGE_MEASURE_PIN, INPUT);
}