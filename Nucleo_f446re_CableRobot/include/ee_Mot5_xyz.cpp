long Mot5(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 5
  long result;

  float Mot5LengthX = XCorner5 - X;
  float Mot5lengthY = yCorner5 - Y;
  float Mot5lengthZ = zCorner5 - Z;

  float DB = Pythagoras(Mot5lengthY, Mot5LengthX, Mot5lengthZ);

  float BP = DB - RadiusPulley;
  float MP = zCorner5 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, RadiusPulley, 0);

  // float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);
  float hoekKLM = phi - acos1(RadiusPulley / BM) - acos1(abs(MP) / BM);

  float KL = RadiusPulley * hoekKLM;

  float Mot5Length = BK + KL;

  result = ((Mot5Length * StepUnit) + 0.5);
  return result;
};
