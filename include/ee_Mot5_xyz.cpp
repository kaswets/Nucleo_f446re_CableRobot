long Mot5(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 5
  long result;

  float XCorner5 = -(LengthFrame / 2);
  float yCorner5 = -(WidhtFrame / 2);
  float zCorner5 = -(HighFrame / 2);

  float Mot5LengthX = XCorner5 - X;
  float Mot5lengthY = yCorner5 - Y;
  float Mot5lengthZ = zCorner5 - Z;

  float DB = Pythagoras(Mot5lengthY, Mot5LengthX, Mot5lengthZ);

  float BP = DB - (DiaKatrol / 2);
  float MP = zCorner5 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, (DiaKatrol / 2), 0);

  float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);

  float KL = (DiaKatrol / 2) * hoekKLM;

  float Mot5Length = BK + KL;

  //Serial.print("Length Motor 5 :");
  //Serial.print(Mot5Length);
  //Serial.print("mm, total Steps :");
  //Serial.println(Mot5Length * StepUnit);

  result = ((Mot5Length * StepUnit) + 0.5);
  return result;
};
