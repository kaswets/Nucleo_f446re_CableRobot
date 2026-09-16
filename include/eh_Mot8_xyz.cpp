long Mot8(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 8
  long result;

  float Mot8LengthX = XCorner8 - X;
  float Mot8lengthY = yCorner8 - Y;
  float Mot8lengthZ = zCorner8 - Z;

  float DB = Pythagoras(Mot8lengthY, Mot8LengthX, Mot8lengthZ);

  float BP = DB - RadiusPulley;
  float MP = zCorner8 - Z;

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, RadiusPulley, 0);

  // float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);
  float hoekKLM = phi - acos1(RadiusPulley / BM) - acos1(abs(MP) / BM);

  float KL = RadiusPulley * hoekKLM;

  float Mot8Length = BK + KL;

  result = ((Mot8Length * StepUnit) + 0.5);
  return result;
};
