long Mot3(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 3
  long result;

  float Mot3LengthX = XCorner3 - X;
  float Mot3lengthY = yCorner3 - Y;
  float Mot3lengthZ = zCorner3 - Z;

  float DB = Pythagoras(Mot3lengthY, Mot3LengthX, Mot3lengthZ);

  float BP = DB - RadiusPulley;
  float MP = zCorner3 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, RadiusPulley, 0);

  // float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);
  float hoekKLM = phi - acos1(RadiusPulley / BM) - acos1(abs(MP) / BM);

  float KL = RadiusPulley * hoekKLM;

  float Mot3Length = BK + KL;

  result = ((Mot3Length * StepUnit) + 0.5);
  return result;
};
