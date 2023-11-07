long Mot1(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 1
  long result;

  float Mot1LengthX = XCorner1 - X;
  float Mot1lengthY = yCorner1 - Y;
  float Mot1lengthZ = zCorner1 - Z;

  float DB = Pythagoras(Mot1lengthY, Mot1LengthX, Mot1lengthZ);

  float BP = DB - RadiusPulley;
  float MP = zCorner1 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, RadiusPulley, 0);

  // float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);
  float hoekKLM = phi - acos1(RadiusPulley / BM) - acos1(abs(MP) / BM);
 
  float KL = RadiusPulley * hoekKLM;

  float Mot1Length = BK + KL;

  result = ((Mot1Length * StepUnit) + 0.5);
  return result;
};
