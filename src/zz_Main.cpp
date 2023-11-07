
#include <Arduino.h>

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// remove Serial prints for debugging

#define DEBUG 1

#if DEBUG == 1
#define Debug(x) Serial.print(x)
#define Debugln(x) Serial.println(x)
#else
#define Debug(x)
#define Debugln(x)
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------
// init globale variabelen:
int blockx = 0;
int blocky = 0;
int blockz = 0;
int steplength = 1;

//// beweeg:
// lineto(-40,30,0); // lijnrecht naar punt (-40,30), met snelheid 10 cm/sec
// circle(-10,0,2*pi/3,30); // cirkelvormig om middelpunt (-10,0) over een hoek van (2/3)*pi (tegen de klok in) met snelheid 30 cm/sec
// lineto(80,20,20); // lijnrecht naar punt (80,20) met snelheid 20 cm/sec
// circle(40,10,-pi,10); // cirkelvormig om middelpunt (40,10) over een hoek van -pi (=pi met klok mee) met snelheid 10 cm/sec

// Constanten
// alle maten voorlopig in mm

// gemeten stappen/mm ~250
float StepUnit = 250.0;

bool AutoMove = 0;
bool NextPos = 0;
int AutoCnt = 0;

float phi = 3.141592653589;

float StepSpeed = 500;

float LengthBlockBottom = 118;
float WidthBlockBottom = 48;
float HighBlock = 41;

float LengthBlockTop = 68;
float WidthBlockTop = 78;

float LengthFrame = 320;
float WidhtFrame = 200;
float HighFrame = 280;

float DiaKatrol = 21.40;

float ActualMainX;
float ActualMainY;
float ActualMainZ;

float WantedMainX;
float WantedMainY;
float WantedMainZ;

float WantedRotX;
float ActualRotX;

float WantedRotY;
float ActualRotY;

float WantedRotZ;
float ActualRotZ;

long TotalMove[8];
int MotorNbr[8] = {1, 2, 3, 4, 5, 6, 7, 8};
String CommandFromPC = "";
bool Inpos = 1;
bool InposOld = 1;
float Procent100;
bool CalibrationIsOn = true;
bool go = false;

struct Coordinate
{
  float X;
  float Y;
  float Z;
};
struct Rotation
{
  float X;
  float Y;
  float Z;
};
struct Coordinate ObjectCorner[9];

struct Coordinate NewPosition[20] = {{0, 0, 30},
                                     {0, 0, 0},
                                     {0, 0, -30},
                                     {0, 0, 0},
                                     {0, 30, 0},
                                     {0, 0, 0},
                                     {0, -30, 0},
                                     {0, 0, 0},
                                     {30, 0, 0},
                                     {0, 0, 0},
                                     {-30, 0, 0},
                                     {0, 0, 0},
                                     {0, 0, 0},
                                     {0, 0, 0},
                                     {0, 0, 0},
                                     {0, 0, 0},
                                     {0, 0, 0},
                                     {0, 0, 0},
                                     {0, 0, 0},
                                     {0, 0, 0}};
