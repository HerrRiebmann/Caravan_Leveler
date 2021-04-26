//Gyro ADXL345
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

//Filesystem
#include "SPIFFS.h"
#include "EEPROM.h"
#define EEPROM_LEVEL_X 0
#define EEPROM_LEVEL_Y 2
#define EEPROM_VALUATION_X 4
#define EEPROM_VALUATION_Y 6
#define EEPROM_LEVEL_THRESHOLD 8
#define EEPROM_LEVEL_INVERTATION 9
#define EEPROM_LEVEL_ACCESSPOINT 10

//Webserver
#include <WiFi.h>
#include <WebServer.h>
const char* ssid = "FindMichDoch2";
const char* password = "5AF714A8B";

WebServer webServer(80);

#include <DNSServer.h>
const byte DNS_PORT = 53;
DNSServer dnsServer;

//Over the Air Update
#include <ArduinoOTA.h>

const char deviceName[] = "Sport&Fun Leveler";

bool accelInitialized = false;
int levelX = 0;
int levelY = 0;
int calibrationX = -150;
int calibrationY = -25;
int valuationX = 271;
int valuationY = 267;
bool valuationActive = false;
uint8_t levelThreshold = 5;
bool invertAxis = false;
bool useAcessPointMode = false;

long lastMillis = 0;
long lastMillisClientAvailable = 0;

void setup() {
  SerialBegin();
  Adxl345Begin();
  EepromBegin();  

  LoadData();

  SpiffsBegin();
  WiFiBegin();
  setupOTA();
}

void loop() {
  //WebServer
  webServer.handleClient();

  //DNS
  dnsServer.processNextRequest();

  //OTA
  ArduinoOTA.handle();
  
  if(millis() - lastMillis > 200){
    //Only update when someone is listening:
    if(millis() - lastMillisClientAvailable < 1000)
      getLevel();
    lastMillis = millis();
  }
}

