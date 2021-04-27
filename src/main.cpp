/* PLAN

Sensors
  HTU21D
  BME280
  MHZ16 NDIR CO2
  VMEL7700
  DS18B20

Processing
  Comma separated Values output
  JSON output

Logging
  Send to ThingSpeak
  Value averaging
  Ignore obviously wrong values


Connectivity
  WiFi
  Bluetooth Serial
  OTA updates

Power management
  Battery voltage sensing (on Pin IO35)
  Sleep mode between measurements

*/

/* Problems

  Reading twice, somehow ignoring if statement
  Inside temperature is higher than ambient, giving false temperature and
  humidity readings

*/

#include <Arduino.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include "Sensors/Sensors.h"
#include "i2cScanner.h"
#include "keys.h"

#define NTP_OFFSET 2 * 60 * 60
#define NTP_INTERVAL 60 * 1000
#define NTP_ADDRESS "0.pool.ntp.org"

#define MEASUREMENT_MINUTES_MODULO 5

#define countof(a) (sizeof(a) / sizeof(a[0]))

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

const char* thingSpeakServerName = "http://api.thingspeak.com/update";

void setupWiFi() {
  int wifiTimer = millis();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected in: ");
  Serial.print(millis() - wifiTimer);
  Serial.println(" mS");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Wire.begin(21, 22);

  // Scanner();
  setupSensors();
  setupWiFi();
}

void printSensorDataCSV() {
  float temperature = readHTU21dTemperature();
  float humidity = readHTU21dHumidity();
  int CO2Level = readCO2Level();
  float probeTemp = readDS18B20Temp();
  int luminosity = readVeml7700();

  char temperatureString[5];
  char humidityString[5];
  char CO2String[7];
  char probeTempString[5];
  char luminosityString[7];

  dtostrf(temperature, 3, 1, temperatureString);
  dtostrf(humidity, 4, 1, humidityString);
  dtostrf(CO2Level, 5, 0, CO2String);
  dtostrf(probeTemp, 3, 1, probeTempString);
  dtostrf(luminosity, 5, 0, luminosityString);

  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  time_t rawtime = timeClient.getEpochTime();
  struct tm ts;

  char dateStamp[11];
  char timeStamp[9];

  ts = *localtime(&rawtime);

  strftime(dateStamp, sizeof(dateStamp), "%Y-%m-%d", &ts);
  strftime(timeStamp, sizeof(timeStamp), "%H:%M:%S", &ts);

  char commaSeparatedValuesOutput[80];

  snprintf_P(commaSeparatedValuesOutput, countof(commaSeparatedValuesOutput),
             PSTR("%s, %s, %s, %s, %s, %s, %s"), dateStamp, timeStamp,
             temperatureString, humidityString, CO2String, probeTempString,
             luminosityString);
  Serial.println();
  Serial.println(commaSeparatedValuesOutput);
  measureBatteryVoltage();
  Serial.println();
}

typedef struct _SensorValues {
  float temperature;
  float humidity;
  int cO2Level;
  float probeTemp;
  int luminosity;
  // float internalHumidity;
  // float internalTemperature;
} SensorValues;

SensorValues getSensorValues() {
  SensorValues getValuesStruct = {.temperature = readHTU21dTemperature(),
                                  .humidity = readHTU21dHumidity(),
                                  .cO2Level = readCO2Level(),
                                  .probeTemp = readDS18B20Temp(),
                                  .luminosity = readVeml7700()};
  return getValuesStruct;
}

void sendDataToThingSpeak(String thingSpeakPostString) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected");
    return;
  }
  HTTPClient http;
  // Domain name
  http.begin(thingSpeakServerName);
  // Specify Content Type Header
  http.addHeader("Content-Type", "application/json");

  // Send HTTP POST request
  int httpResponseCode = http.POST(thingSpeakPostString);
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  http.end();
  Serial.println("Data sent");
  printSensorDataCSV();
}

void logValues(SensorValues receivedValues) {
  String thingSpeakPostString =
      "{\"api_key\":\"" + apiKey + "\",\"field1\":\"" +
      receivedValues.temperature + "\",\"field2\":\"" +
      receivedValues.humidity + "\",\"field3\":\"" + receivedValues.cO2Level +
      "\",\"field4\":\"" + receivedValues.probeTemp + "\",\"field5\":\"" +
      receivedValues.luminosity + "\"}";

  sendDataToThingSpeak(thingSpeakPostString);
}

bool shouldLog() {
  timeClient.forceUpdate();
  int currentTimeClientMinutes = timeClient.getMinutes();
  Serial.print("Modulo of minutes = ");
  Serial.println(currentTimeClientMinutes % MEASUREMENT_MINUTES_MODULO);

  return ((currentTimeClientMinutes % MEASUREMENT_MINUTES_MODULO == 0) &&
          (timeClient.getSeconds() == 0));
}

void loop() {
  // printHTU21D();
  // printBME280();
  // printVeml7700();
  // printCO2Level();
  // printDS18B20Temp();
  // printCSV();
  // int minutesModuloValue = timeClient.getMinutes() %
  // MEASUREMENT_MINUTES_MODULO;

  // if (minutesModuloValue == 0) {
  // sendValuesToThingSpeak();
  // }
  // choose values to log

  if (shouldLog()) {
    logValues(getSensorValues());
  }

  // Serial.println();
  // delay(10000);
}