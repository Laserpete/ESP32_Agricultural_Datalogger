/* PLAN

Sensors
  HTU21D
  BME280
  MHZ16 NDIR CO2
  VMEL7700
  DS18B20

Formatting
  Comma separated Values output
  JSON output

Logging
  Send to ThingSpeak
  CRC
  Value averaging


Connectivity
  WiFi
  Bluetooth Serial
  OTA updates

Power management
  Battery voltage sensing (on Pin IO35)
  Sleep mode between measurements

*/

/* Issues


*/

#include <Arduino.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

#include "IIC/i2cScanner.h"
#include "Sensors/Sensors.h"
#include "header.h"
#include "keys.h"

#define DEBUG

#define NTP_OFFSET 2 * 60 * 60
#define NTP_INTERVAL 60 * 1000
#define NTP_ADDRESS "0.pool.ntp.org"

#define WIFI_CONNECT_SECONDS 10
#define MEASUREMENT_MINUTES_MODULO 1

#define countof(a) (sizeof(a) / sizeof(a[0]))

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menuconfig# to enable it'
#endif

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
const char *thingSpeakServerName = "http://api.thingspeak.com/update";
long long lastMillis = 0;
bool loggedToThingSpeak = 0;

bool setupWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  lastMillis = millis();
  WiFi.begin(ssid, password);
  while ((millis() <= (WIFI_CONNECT_SECONDS * 1000)) &&
         (WiFi.status() != WL_CONNECTED)) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Connected in ");
    Serial.print(millis() - lastMillis);
    Serial.println(" mS");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    timeClient.begin();
    timeClient.setTimeOffset(3600);
    return 1;
  } else {
    Serial.println();
    Serial.println("Wifi not connected");
    return 0;
  }
  lastMillis = 0;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Wire.begin(21, 22);
  setupBluetooth();
  IICScanner();
  setupSensors();
  setupWiFi();
  measureBatteryVoltage();
  Serial.println("Setup Finished");
  Serial.println();
}

SensorValues getSensorValues() {
  SensorValues getValuesStruct = {
      .internalTemperature = readBME280Temperature(),
      .internalHumidity = readBME280Humidity(),
      .cO2Level = readCO2Level(),
      .SHT31probeTemp = readSHT31Temperature(),
      .SHT31probeHumidity = readSHT31Humidity(),
      .luminosity = readVeml7700()};
  return getValuesStruct;
}

void printSensorDataCSV(SensorValues receivedSensorValues) {
  char temperatureString[5];
  char humidityString[5];
  char cO2String[7];
  char luminosityString[7];

  dtostrf(receivedSensorValues.SHT31probeTemp, 3, 1, temperatureString);
  dtostrf(receivedSensorValues.SHT31probeHumidity, 4, 1, humidityString);
  dtostrf(receivedSensorValues.cO2Level, 5, 0, cO2String);
  dtostrf(receivedSensorValues.luminosity, 5, 0, luminosityString);

  if (!timeClient.update()) {
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
             PSTR("%s, %s, %s, %s, %s, %s"), dateStamp, timeStamp,
             temperatureString, humidityString, cO2String, luminosityString);
  Serial.println();
  Serial.println(commaSeparatedValuesOutput);
  measureBatteryVoltage();
  Serial.println();
}

void postDataToThingSpeak(String thingSpeakPostString) {
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
  if (httpResponseCode == 200) {
    loggedToThingSpeak = 1;
  }
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  http.end();

  Serial.print("Data sent at ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  if (timeClient.getSeconds() == 0) {
    Serial.print("0");
  }
  Serial.println();
}

void logToThingSpeak(SensorValues receivedValues) {
  postDataToThingSpeak("{\"api_key\":\"" + apiKey + "\",\"field1\":\"" +
                       receivedValues.SHT31probeTemp + "\",\"field2\":\"" +
                       receivedValues.SHT31probeHumidity + "\",\"field3\":\"" +
                       receivedValues.cO2Level + "\",\"field4\":\"" +
                       receivedValues.luminosity + "\"}");
}

void logValues(SensorValues receivedValues) {
  if (WiFi.status() == WL_CONNECTED) {
    logToThingSpeak(receivedValues);
  }
// printSensorDataCSV(receivedValues);
#ifdef DEBUG
  printSHT31();
  printBME280();
  printCO2Level();
  printVeml7700();
  Serial.print("\n");
#endif
}

bool shouldLog() {
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.forceUpdate();
    if ((timeClient.getSeconds() >= 5) && (loggedToThingSpeak == 1)) {
      loggedToThingSpeak = 0;
    }
    int currentTimeClientMinutes = timeClient.getMinutes();
    if ((currentTimeClientMinutes % MEASUREMENT_MINUTES_MODULO == 0) &&
        (timeClient.getSeconds() == 0) && !loggedToThingSpeak) {
      return 1;
    } else {
      return 0;
    }

  } else {
    if (millis() >= (lastMillis + (MEASUREMENT_MINUTES_MODULO * 60 * 1000))) {
      lastMillis = millis();
      return 1;
    } else
      return 0;
  }
}

void loop() {
  BTtoUART();
  if (shouldLog()) {
    SensorValues currentValues = getSensorValues();
    logValues(currentValues);
    writeToBluetoothSerial(currentValues);
  }
}