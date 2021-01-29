void SerialBegin() {
  Serial.begin(115200);
  Serial.println("Start Leveler");
  Serial.println("");
}

void Adxl345Begin() {
  accelInitialized = accel.begin();
  if (!accelInitialized)
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
  else
    accel.setRange(ADXL345_RANGE_2_G);
}

void SpiffsBegin() {
  if (!SPIFFS.begin(true))
    Serial.println("An Error has occurred while mounting SPIFFS");
}

void EepromBegin() {
  if (!EEPROM.begin(11))
    Serial.println("An Error has occurred while initializing EEPROM");
}

boolean isIp(String str) {
  Serial.print("-IsIP: ");
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      Serial.println("false");
      return false;
    }
  }
  Serial.println("true");
  return true;
}

bool ProcessETag(const char* ETag) {
  for (int i = 0; i < webServer.headers(); i++) {
    if (webServer.headerName(i).compareTo(F("If-None-Match")) == 0)
      if (webServer.header(i).compareTo(ETag) == 0) {
        webServer.send(304, "text/plain", F("Not Modified"));
        Serial.println(String(F("\t")) + webServer.headerName(i) + F(": ") + webServer.header(i));        
        return true;
      }
  }
  webServer.sendHeader("ETag", ETag);
  webServer.sendHeader("Cache-Control", "public");
  return false;
}

void ProcessSetupArguments() {
  // /setup?x=123&y=321&inv=0&ap=1&t=10

  bool valutationChanged = false;

  for (uint8_t i = 0; i < webServer.args(); i++) {
    Serial.println(String(F(" ")) + webServer.argName(i) + F(": ") + webServer.arg(i));

    if (webServer.argName(i).compareTo(F("vx")) == 0) {
      valuationX = webServer.arg(i).toInt();
      valutationChanged = true;
    }

    if (webServer.argName(i).compareTo(F("vy")) == 0) {
      valuationY = webServer.arg(i).toInt();
      valutationChanged = true;
    }

    if (webServer.argName(i).compareTo(F("inv")) == 0) {
      invertAxis = webServer.arg(i) == "1";
      StoreInvertation();
    }

    if (webServer.argName(i).compareTo(F("ap")) == 0) {
      useAcessPointMode = webServer.arg(i) == "1";
      StoreAP();
    }

    if (webServer.argName(i).compareTo(F("t")) == 0) {
      int j = webServer.arg(i).toInt();
      if (j > 0 && j <= 90) {
        levelThreshold = j;
        StoreLevelThreshold();
      }
    }
    if (valutationChanged)
      StoreLevelValuation();    
  }  
}

String toStringIp(IPAddress ip) {
  Serial.println("IptoString");
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

void writeIntIntoEEPROM(int address, int16_t number) {
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int16_t readIntFromEEPROM(int address) {
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}
