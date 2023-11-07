void matrixRotY (float X, float Y , float Z , float Yrot, int Motnr ) {

  // Vectoren/punten hebben 4 elementen ipv 3: [X Y Z 1]
  //  Origineel[3] := 1.0;

  float Origineel[4]  ;
  float Getransformeerd[4];
  float Matrix[4][4];

  Yrot=Yrot/100;

  //    [ cos(yrot), 0, sin(yrot), 0]
  //    [         0, 1,         0, 0]
  //    [-sin(yrot), 0, cos(yrot), 0]
  //    [         0, 0,         0, 1]

  Matrix[0][0] = cos(Yrot);
  Matrix[0][1] = 0.0;
  Matrix[0][2] = sin(Yrot);
  Matrix[0][3] = 0.0;

  Matrix[1][0] = 0.0;
  Matrix[1][1] = 1.0;
  Matrix[1][2] = 0.0;
  Matrix[1][3] = 0.0;

  Matrix[2][0] = -sin(Yrot);
  Matrix[2][1] = 0.0;
  Matrix[2][2] = cos(Yrot);
  Matrix[2][3] = 0.0;

  Matrix[3][0] = 0.0;
  Matrix[3][1] = 0.0;
  Matrix[3][2] = 0.0;
  Matrix[3][3] = 1.0;

  Origineel[0] = X;
  Origineel[1] = Y;
  Origineel[2] = Z;
  Origineel[3] = 1.0;

  for (int i = 0; i <= 3; i++)
  {
    Getransformeerd[i]  = (Origineel[0] * Matrix[0][i]) + (Origineel[1] * Matrix[1][i]) + (Origineel[2] * Matrix[2][i]) + (Origineel[3] * Matrix[3][i]);
  }

  Getransformeerd[0] = Getransformeerd[0] / Getransformeerd[3];
  Getransformeerd[1] = Getransformeerd[1] / Getransformeerd[3];
  Getransformeerd[2] = Getransformeerd[2] / Getransformeerd[3];

//  Debug(Getransformeerd[0] );
//  Debug(" - ");
//  Debug(Getransformeerd[1] );
//  Debug(" - ");
//  Debugln(Getransformeerd[2] );

  ObjectCorner[Motnr].X = Getransformeerd[0];
  ObjectCorner[Motnr].Y = Getransformeerd[1];
  ObjectCorner[Motnr].Z = Getransformeerd[2];
  
};
