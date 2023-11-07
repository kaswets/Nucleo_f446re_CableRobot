long Mot8(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 8
  long result;

  float XCorner8 = (LengthFrame / 2);
  float yCorner8 = (WidhtFrame / 2);
  float zCorner8 = -(HighFrame / 2);

  float Mot8LengthX = XCorner8 - X;
  float Mot8lengthY = yCorner8 - Y;
  float Mot8lengthZ = zCorner8 - Z;

  float DB = Pythagoras(Mot8lengthY, Mot8LengthX, Mot8lengthZ);

  float BP = DB - (DiaKatrol / 2);
  float MP = zCorner8 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, (DiaKatrol / 2), 0);

  float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);

  float KL = (DiaKatrol / 2) * hoekKLM;

  float Mot8Length = BK + KL;

  // Serial.print("Length Motor 8 :");
  // Serial.print(Mot8Length);
  // Serial.print("mm, total Steps :");
  // Serial.println(Mot8Length * StepUnit);

  result = ((Mot8Length * StepUnit) + 0.5);
  return result;
};
