#define VOLTAGE_MEASURE_PIN 35

void setupSensors(void);

void setupHTU21D(void);
float readHTU21dTemperature(void);
float readHTU21dHumidity(void);
void printHTU21D(void);

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

void measureBatteryVoltage(void);

void setupSHT31(void);
float readSHT31Temperature(void);
float readSHT31Humidity(void);
void printSHT31(void);