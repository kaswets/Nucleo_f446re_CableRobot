long Mot3(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 3
  long result;

  float XCorner3 = (LengthFrame / 2);
  float yCorner3 = -(WidhtFrame / 2);
  float zCorner3 = (HighFrame / 2);

  float Mot3LengthX = XCorner3 - X;
  float Mot3lengthY = yCorner3 - Y;
  float Mot3lengthZ = zCorner3 - Z;

  float DB = Pythagoras(Mot3lengthY, Mot3LengthX, Mot3lengthZ);

  float BP = DB - (DiaKatrol / 2);
  float MP = zCorner3 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, (DiaKatrol / 2), 0);

  float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);

  float KL = (DiaKatrol / 2) * hoekKLM;

  float Mot3Length = BK + KL;

  //Serial.print("Length Motor 3 :");
  //Serial.print(Mot3Length);
  //Serial.print("mm, total Steps :");
  //Serial.println(Mot3Length * StepUnit);

  result = ((Mot3Length * StepUnit) + 0.5);
  return result;
};
