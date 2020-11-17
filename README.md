# Caravan Leveler
ESP32 digital water level bubble with an ADXL345 Accelerometer



An ESP WROOM32 will create a hotspot with a captive portal.
This captive portal will force the phone to open up a login-site (_which represents the water level_).

## Compatibility
Tested on iOS and Android 4.4.3, 8 and 10.


### Intention
The intention was, to attach this to my caravan (_Knaus Sport & Fun_) and provide it with 12V on-board voltage.
When arriving at a campingside, the phone will automatically connect to this as a known hotspot, open up the digital leveler and I can happily crank the supports.

In short: Lazy me doesn´t want to constantly walk to the inside and check the level &#129335;

## Components
* ESP WROOM32
* ADXL345 (_Accelerometer / Gyro_)
* LM2596 (_DC-DC converter_)
* Rocker switch (_15mm x 10mm_)
* 3D Printed Case

## Wiring
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


## Images

Captive Portal (_Android 10_):

![Captive Portal](/Images/Screenshot_01.jpg)

**Calibrate** sets the values to zero

Settings:

![Settings](/Images/Screenshot_02.jpg)

**Threshold** describes the water bubble maximum value, when it will reach the scales end

**Invert Axis** swaps X- and Y-Axis

**Valutation** represents the values range from 0° to 90° (_or -90° to 0°_)


3D Printed case with wood filament:

![Captive Portal](/Images/3D%20Wood%20Case.jpg)

**Attention!** The wood filament comes thicker, so you might have to sand parts of the case. That´s not necesary using regular PLA.


Hotspot (_Android 4.4.3_):

![Hotspot](/Images/Hotspot.jpg)

## Disclamer
The ADXL345 is **not** a proper device to show exact degrees! It´s an accelerometer, which goal was to measure movements by gravitation.
So all results are just approximation.
