#include <Adafruit_VEML7700.h>
#include <Arduino.h>

#include "Sensors.h"

Adafruit_VEML7700 veml7700;

void setupVeml7700() {
  if (!veml7700.begin()) {
    Serial.println("VEML7700 not found");
  }
  if (veml7700.begin()) {
    delay(3);
    veml7700.setIntegrationTime(VEML7700_IT_100MS);
    veml7700.setGain(VEML7700_GAIN_1_8);
    veml7700.setLowThreshold(0);
    veml7700.setHighThreshold(20000);
    veml7700.interruptEnable(false);
    Serial.println("VEML7700 ready");
  }
}
void printVeml7700() {
  int lux = veml7700.readLux();
  Serial.print("Luminosity = ");
  Serial.print(lux);
  Serial.println(" Lux");
}

int readVeml7700() { return veml7700.readLux(); }
