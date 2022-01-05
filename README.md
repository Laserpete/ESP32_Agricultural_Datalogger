# ESP32_Agricultural_Datalogger

I'm building the because I want to accurately measure the environmental parameters inside controlled growing environments that I've designed and built.

The Datalogger is capable of measuring;

=============================
------------------------------------------------

    |Parameter   |Sensor      |
    | ---------- | ----------:|
    |Temperature |SHT31       |
    |Humidity    |SHT31       |
    |Luminosity  |VEML7700    |
    |CO2         |MHZ16 NDIR  |

The sensors are accurate devices and suitable for use at high humidities (mushroom fruiting environments).
The case is waterproof and power is provided over USB cable or using 3*18650 batteries.
Bluetooth and WiFi connectivity are currently implemented on the device.

Logging to ThingSpeak is currently working - I like graphs. See [ThingSpeak.](https://thingspeak.com/channels/1370464)


