#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H

#include <BluetoothSerial.h>

#include "header.h"

BluetoothSerial SerialBT;

void setupBluetooth() {
  SerialBT.begin("ESP32Test");
  Serial.println("Bluetooth started, try to connect.");
}

void writeToBluetoothSerial(SensorValues receivedValues) {
  SerialBT.print("Probe Temperature = ");
  SerialBT.print(receivedValues.SHT31probeTemp);
  SerialBT.println(" Celcius");
  SerialBT.print("Probe Humidity = ");
  SerialBT.print(receivedValues.SHT31probeHumidity);
  SerialBT.println(" RH");
  SerialBT.print("CO2 Level = ");
  SerialBT.print(receivedValues.cO2Level);
  SerialBT.println(" PPM");
  SerialBT.print("Luminosity =  ");
  SerialBT.print(receivedValues.luminosity);
  SerialBT.println(" Lux");
  SerialBT.println();
}

void BTtoUART() {
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}

#endif