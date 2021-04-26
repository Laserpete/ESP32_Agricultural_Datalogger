#include <Arduino.h>

#include "Adafruit_BME280.h"
#include "Sensors.h"

Adafruit_BME280 bme280;

void setupBME280(void) {
  if (!bme280.begin(0x76, &Wire)) {
    Serial.println("BME280 not detected");
    // while (1)
    ;
  } else {
    Serial.println("BME280 ready");
  }
}

float readBME280Temperature() { return bme280.readTemperature(); }

int readBME280Humidity() { return bme280.readHumidity(); }

void printBME280(void) {
  bme280.takeForcedMeasurement();
  Serial.print("BME280 Temp = ");
  Serial.println(bme280.readTemperature());
  Serial.print("BME280 Humid = ");
  Serial.println(bme280.readHumidity());
}