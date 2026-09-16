float Pythagoras(float X, float Y, float Z)
{
  float TmpXkwadraat = X * X;
  float TmpYKwadraat = Y * Y;
  float TmpZKwadraat = Z * Z;

  float result;

  result = sqrt(TmpXkwadraat + TmpYKwadraat + TmpZKwadraat);
  return result;
};
