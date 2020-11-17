void getLevel() {
  if (!accelInitialized)
    return;

  /*
    sensors_event_t event;
    accel.getEvent(&event);

    float x, y, z;
    x = event.acceleration.x;
    y = event.acceleration.y;
    z = event.acceleration.z;

    //Values from zero to +90 or -90 degrees (~10.50)
    levelX = map(x * 100, calibrationX - 1065, calibrationX + 1065, -900, 900);
    levelY = map(y * 100, calibrationY - 1055, calibrationY + 1055, -900, 900);
  */

  int x = accel.getX();
  int y = accel.getY();
  int z = accel.getZ();
  //-90 -> +90
  //-432 -> 109 / -287 -> 247
  //541 / 533
  //Divide by 2

  //Zero
  //-150 / -25
  levelX = map(x, (valuationX - calibrationX) * -1, (valuationX + calibrationX), -900, 900);
  levelY = map(y, (valuationY - calibrationY) * -1, (valuationY + calibrationY), -900, 900);

  Serial.print("X: ");
  Serial.print(levelX);
  Serial.print(" Y: ");
  Serial.print(levelY);
  Serial.print(" (");
  Serial.print(x);
  Serial.print(" / ");
  Serial.print(y);
  Serial.print(" / ");
  Serial.print(z);
  Serial.println(")");
}

void CalibrateLevel() {
  //Get current values, store as "Zero-levelled" and write to eeprom
  calibrationX = accel.getX();
  calibrationY = accel.getY();

  Serial.print("Calibrated to X: ");
  Serial.print(calibrationX);
  Serial.print(" Y: ");
  Serial.println(calibrationY);
  StoreLevel();
}

void LoadLevel() {
  calibrationX = readIntFromEEPROM(EEPROM_LEVEL_X);
  calibrationY = readIntFromEEPROM(EEPROM_LEVEL_Y);

  if (calibrationX == 0xffff || calibrationY == 0xffff) {
    calibrationX = -150;
    calibrationY = -125;
    StoreLevel();
  }
  Serial.print("Loaded X: ");
  Serial.print(calibrationX);
  Serial.print(" Y: ");
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
  Serial.print(" X: ");
  Serial.print(valuationX);
  Serial.print(" Y: ");
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

