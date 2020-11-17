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
  if (!EEPROM.begin(10))
    Serial.println("An Error has occurred while initializing EEPROM");
}

boolean isIp(String str) {
  Serial.println("IsIP");
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
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
