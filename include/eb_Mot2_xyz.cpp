long Mot2(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 2
  long result;

  float Mot2LengthX = XCorner2 - X;
  float Mot2lengthY = yCorner2 - Y;
  float Mot2lengthZ = zCorner2 - Z;

  float DB = Pythagoras(Mot2lengthY, Mot2LengthX, Mot2lengthZ);

  float BP = DB - RadiusPulley;
  float MP = zCorner2 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, RadiusPulley, 0);

  // float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);
  float hoekKLM = phi - acos1(RadiusPulley / BM) - acos1(abs(MP) / BM);

  float KL = RadiusPulley * hoekKLM;

  float Mot2Length = BK + KL;

  result = ((Mot2Length * StepUnit) + 0.5);
  return result;
};
