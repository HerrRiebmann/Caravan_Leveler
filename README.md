# Caravan Leveler
ESP32 digital water level bubble with an ADXL345 Accelerometer

![Caravan Leveler](/Images/Caravan_Leveler.jpg)

An ESP WROOM32 will create a hotspot with a captive portal.
This captive portal will force the phone to open up a login-site (_which represents the water level_).

## Contents
* [Usage](#usage)
* [Video](#video)
* [3D Case](#3D-Case)
* [Things](#Things)
* [HardwareComponents](#HardwareComponents)
  * [Wiring](#Wiring)
  * [SPIFFS Upload](#SPIFFS-Upload)
  * [Libraries](#Libraries)
  * [OTA (Over the Air Update)](#OTA-Over-the-Air-Update)
* [Compatibility](#Compatibility)
* [Disclaimer](#Disclaimer)

### Intention
The intention was, to attach this to my caravan (_Knaus Sport & Fun_) and provide it with 12V on-board voltage.
When arriving at a campingside, the phone will automatically connect to this as a known hotspot, open up the digital leveler and I can happily crank the supports.

In short: Lazy me doesn´t want to constantly walk to the inside and check the level &#129335;

## Usage

Captive Portal (_Android 10_):

![Captive Portal](/Images/Screenshot_01.jpg)

**Calibrate** sets the values to zero

Settings:

![Settings](/Images/Screenshot_02.jpg)

**Threshold** describes the water bubble maximum value, when it will reach the scales end

**Invert Axis** swaps X- and Y-Axis

**Use Accesspoint** or connect to an existing WiFi (_SSID and Pasword required in code_)

**Valutation** represents the values range from 0° to 90° (_or -90° to 0°_)

**Rec** starts recording the minimum and maximum values on both axis. Tip the board over to 90 degrees on every side. Afterwards, hit **End** to see those values.

**Save** will store all values above, to the ESP32

**Upload** loads a file to the SPIFFS. To overwrite existing files, the filename must be equal (see [/data](https://github.com/HerrRiebmann/Caravan_Leveler/tree/main/data))!

## Video
[![](http://img.youtube.com/vi/iUhjaghWvkY/0.jpg)](http://www.youtube.com/watch?v=iUhjaghWvkY "Caravan Leveler")

## 3D Case
3D Printed case with wood filament:

![Captive Portal](/Images/3D%20Wood%20Case.jpg)

**Attention!** The wood filament comes thicker, so you might have to sand parts of the case. That´s not necesary using regular PLA.


Hotspot (_Android 4.4.3_):

![Hotspot](/Images/Hotspot.jpg)

## Things
### HardwareComponents
* ESP WROOM32
* ADXL345 (_Accelerometer / Gyro_)
* LM2596 (_DC-DC converter_)
* Rocker switch (_15mm x 10mm_)
* 3D Printed Case

### Wiring
ESP 32 | ADXL345
------- | --------
G22 | SCL
G21 | SDA
3.3V | VCC
GND | GND

ESP 32 | LM2596
------- | --------
5V | OUT+
GND | OUT-

### SPIFFS Upload
To upload the HTML, JS and CSS files, I´ve used the [Arduino ESP32 filesystem uploader](https://github.com/me-no-dev/arduino-esp32fs-plugin)
You can find the latest release [here](https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/) and a tutorial on [RandomNerdTutorials](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/)

### Libraries
* Adafruit Unified Sensor (1.0.3)
* Adafruit ADXL345 (1.2.2)

### OTA (Over the Air Update)
You should see the ESP32 in Arduino IDE under Tools -> Port -> Network-Interfaces (Sport&Fun Leveler at _IP-Adress_)
For more information see [RandomNerdTutorials](https://randomnerdtutorials.com/esp32-over-the-air-ota-programming/)


## Compatibility
Tested on iOS and Android 4.4.3, 8 and 10.

(*And all estabished none-mobile Browsers, like Firefox, Edge or Chrome*)

### Disclamer
The ADXL345 is **not** a proper device to show exact degrees! It´s an accelerometer, which goal was to measure movements by gravitation.
So all results are just approximation.
