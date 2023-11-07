long Mot7(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 7
  long result;

  float XCorner7 = (LengthFrame / 2);
  float yCorner7 = -(WidhtFrame / 2);
  float zCorner7 = -(HighFrame / 2);

  float Mot7LengthX = XCorner7 - X;
  float Mot7lengthY = yCorner7 - Y;
  float Mot7lengthZ = zCorner7 - Z;

  float DB = Pythagoras(Mot7lengthY, Mot7LengthX, Mot7lengthZ);

  float BP = DB - (DiaKatrol / 2);
  float MP = zCorner7 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, (DiaKatrol / 2), 0);

  float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);

  float KL = (DiaKatrol / 2) * hoekKLM;

  float Mot7Length = BK + KL;

  //Serial.print("Length Motor 7 :");
  //Serial.print(Mot7Length);
  //Serial.print("mm, total Steps :");
  //Serial.println(Mot7Length * StepUnit);

  result = ((Mot7Length * StepUnit) + 0.5);
  return result;
};
