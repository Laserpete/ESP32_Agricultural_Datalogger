
typedef struct _SensorValues
{
  float internalTemperature;
  int internalHumidity;
  int cO2Level;
  float SHT31probeTemp;
  float SHT31probeHumidity;
  int luminosity;
} SensorValues;

void setupBluetooth();
void writeToBluetoothSerial(SensorValues);
void BTtoUART();