#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_SHT31.h"
#include "Sensors.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setupSHT31(void) {
  sht31.begin(0x44);
  Serial.println("SHT31 setup");
}

float readSHT31Temperature(void) {
  float f;
  return f = sht31.readTemperature();
}

float readSHT31Humidity(void) {
  float h;
  return h = sht31.readHumidity();
}

void printSHT31(void) {
  Serial.print("SHT31 Temp = ");
  Serial.print(sht31.readTemperature());
  Serial.print(" Humidity = ");
  Serial.println(sht31.readHumidity());
}