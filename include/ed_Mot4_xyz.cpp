long Mot4(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 4
  long result;

  float XCorner4 = (LengthFrame / 2);
  float yCorner4 = (WidhtFrame / 2);
  float zCorner4 = (HighFrame / 2);

  float Mot4LengthX = XCorner4 - X;
  float Mot4lengthY = yCorner4 - Y;
  float Mot4lengthZ = zCorner4 - Z;

  float DB = Pythagoras(Mot4lengthY, Mot4LengthX, Mot4lengthZ);

  float BP = DB - (DiaKatrol / 2);
  float MP = zCorner4 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, (DiaKatrol / 2), 0);

  float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);

  float KL = (DiaKatrol / 2) * hoekKLM;

  float Mot4Length = BK + KL;

  //Serial.print("Length Motor 4 :");
  //Serial.print(Mot4Length);
  //Serial.print("mm, total Steps :");
  //Serial.println(Mot4Length * StepUnit);

  result = ((Mot4Length * StepUnit) + 0.5);
  return result;
};
