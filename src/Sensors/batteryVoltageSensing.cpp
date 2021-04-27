#include <Arduino.h>

#include "Sensors.h"

void measureBatteryVoltage() {
  float voltage =
      analogRead((VOLTAGE_MEASURE_PIN)*5) * ((10 + 1.5) / 1.5) / 1024;
  voltage = voltage * (voltage * 0.08);
  Serial.print("Current battery voltage = ");
  Serial.print(voltage);
  Serial.println(" V");
  if (voltage >= 3.7) {
    Serial.println("Battery is fully charged");
  }
}