struct Rotation NewRotation[20] = {{0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {0, 0, 0},
                                   {45, 0, 0},
                                   {0, 0, 0},
                                   {-45, 0, 0},
                                   {0, 0, 0},
                                   {0, 30, 0},
                                   {0, 0, 0},
                                   {0, -30, 0},
                                   {0, 0, 0}};

#define LED_BUILTIN_Green PA5
#define oDir1 PC1
#define oStep1 PC0
#define oDir2 PC3
#define oStep2 PC2
#define oDir3 PC10
#define oStep3 PC11
#define oDir4 PC13
#define oStep4 PC12
#define oDir5 PA1
#define oStep5 PA0
#define oDir6 PC5
#define oStep6 PB13
#define oDir7 PC7
#define oStep7 PC6
#define oDir8 PC9
#define oStep8 PC8

//----------------------------------------------------------------------------------------------
bool Mot1On = 0;
bool Mot1Direction = 0;
long Mot1WantedLength;
long Mot1ActualLength;
int Mot1PulseProcent;
int Mot1PulseCounter;
bool Pulse1 = 0;
//----------------------------------------------------------------------------------------------
bool Mot2On = 0;
bool Mot2Direction = 0;
long Mot2WantedLength;
long Mot2ActualLength;
int Mot2PulseProcent;
int Mot2PulseCounter;
bool Pulse2 = 0;
//----------------------------------------------------------------------------------------------
bool Mot3On = 0;
bool Mot3Direction = 0;
long Mot3WantedLength;
long Mot3ActualLength;
int Mot3PulseProcent;
int Mot3PulseCounter;
bool Pulse3 = 0;
//----------------------------------------------------------------------------------------------
bool Mot4On = 0;
bool Mot4Direction = 0;
long Mot4WantedLength;
long Mot4ActualLength;
int Mot4PulseProcent;
int Mot4PulseCounter;
bool Pulse4 = 0;
//----------------------------------------------------------------------------------------------
bool Mot5On = 0;
bool Mot5Direction = 0;
long Mot5WantedLength;
long Mot5ActualLength;
int Mot5PulseProcent;
int Mot5PulseCounter;
bool Pulse5 = 0;
//----------------------------------------------------------------------------------------------
bool Mot6On = 0;
bool Mot6Direction = 0;
long Mot6WantedLength;
long Mot6ActualLength;
int Mot6PulseProcent;
int Mot6PulseCounter;
bool Pulse6 = 0;
//----------------------------------------------------------------------------------------------
bool Mot7On = 0;
bool Mot7Direction = 0;
long Mot7WantedLength;
long Mot7ActualLength;
int Mot7PulseProcent;
int Mot7PulseCounter;
bool Pulse7 = 0;
//----------------------------------------------------------------------------------------------
bool Mot8On = 0;
bool Mot8Direction = 0;
long Mot8WantedLength;
long Mot8ActualLength;
int Mot8PulseProcent;
int Mot8PulseCounter;
bool Pulse8 = 0;
//----------------------------------------------------------------------------------------------

int incomingByte = 0;

bool Pulse = 0;
bool PulseOld = 0;
int PulseCounter = 0;
bool CalAgain = 0;

#include "ab_Pythagoras.cpp"

#include "ag_ParseData.cpp"

#include "ac_MatrixTrans.cpp"
#include "ad_MatrixRotX.cpp"
#include "ae_MatrixRotY.cpp"
#include "af_MatrixRotZ.cpp"

#include "ea_Mot1_xyz.cpp"
#include "eb_Mot2_xyz.cpp"
#include "ec_Mot3_xyz.cpp"
#include "ed_Mot4_xyz.cpp"
#include "ee_Mot5_xyz.cpp"
#include "ef_Mot6_xyz.cpp"
#include "eg_Mot7_xyz.cpp"
#include "eh_Mot8_xyz.cpp"

#include "ba_Motor1.cpp"
#include "bb_Motor2.cpp"
#include "bc_Motor3.cpp"
#include "bd_Motor4.cpp"
#include "ca_Motor5.cpp"
#include "cb_Motor6.cpp"
#include "cc_Motor7.cpp"
#include "cd_Motor8.cpp"

void setup()
{

  Serial.begin(115200);
  while (!Serial.available())
    ;
  Serial.println("Communication Active");

  pinMode(LED_BUILTIN_Green, OUTPUT);
  digitalWrite(LED_BUILTIN_Green, HIGH);

  pinMode(oDir1, OUTPUT);
  pinMode(oStep1, OUTPUT);

  pinMode(oDir2, OUTPUT);
  pinMode(oStep2, OUTPUT);

  pinMode(oDir3, OUTPUT);
  pinMode(oStep3, OUTPUT);

  pinMode(oDir4, OUTPUT);
  pinMode(oStep4, OUTPUT);

  pinMode(oDir5, OUTPUT);
  pinMode(oStep5, OUTPUT);

  pinMode(oDir6, OUTPUT);
  pinMode(oStep6, OUTPUT);

  pinMode(oDir7, OUTPUT);
  pinMode(oStep7, OUTPUT);

  pinMode(oDir8, OUTPUT);
  pinMode(oStep8, OUTPUT);

  // Setup Array by start programma
  ObjectCorner[1].X = ActualMainX - (LengthBlockBottom / 2.0);
  ObjectCorner[1].Y = ActualMainY - (WidthBlockBottom / 2.0);
  ObjectCorner[1].Z = ActualMainZ - (HighBlock / 2.0);

  Mot1ActualLength = Mot1(ObjectCorner[1].X, ObjectCorner[1].Y, ObjectCorner[1].Z);
  Mot1WantedLength = Mot1ActualLength;

  ObjectCorner[2].X = ActualMainX - (LengthBlockBottom / 2.0);
  ObjectCorner[2].Y = ActualMainY + (WidthBlockBottom / 2.0);
  ObjectCorner[2].Z = ActualMainZ - (HighBlock / 2.0);

  Mot2ActualLength = Mot2(ObjectCorner[2].X, ObjectCorner[2].Y, ObjectCorner[2].Z);
  Mot2WantedLength = Mot2ActualLength;

  ObjectCorner[3].X = ActualMainX + (LengthBlockBottom / 2.0);
  ObjectCorner[3].Y = ActualMainY - (WidthBlockBottom / 2.0);
  ObjectCorner[3].Z = ActualMainZ - (HighBlock / 2.0);

  Mot3ActualLength = Mot3(ObjectCorner[3].X, ObjectCorner[3].Y, ObjectCorner[3].Z);
  Mot3WantedLength = Mot3ActualLength;

  ObjectCorner[4].X = ActualMainX + (LengthBlockBottom / 2.0);
  ObjectCorner[4].Y = ActualMainY + (WidthBlockBottom / 2.0);
  ObjectCorner[4].Z = ActualMainZ - (HighBlock / 2.0);

  Mot4ActualLength = Mot4(ObjectCorner[4].X, ObjectCorner[4].Y, ObjectCorner[4].Z);
  Mot4WantedLength = Mot4ActualLength;

  ObjectCorner[5].X = ActualMainX - (LengthBlockBottom / 2.0);
  ObjectCorner[5].Y = ActualMainY - (WidthBlockBottom / 2.0);
  ObjectCorner[5].Z = ActualMainZ + (HighBlock / 2.0);

  Mot5ActualLength = Mot5(ObjectCorner[5].X, ObjectCorner[5].Y, ObjectCorner[5].Z);
  Mot5WantedLength = Mot5ActualLength;

  ObjectCorner[6].X = ActualMainX - (LengthBlockBottom / 2.0);
  ObjectCorner[6].Y = ActualMainY + (WidthBlockBottom / 2.0);
  ObjectCorner[6].Z = ActualMainZ + (HighBlock / 2.0);

  Mot6ActualLength = Mot6(ObjectCorner[6].X, ObjectCorner[6].Y, ObjectCorner[6].Z);
  Mot6WantedLength = Mot6ActualLength;

  ObjectCorner[7].X = ActualMainX + (LengthBlockBottom / 2.0);
  ObjectCorner[7].Y = ActualMainY - (WidthBlockBottom / 2.0);
  ObjectCorner[7].Z = ActualMainZ + (HighBlock / 2.0);

  Mot7ActualLength = Mot7(ObjectCorner[7].X, ObjectCorner[7].Y, ObjectCorner[7].Z);
  Mot7WantedLength = Mot7ActualLength;

  ObjectCorner[8].X = ActualMainX + (LengthBlockBottom / 2.0);
  ObjectCorner[8].Y = ActualMainY + (WidthBlockBottom / 2.0);
  ObjectCorner[8].Z = ActualMainZ + (HighBlock / 2.0);

  Mot8ActualLength = Mot8(ObjectCorner[8].X, ObjectCorner[8].Y, ObjectCorner[8].Z);
  Mot8WantedLength = Mot8ActualLength;
}

void loop()
{

  digitalWrite(LED_BUILTIN_Green, Pulse);

  if (Serial.available())
  {
    char c = Serial.read();
    Serial.println(c);
    if (c == '\n')
    {
      ParseCommand(CommandFromPC);
      CommandFromPC = "";
    }
    else
    {
      CommandFromPC += c;
    }
  }

  //=====================================================================================================
  // Create pulses
  Pulse = Pulse xor 1; // invert pulse

  if (Pulse > PulseOld)
  {
    PulseCounter = PulseCounter + 1;
    if (PulseCounter >= 100 and Inpos == 0)
    {
      PulseCounter = 0;
      CalAgain = 1;
    }
  }

  // delayMicroseconds(10);
  //=====================================================================================================
  if (Inpos == 1)
  {

    if (WantedRotX != ActualRotX)
    {
      // turn object  x-axis
      for (int j = 1; j < 9; j++)
      {
        matrixRotX(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, ActualRotX - WantedRotX, j);
      };
      go = 1;
    };
    if (WantedRotY != ActualRotY)
    {
      // turn object Y-axis
      for (int j = 1; j < 9; j++)
      {
        matrixRotY(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, ActualRotY - WantedRotY, j);
      };
      go = 1;
    };
    if (WantedRotZ != ActualRotZ)
    {
      // turn object Z-axis
      for (int j = 1; j < 9; j++)
      {
        matrixRotZ(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, ActualRotZ - WantedRotZ, j);
      };
      go = 1;
    };

    if (WantedMainX != ActualMainX or WantedMainY != ActualMainY or WantedMainZ != ActualMainZ)
    {
      // hoekpunten door de matrix halen, aan de hand van de main coordinaten
      for (int j = 1; j < 9; j++)
      {
        matrixTrans(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, (ActualMainX - WantedMainX), (ActualMainY - WantedMainY), -(ActualMainZ - WantedMainZ), j);
      };
      go = 1;
    };

    Mot1WantedLength = Mot1(ObjectCorner[1].X, ObjectCorner[1].Y, ObjectCorner[1].Z);
    Mot2WantedLength = Mot2(ObjectCorner[2].X, ObjectCorner[2].Y, ObjectCorner[2].Z);
    Mot3WantedLength = Mot3(ObjectCorner[3].X, ObjectCorner[3].Y, ObjectCorner[3].Z);
    Mot4WantedLength = Mot4(ObjectCorner[4].X, ObjectCorner[4].Y, ObjectCorner[4].Z);

    Mot5WantedLength = Mot5(ObjectCorner[5].X, ObjectCorner[5].Y, ObjectCorner[5].Z);
    Mot6WantedLength = Mot6(ObjectCorner[6].X, ObjectCorner[6].Y, ObjectCorner[6].Z);
    Mot7WantedLength = Mot7(ObjectCorner[7].X, ObjectCorner[7].Y, ObjectCorner[7].Z);
    Mot8WantedLength = Mot8(ObjectCorner[8].X, ObjectCorner[8].Y, ObjectCorner[8].Z);
  };

  if (go == 1 or CalAgain == 1)
  {
    CalAgain = 0;
    Inpos = 0;
    go = 0;

    // search for the longest move

    TotalMove[1] = abs(Mot1WantedLength - Mot1ActualLength);
    TotalMove[2] = abs(Mot2WantedLength - Mot2ActualLength);
    TotalMove[3] = abs(Mot3WantedLength - Mot3ActualLength);
    TotalMove[4] = abs(Mot4WantedLength - Mot4ActualLength);

    TotalMove[5] = abs(Mot5WantedLength - Mot5ActualLength);
    TotalMove[6] = abs(Mot6WantedLength - Mot6ActualLength);
    TotalMove[7] = abs(Mot7WantedLength - Mot7ActualLength);
    TotalMove[8] = abs(Mot8WantedLength - Mot8ActualLength);

    long storeTemp;
    int storeNbr;
    bool tmpSwapped;
  again:
    tmpSwapped = false;
    for (int i = 1; i < 9; i++)
    {
      if (TotalMove[i] < TotalMove[i + 1])
      {

        storeTemp = TotalMove[i + 1];
        TotalMove[i + 1] = TotalMove[i];
        TotalMove[i] = storeTemp;

        storeNbr = MotorNbr[i + 1];
        MotorNbr[i + 1] = MotorNbr[i];
        MotorNbr[i] = storeNbr;

        tmpSwapped = true;
      }
    }
    if (tmpSwapped == 1)
    {
      goto again;
    }

    Procent100 = 100.0 / TotalMove[1];
    // Serial.print("Procent100 :  100.0 /");
    // Serial.print(TotalMove[0]);
    // Serial.print(" = ");
    // Serial.println(Procent100, 4);

    Mot1PulseProcent = (Procent100 * TotalMove[1]) + 0.5;
    Mot2PulseProcent = (Procent100 * TotalMove[2]) + 0.5;
    Mot3PulseProcent = (Procent100 * TotalMove[3]) + 0.5;
    Mot4PulseProcent = (Procent100 * TotalMove[4]) + 0.5;
    Mot5PulseProcent = (Procent100 * TotalMove[5]) + 0.5;
    Mot6PulseProcent = (Procent100 * TotalMove[6]) + 0.5;
    Mot7PulseProcent = (Procent100 * TotalMove[7]) + 0.5;
    Mot8PulseProcent = (Procent100 * TotalMove[8]) + 0.5;

    // for (int i = 0; i < 8; i++)
    //{
    //   Serial.print("Motornr :");
    //  Serial.print(MotorNbr[i]);
    //  Serial.print(" TotalMove:");
    //  Serial.println(TotalMove[i]);
    //}

    // Serial.println(" ----------");
  }

  //=====================================================================================================
  // In position !

  if (abs(Mot1WantedLength - Mot1ActualLength) < 25 and
      abs(Mot2WantedLength - Mot2ActualLength) < 25 and
      abs(Mot3WantedLength - Mot3ActualLength) < 25 and
      abs(Mot4WantedLength - Mot4ActualLength) < 25 and
      abs(Mot5WantedLength - Mot5ActualLength) < 25 and
      abs(Mot6WantedLength - Mot6ActualLength) < 25 and
      abs(Mot7WantedLength - Mot7ActualLength) < 25 and
      abs(Mot8WantedLength - Mot8ActualLength) < 25)

  // if (Mot1WantedLength == Mot1ActualLength and
  //     Mot2WantedLength == Mot2ActualLength and
  //     Mot3WantedLength == Mot3ActualLength and
  //     Mot4WantedLength == Mot4ActualLength and
  //     Mot5WantedLength == Mot5ActualLength and
  //     Mot6WantedLength == Mot6ActualLength and
  //     Mot7WantedLength == Mot7ActualLength and
  //     Mot8WantedLength == Mot8ActualLength)
  {
    ActualMainX = WantedMainX;
    ActualMainY = WantedMainY;
    ActualMainZ = WantedMainZ;

    ActualRotX = WantedRotX;
    ActualRotY = WantedRotY;
    ActualRotZ = WantedRotZ;

    delay(50);

    Inpos = 1;
    Mot1PulseCounter = 0;
    Mot2PulseCounter = 0;
    Mot3PulseCounter = 0;
    Mot4PulseCounter = 0;
    Mot5PulseCounter = 0;
    Mot6PulseCounter = 0;
    Mot7PulseCounter = 0;
    Mot8PulseCounter = 0;
  };

  //------------------------------------------------------------------------------------
  // Control Motors
  if (Inpos == 0)
  {
    Motor1();
    Motor2();
    Motor3();
    Motor4();
    //
    Motor5();
    Motor6();
    Motor7();
    Motor8();
  };

  if (Inpos > InposOld)
  {
    Serial.println("InPosition");
    Serial.print("Main :");
    Serial.print(ActualMainX);
    Serial.print(" , ");
    Serial.print(ActualMainY);
    Serial.print(" , ");
    Serial.println(ActualMainZ);
    NextPos = 1;
  };
  if (Inpos < InposOld)
  {
    Serial.println("Moving");
  };

  InposOld = Inpos;

  if (AutoMove == 1)
  {
    if (NextPos == 1)
    {
      NextPos = 0;
      WantedMainX = NewPosition[AutoCnt].X;
      WantedMainY = NewPosition[AutoCnt].Y;
      WantedMainZ = NewPosition[AutoCnt].Z;

      WantedRotX = NewRotation[AutoCnt].X;
      WantedRotY = NewRotation[AutoCnt].Y;
      WantedRotZ = NewRotation[AutoCnt].Z;

      AutoCnt = AutoCnt + 1;
      if (AutoCnt > 19)
      {
        AutoCnt = 0;
      }
    }
  }
};
