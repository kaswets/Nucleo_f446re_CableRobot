long Mot2(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 2
  long result;

  float XCorner2 = -(LengthFrame / 2);
  float yCorner2 = (WidhtFrame / 2);
  float zCorner2 = (HighFrame / 2);

  float Mot2LengthX = XCorner2 - X;
  float Mot2lengthY = yCorner2 - Y;
  float Mot2lengthZ = zCorner2 - Z;

  float DB = Pythagoras(Mot2lengthY, Mot2LengthX, Mot2lengthZ);

  float BP = DB - (DiaKatrol / 2);
  float MP = zCorner2 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, (DiaKatrol / 2), 0);

  float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);

  float KL = (DiaKatrol / 2) * hoekKLM;

  float Mot2Length = BK + KL;

  //Serial.print("Length Motor 2 :");
  //Serial.print(Mot2Length);
  //Serial.print("mm, total Steps :");
  //Serial.println(Mot2Length * StepUnit);

  result = ((Mot2Length * StepUnit) + 0.5);
  return result;
};
