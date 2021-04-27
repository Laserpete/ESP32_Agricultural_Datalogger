#ifndef HTU21D_H
#define HTU21D_H

#define VOLTAGE_MEASURE_PIN 35

void setupSensors(void);

void setupHTU21D(void);
void printHTU21D(void);
float readHTU21dTemperature(void);
float readHTU21dHumidity(void);

void setupBME280(void);
float readBME280Temperature(void);
int readBME280Humidity(void);
void printBME280(void);

void setupMHZ16(void);
int readCO2Level(void);
void printCO2Level(void);

void setupVeml7700(void);
int readVeml7700(void);
void printVeml7700(void);

void setupDS18B20(void);
float readDS18B20Temp(void);
void printDS18B20Temp(void);

#endif