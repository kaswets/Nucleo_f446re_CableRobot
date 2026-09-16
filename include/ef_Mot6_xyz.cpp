long Mot6(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 6
  long result;

  float Mot6LengthX = XCorner6 - X;
  float Mot6lengthY = yCorner6 - Y;
  float Mot6lengthZ = zCorner6 - Z;

  float DB = Pythagoras(Mot6lengthY, Mot6LengthX, Mot6lengthZ);

  float BP = DB - RadiusPulley;
  float MP = zCorner6 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, RadiusPulley, 0);

  // float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);
  float hoekKLM = phi - acos1(RadiusPulley / BM) - acos1(abs(MP) / BM);

  float KL = RadiusPulley * hoekKLM;

  float Mot6Length = BK + KL;

  result = ((Mot6Length * StepUnit) + 0.5);
  return result;
};
