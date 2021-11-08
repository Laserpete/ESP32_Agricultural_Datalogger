#include <Arduino.h>

#include "Sensors.h"

void measureBatteryVoltage() {
  // Voltage = analog read value * batttery voltage * ((R1+R2) / R2) / 1024
  float voltage = ((analogRead(VOLTAGE_MEASURE_PIN) * 3.65) * 2) / 4095;
  // voltage = voltage * (voltage * 0.08);
  Serial.print("Current battery voltage = ");
  Serial.print(voltage);
  Serial.println(" V");
  if (voltage >= 3.7) {
    Serial.println("Battery is fully charged");
  }
}