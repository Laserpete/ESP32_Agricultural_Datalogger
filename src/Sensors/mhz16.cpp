#include <Arduino.h>

#include "NDIR_I2C.h"
#include "Sensors.h"

#define CO2_SENSOR_I2C_ADDRESS 0x4D
#define CO2_SENSOR_STARTUP_DELAY 0  // 60000

NDIR_I2C CO2_Sensor(CO2_SENSOR_I2C_ADDRESS);

int CO2startTime;
bool CO2SensorReady = false;

void setupMHZ16(void) {
  if (CO2_Sensor.begin()) {
    Serial.println(F("CO2 sensor initializing..."));
    CO2startTime = millis();

    // delay(CO2_SENSOR_STARTUP_DELAY * 1000);
  } else {
    Serial.println(F("CO2 sensor not found."));
  }
}

int readCO2Level(void) {  // Check that the CO2 sensor had time to start up
  if (millis() >= (CO2startTime + CO2_SENSOR_STARTUP_DELAY)) {
    if (CO2SensorReady == false) {
      CO2SensorReady = true;
      // Serial.println("CO2 Sensor ready");
    }
  }
  if (CO2SensorReady == true) {
    if (CO2_Sensor.measure() == 0) {
      CO2_Sensor.begin();
    }
    return CO2_Sensor.ppm;
  } else {
    // Serial.print("CO2 sensor warming up, wait ");
    Serial.print(((CO2startTime + CO2_SENSOR_STARTUP_DELAY) - millis()) / 1000);
    // Serial.println(" Seconds");
    return 0;
  }
}
void printCO2Level(void) {
  Serial.print("CO2 Level   = ");
  Serial.print(readCO2Level());
  Serial.println(" ppm");
}