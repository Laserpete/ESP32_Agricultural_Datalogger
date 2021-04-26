#include <Arduino.h>
#include <HTU21D.h>

#include "Sensors.h"

HTU21D htu21d(HTU21D_RES_RH12_TEMP14);

void setupHTU21D() {
  while (htu21d.begin() != true) {
    Serial.println("HTU21D not found");
  }
  Serial.println("HTU21D ready");
}

void printHTU21D() {
  Serial.print("Humidity    = ");
  Serial.print(htu21d.readCompensatedHumidity());
  Serial.println(" % RH");
  Serial.print("Temperature = ");
  Serial.print(htu21d.readTemperature());
  Serial.println(" C");
  if (htu21d.readCompensatedHumidity() >= 99) {
    htu21d.setHeater(HTU21D_ON);
    Serial.println("HTU21D Heater on");
  } else {
    htu21d.setHeater(HTU21D_OFF);
  }
}

float readHTU21dTemperature(void) { return htu21d.readTemperature(); }

float readHTU21dHumidity(void) {
  if (htu21d.readCompensatedHumidity() >= 99) {
    htu21d.setHeater(HTU21D_ON);
    Serial.println("HTU21D Heater on");
  } else {
    htu21d.setHeater(HTU21D_OFF);
  }
  return htu21d.readCompensatedHumidity();
}