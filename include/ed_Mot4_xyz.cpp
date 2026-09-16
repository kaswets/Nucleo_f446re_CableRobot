long Mot4(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 4
  long result;

  float Mot4LengthX = XCorner4 - X;
  float Mot4lengthY = yCorner4 - Y;
  float Mot4lengthZ = zCorner4 - Z;

  float DB = Pythagoras(Mot4lengthY, Mot4LengthX, Mot4lengthZ);

  float BP = DB - RadiusPulley;
  float MP = zCorner4 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, RadiusPulley, 0);

  // float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);
  float hoekKLM = phi - acos1(RadiusPulley / BM) - acos1(abs(MP) / BM);

  float KL = RadiusPulley * hoekKLM;

  float Mot4Length = BK + KL;

  result = ((Mot4Length * StepUnit) + 0.5);
  return result;
};
