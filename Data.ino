void LoadData(){
  LoadLevel();
  LoadValuation();
  LoadLevelThreshold();
  LoadInvertation();
  LoadAP();
}
void LoadLevel() {
  calibrationX = readIntFromEEPROM(EEPROM_LEVEL_X);
  calibrationY = readIntFromEEPROM(EEPROM_LEVEL_Y);

  if (calibrationX == 0xffff || calibrationY == 0xffff) {
    calibrationX = -150;
    calibrationY = -125;
    StoreLevel();
  }
  Serial.print(F("Loaded X: "));
  Serial.print(calibrationX);
  Serial.print(F(" Y: "));
  Serial.println(calibrationY);
}

void LoadValuation() {
  valuationX = readIntFromEEPROM(EEPROM_VALUATION_X);
  valuationY = readIntFromEEPROM(EEPROM_VALUATION_Y);

  if (valuationX == 0xffff || valuationY == 0xffff) {
    valuationX = 271;
    valuationY = 267;
    StoreLevelValuation();
  }
  Serial.print(F("Loaded Valuation: "));
  Serial.print(F(" X: "));
  Serial.print(valuationX);
  Serial.print(F(" Y: "));
  Serial.println(valuationY);
}
void LoadLevelThreshold() {
  levelThreshold = EEPROM.read(EEPROM_LEVEL_THRESHOLD);
  if (levelThreshold == 0xff || levelThreshold == 0)
    levelThreshold = 10;
  Serial.print(F("Loaded Threshold: "));
  Serial.println(levelThreshold);
}
void LoadInvertation() {
  invertAxis = EEPROM.read(EEPROM_LEVEL_INVERTATION);
  Serial.print(F("Loaded Inverted Axis: "));
  Serial.println(invertAxis);
}

void LoadAP() {
  useAcessPointMode = EEPROM.read(EEPROM_LEVEL_ACCESSPOINT);
  Serial.print(F("Loaded AccessPoint: "));
  Serial.println(useAcessPointMode);
}

void StoreLevel() {
  writeIntIntoEEPROM(EEPROM_LEVEL_X, calibrationX);
  writeIntIntoEEPROM(EEPROM_LEVEL_Y, calibrationY);
  EEPROM.commit();
}

void StoreLevelValuation() {
  writeIntIntoEEPROM(EEPROM_VALUATION_X, valuationX);
  writeIntIntoEEPROM(EEPROM_VALUATION_Y, valuationY);
  EEPROM.commit();
}
void StoreLevelThreshold() {
  EEPROM.write(EEPROM_LEVEL_THRESHOLD, levelThreshold);
  EEPROM.commit();
}
void StoreInvertation() {
  EEPROM.write(EEPROM_LEVEL_INVERTATION, invertAxis);
  EEPROM.commit();
}

void StoreAP() {
  EEPROM.write(EEPROM_LEVEL_ACCESSPOINT, useAcessPointMode);
  EEPROM.commit();
}
