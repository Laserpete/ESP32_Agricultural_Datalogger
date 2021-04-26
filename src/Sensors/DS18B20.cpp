#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#include "Sensors.h"

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperatureProbe(&oneWire);

void setupDS18B20() {
  temperatureProbe.begin();
  Serial.println("Temperature probe ready");
}

float readDS18B20Temp(void) {
  temperatureProbe.requestTemperatures();
  return temperatureProbe.getTempCByIndex(0);
}
void printDS18B20Temp(void) {
  temperatureProbe.requestTemperatures();
  float tempC = temperatureProbe.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature Probe = ");
    Serial.print(tempC);
    Serial.println(" C");
  } else {
    Serial.println("ERROR: Could not read Temperature Probe");
  }
}