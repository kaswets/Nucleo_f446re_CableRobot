long Mot7(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 7
  long result;

  float Mot7LengthX = XCorner7 - X;
  float Mot7lengthY = yCorner7 - Y;
  float Mot7lengthZ = zCorner7 - Z;

  float DB = Pythagoras(Mot7lengthY, Mot7LengthX, Mot7lengthZ);

  float BP = DB - RadiusPulley;
  float MP = zCorner7 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, RadiusPulley, 0);

  // float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);
  float hoekKLM = phi - acos1(RadiusPulley / BM) - acos1(abs(MP) / BM);

  float KL = RadiusPulley * hoekKLM;

  float Mot7Length = BK + KL;

  result = ((Mot7Length * StepUnit) + 0.5);
  return result;
};
