
long Mot6(float X, float Y, float Z)
{
  // z,y,z uit rekenen van uit main x,y,z
  // x,y,z van hoek 6
  long result;

  float XCorner6 = -(LengthFrame / 2);
  float yCorner6 = (WidhtFrame / 2);
  float zCorner6 = -(HighFrame / 2);

  float Mot6LengthX = (XCorner6 - X);   //- (LengthBlockBottom / 2);
  float Mot6lengthY = (yCorner6 - Y);   //+ (WidthBlockBottom / 2);
  float Mot6lengthZ = (zCorner6 - (Z)); //- (HighBlock / 2);

  float DB = Pythagoras(Mot6lengthY, Mot6LengthX, Mot6lengthZ);

  float BP = DB - (DiaKatrol / 2);
  float MP = zCorner6 - (Z);

  float BM = Pythagoras(MP, BP, 0);

  float BK = Pythagoras(BM, (DiaKatrol / 2), 0);

  float hoekKLM = phi - asin(BK / BM) - asin(BP / BM);

  float KL = (DiaKatrol / 2) * hoekKLM;

  float Mot6Length = BK + KL;

  //Serial.print("Length Motor 6 :");
  //Serial.print(Mot6Length);
  //Serial.print("mm, total Steps :");
  //Serial.println(Mot6Length * StepUnit);

  result = ((Mot6Length * StepUnit) + 0.5);
  return result;
};
