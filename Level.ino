void getLevel() {
  if (!accelInitialized)
    return;

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
  Serial.print(")");  
  levelX = deepPassX(levelX);
  levelY = deepPassY(levelY);
  Serial.print(" DeepPass ");
  Serial.print(levelX);
  Serial.print(" / ");
  Serial.println(levelY);
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
