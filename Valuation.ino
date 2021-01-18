int minValuationX = 0;
int minValuationY = 0;
int maxValuationX = 0;
int maxValuationY = 0;

void Valutation() {
  if (!accelInitialized)
    return;

  int x = accel.getX();
  int y = accel.getY();
  int z = accel.getZ();
  //-90 -> +90
  //-432 -> 109 / -287 -> 247
  //541 / 533
  //Divide by 2

  if (x > maxValuationX)
    maxValuationX = x;
  if (x < minValuationX)
    minValuationX = x;

  if (y > maxValuationY)
    maxValuationY = y;
  if (y < minValuationY)
    minValuationY = y;

  valuationX = (maxValuationX - minValuationX) / 2;
  valuationY = (maxValuationY - minValuationY) / 2;

  if (valuationX < 0)
    valuationX *= -1;

  if (valuationY < 0)
    valuationY *= -1;


  Serial.print("X: ");
  Serial.print(minValuationX);
  Serial.print(" - ");
  Serial.print(maxValuationX);
  Serial.print(" (");
  Serial.print(valuationX);
  Serial.print(")");
  Serial.print(" Y: ");
  Serial.print(minValuationY);
  Serial.print(" - ");
  Serial.print(maxValuationY);
  Serial.print(" (");
  Serial.print(valuationX);
  Serial.println(")");
  
}

