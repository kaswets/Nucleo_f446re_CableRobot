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

float StepSpeed = 200;

float LengthBlockBottom = 118;
float WidthBlockBottom = 48;
float HighBlock = 41;

float LengthBlockTop = 68;
float WidthBlockTop = 78;

float LengthFrame = 325;
float WidhtFrame = 210;
float HighFrame = 208;

float DiaPulley = 21.40;
float RadiusPulley = DiaPulley / 2.0;

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

// SIMTOOLS INTEGRATION VARIABLES
bool simToolsMode = false;
unsigned long lastSimToolsData = 0;
unsigned long lastSimHubData = 0;  // ADDED FOR SIMHUB SUPPORT
const unsigned long SIMTOOLS_TIMEOUT = 1000;

// Motion scaling factors (pas aan voor jouw workspace)
const float SIMTOOLS_ROTATION_SCALE = 0.15;     // 15% van max rotatie
const float SIMTOOLS_TRANSLATION_SCALE = 0.1;   // 10% van max translatie

// Workspace limits (in mm en graden)  
const float MAX_TRANSLATION = 25.0;  // ±25mm veilig
const float MAX_ROTATION = 6.0;      // ±6° veilig

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
struct Coordinate ObjectCorner[8];

struct Coordinate NewPosition[24] = {{0, 0, 0},
                                     {0, 0, -5},
                                     {0, 0, -10},
                                     {0, 0, -15},
                                     {0, 0, -20},
                                     {0, 0, -25},
                                     {0, 0, -30},
                                     {0, 0, -25},
                                     {0, 0, -20},
                                     {0, 0, -15},
                                     {0, 0, -10},
                                     {0, 0, -5},
                                     {0, 0, 0},
                                     {0, 0, 5},
                                     {0, 0, 10},
                                     {0, 0, 15},
                                     {0, 0, 20},
                                     {0, 0, 25},
                                     {0, 0, 30},
                                     {0, 0, 25},
                                     {0, 0, 20},
                                     {0, 0, 15},
                                     {0, 0, 10},
                                     {0, 0, 5}};

struct Rotation NewRotation[24] = {{0, 0, 0},
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
bool Mot1Direction = 0;
long Mot1WantedLength;
long Mot1ActualLength;
int Mot1PulseProcent;
int Mot1PulseCounter;
bool Pulse1 = 0;

float XCorner1 = -(LengthFrame / 2.0);
float yCorner1 = -(WidhtFrame / 2.0);
float zCorner1 = (HighFrame / 2.0);
//----------------------------------------------------------------------------------------------
bool Mot2Direction = 0;
long Mot2WantedLength;
long Mot2ActualLength;
int Mot2PulseProcent;
int Mot2PulseCounter;
bool Pulse2 = 0;

float XCorner2 = -(LengthFrame / 2);
float yCorner2 = (WidhtFrame / 2);
float zCorner2 = (HighFrame / 2);
//----------------------------------------------------------------------------------------------
bool Mot3Direction = 0;
long Mot3WantedLength;
long Mot3ActualLength;
int Mot3PulseProcent;
int Mot3PulseCounter;
bool Pulse3 = 0;

float XCorner3 = (LengthFrame / 2);
float yCorner3 = -(WidhtFrame / 2);
float zCorner3 = (HighFrame / 2);
//----------------------------------------------------------------------------------------------
bool Mot4Direction = 0;
long Mot4WantedLength;
long Mot4ActualLength;
int Mot4PulseProcent;
int Mot4PulseCounter;
bool Pulse4 = 0;

float XCorner4 = (LengthFrame / 2);
float yCorner4 = (WidhtFrame / 2);
float zCorner4 = (HighFrame / 2);
//----------------------------------------------------------------------------------------------
bool Mot5Direction = 0;
long Mot5WantedLength;
long Mot5ActualLength;
int Mot5PulseProcent;
int Mot5PulseCounter;
bool Pulse5 = 0;

float XCorner5 = -(LengthFrame / 2);
float yCorner5 = -(WidhtFrame / 2);
float zCorner5 = -(HighFrame / 2);
//----------------------------------------------------------------------------------------------
bool Mot6Direction = 0;
long Mot6WantedLength;
long Mot6ActualLength;
int Mot6PulseProcent;
int Mot6PulseCounter;
bool Pulse6 = 0;

float XCorner6 = -(LengthFrame / 2);
float yCorner6 = (WidhtFrame / 2);
float zCorner6 = -(HighFrame / 2);
//----------------------------------------------------------------------------------------------
bool Mot7Direction = 0;
long Mot7WantedLength;
long Mot7ActualLength;
int Mot7PulseProcent;
int Mot7PulseCounter;
bool Pulse7 = 0;

float XCorner7 = (LengthFrame / 2);
float yCorner7 = -(WidhtFrame / 2);
float zCorner7 = -(HighFrame / 2);
//----------------------------------------------------------------------------------------------
bool Mot8Direction = 0;
long Mot8WantedLength;
long Mot8ActualLength;
int Mot8PulseProcent;
int Mot8PulseCounter;
bool Pulse8 = 0;

float XCorner8 = (LengthFrame / 2);
float yCorner8 = (WidhtFrame / 2);
float zCorner8 = -(HighFrame / 2);
//----------------------------------------------------------------------------------------------

int incomingByte = 0;

HardwareTimer timer(TIM1);

// Function declarations - MUST BE BEFORE INCLUDES
void parseSimHubMotion(String data);  // ADDED FOR SIMHUB SUPPORT

#include "ab_Pythagoras.cpp"
#include "acos1.cpp"

#include "ag_ParseData.cpp"
#include "ah_ParseSimHubMotion.cpp"  // ADDED FOR SIMHUB SUPPORT

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

// SimTools timeout check functie
void checkSimToolsTimeout() {
  if (simToolsMode && (millis() - lastSimToolsData > SIMTOOLS_TIMEOUT)) {
    // Return to center bij timeout
    WantedMainX = 0;
    WantedMainY = 0;
    WantedMainZ = 0;
    WantedRotX = 0;
    WantedRotY = 0;
    WantedRotZ = 0;
    
    simToolsMode = false;
    Serial.println("SimTools timeout - returning to center");
  }
}

void OnTimer1Interrupt()
{
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
}

void setup()
{
  Serial.begin(115200);
  while (!Serial.available())
    ;
  Serial.println("Communication Active - SimTools + SimHub Ready");  // UPDATED MESSAGE

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
  ObjectCorner[0].X = ActualMainX - (LengthBlockBottom / 2.0);
  ObjectCorner[0].Y = ActualMainY - (WidthBlockBottom / 2.0);
  ObjectCorner[0].Z = ActualMainZ - (HighBlock / 2.0);
  Mot1ActualLength = Mot1(ObjectCorner[0].X, ObjectCorner[0].Y, ObjectCorner[0].Z);
  Mot1WantedLength = Mot1ActualLength;

  ObjectCorner[1].X = ActualMainX - (LengthBlockBottom / 2.0);
  ObjectCorner[1].Y = ActualMainY + (WidthBlockBottom / 2.0);
  ObjectCorner[1].Z = ActualMainZ - (HighBlock / 2.0);
  Mot2ActualLength = Mot2(ObjectCorner[1].X, ObjectCorner[1].Y, ObjectCorner[1].Z);
  Mot2WantedLength = Mot2ActualLength;

  ObjectCorner[2].X = ActualMainX + (LengthBlockBottom / 2.0);
  ObjectCorner[2].Y = ActualMainY - (WidthBlockBottom / 2.0);
  ObjectCorner[2].Z = ActualMainZ - (HighBlock / 2.0);
  Mot3ActualLength = Mot3(ObjectCorner[2].X, ObjectCorner[2].Y, ObjectCorner[2].Z);
  Mot3WantedLength = Mot3ActualLength;

  ObjectCorner[3].X = ActualMainX + (LengthBlockBottom / 2.0);
  ObjectCorner[3].Y = ActualMainY + (WidthBlockBottom / 2.0);
  ObjectCorner[3].Z = ActualMainZ - (HighBlock / 2.0);
  Mot4ActualLength = Mot4(ObjectCorner[3].X, ObjectCorner[3].Y, ObjectCorner[3].Z);
  Mot4WantedLength = Mot4ActualLength;

  ObjectCorner[4].X = ActualMainX - (LengthBlockTop / 2.0);
  ObjectCorner[4].Y = ActualMainY - (WidthBlockTop / 2.0);
  ObjectCorner[4].Z = ActualMainZ + (HighBlock / 2.0);
  Mot5ActualLength = Mot5(ObjectCorner[4].X, ObjectCorner[4].Y, ObjectCorner[4].Z);
  Mot5WantedLength = Mot5ActualLength;

  ObjectCorner[5].X = ActualMainX - (LengthBlockTop / 2.0);
  ObjectCorner[5].Y = ActualMainY + (WidthBlockTop / 2.0);
  ObjectCorner[5].Z = ActualMainZ + (HighBlock / 2.0);
  Mot6ActualLength = Mot6(ObjectCorner[5].X, ObjectCorner[5].Y, ObjectCorner[5].Z);
  Mot6WantedLength = Mot6ActualLength;

  ObjectCorner[6].X = ActualMainX + (LengthBlockTop / 2.0);
  ObjectCorner[6].Y = ActualMainY - (WidthBlockTop / 2.0);
  ObjectCorner[6].Z = ActualMainZ + (HighBlock / 2.0);
  Mot7ActualLength = Mot7(ObjectCorner[6].X, ObjectCorner[6].Y, ObjectCorner[6].Z);
  Mot7WantedLength = Mot7ActualLength;

  ObjectCorner[7].X = ActualMainX + (LengthBlockTop / 2.0);
  ObjectCorner[7].Y = ActualMainY + (WidthBlockTop / 2.0);
  ObjectCorner[7].Z = ActualMainZ + (HighBlock / 2.0);
  Mot8ActualLength = Mot8(ObjectCorner[7].X, ObjectCorner[7].Y, ObjectCorner[7].Z);
  Mot8WantedLength = Mot8ActualLength;

  // Configure timer
  timer.setPrescaleFactor(2564);              // Set prescaler to 2564 => timer frequency = 168MHz/2564 = 65522 Hz (from prediv'd by 1 clocksource of 168 MHz)
  timer.setOverflow(10);                      // Set overflow to 32761 => timer frequency = 65522 Hz / 32761 = 2 Hz
  timer.attachInterrupt(OnTimer1Interrupt);
  timer.refresh();                            // Make register changes take effect
  timer.resume();                             // Start
}

void loop()
{
  // Simplified serial handling for SimTools
  if (Serial.available())
  {
    char c = Serial.read();
    
    if (c == '\n')  // End of line
    {
      if (CommandFromPC.length() > 0) {
        // Remove any \r characters
        CommandFromPC.replace("\r", "");
        
        // Process command (SimTools or manual)
        ParseCommand(CommandFromPC);
      }
      
      CommandFromPC = "";
    }
    else if (c != '\r')  // Ignore carriage return, but keep other chars
    {
      CommandFromPC += c;
    }
  }

  // SimTools timeout check
  checkSimToolsTimeout();

  // Rest van je bestaande loop code
  if (Inpos == 1)
  {
    if (WantedRotX != ActualRotX)
    {
      // turn object  x-axis
      // nieuwe X,Y,Z coordinaten hoekpunten berekenen
      for (int j = 0; j < 8; j++)
      {
        matrixRotX(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, ActualRotX - WantedRotX, j);
      };
      go = 1;
    };
    if (WantedRotY != ActualRotY)
    {
      // turn object Y-axis
      // nieuwe X,Y,Z coordinaten hoekpunten berekenen
      for (int j = 0; j < 8; j++)
      {
        matrixRotY(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, ActualRotY - WantedRotY, j);
      };
      go = 1;
    };
    if (WantedRotZ != ActualRotZ)
    {
      // turn object Z-axis
      // nieuwe X,Y,Z coordinaten hoekpunten berekenen
      for (int j = 0; j < 8; j++)
      {
        matrixRotZ(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, ActualRotZ - WantedRotZ, j);
      };
      go = 1;
    };

    if (WantedMainX != ActualMainX or WantedMainY != ActualMainY or WantedMainZ != ActualMainZ)
    {
      // hoekpunten door de matrix halen, aan de hand van de main coordinaten
      // nieuwe X,Y,Z coordinaten hoekpunten berekenen
      for (int j = 0; j < 8; j++)
      {
        matrixTrans(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, (ActualMainX - WantedMainX), (ActualMainY - WantedMainY), -(ActualMainZ - WantedMainZ), j);
      };
      go = 1;
    };

    Mot1WantedLength = Mot1(ObjectCorner[0].X, ObjectCorner[0].Y, ObjectCorner[0].Z);
    Mot2WantedLength = Mot2(ObjectCorner[1].X, ObjectCorner[1].Y, ObjectCorner[1].Z);
    Mot3WantedLength = Mot3(ObjectCorner[2].X, ObjectCorner[2].Y, ObjectCorner[2].Z);
    Mot4WantedLength = Mot4(ObjectCorner[3].X, ObjectCorner[3].Y, ObjectCorner[3].Z);

    Mot5WantedLength = Mot5(ObjectCorner[4].X, ObjectCorner[4].Y, ObjectCorner[4].Z);
    Mot6WantedLength = Mot6(ObjectCorner[5].X, ObjectCorner[5].Y, ObjectCorner[5].Z);
    Mot7WantedLength = Mot7(ObjectCorner[6].X, ObjectCorner[6].Y, ObjectCorner[6].Z);
    Mot8WantedLength = Mot8(ObjectCorner[7].X, ObjectCorner[7].Y, ObjectCorner[7].Z);
  };

  if (go == 1 )
  {

    Inpos = 0;
    go = 0;

    // search for the longest move
    TotalMove[0] = abs(Mot1WantedLength - Mot1ActualLength);
    TotalMove[1] = abs(Mot2WantedLength - Mot2ActualLength);
    TotalMove[2] = abs(Mot3WantedLength - Mot3ActualLength);
    TotalMove[3] = abs(Mot4WantedLength - Mot4ActualLength);

    TotalMove[4] = abs(Mot5WantedLength - Mot5ActualLength);
    TotalMove[5] = abs(Mot6WantedLength - Mot6ActualLength);
    TotalMove[6] = abs(Mot7WantedLength - Mot7ActualLength);
    TotalMove[7] = abs(Mot8WantedLength - Mot8ActualLength);

    // FIX 16-9-2026: geen sortering meer. Elke motor krijgt zijn EIGEN percentage
    // t.o.v. de grootste beweging (oude bubble sort gaf motoren de waarde van een andere motor).
    long maxMove = 0;
    for (int i = 0; i < 8; i++)
    {
      if (TotalMove[i] > maxMove) maxMove = TotalMove[i];
    }
    if (maxMove < 1) maxMove = 1;

    Procent100 = 100.0 / maxMove;

    Mot1PulseProcent = (Procent100 * TotalMove[0]) + 0.5;
    Mot2PulseProcent = (Procent100 * TotalMove[1]) + 0.5;
    Mot3PulseProcent = (Procent100 * TotalMove[2]) + 0.5;
    Mot4PulseProcent = (Procent100 * TotalMove[3]) + 0.5;
    Mot5PulseProcent = (Procent100 * TotalMove[4]) + 0.5;
    Mot6PulseProcent = (Procent100 * TotalMove[5]) + 0.5;
    Mot7PulseProcent = (Procent100 * TotalMove[6]) + 0.5;
    Mot8PulseProcent = (Procent100 * TotalMove[7]) + 0.5;
  }

  //=====================================================================================================
  // In position !
  if (abs(Mot1WantedLength - Mot1ActualLength) < 2 and
      abs(Mot2WantedLength - Mot2ActualLength) < 2 and
      abs(Mot3WantedLength - Mot3ActualLength) < 2 and
      abs(Mot4WantedLength - Mot4ActualLength) < 2 and
      abs(Mot5WantedLength - Mot5ActualLength) < 2 and
      abs(Mot6WantedLength - Mot6ActualLength) < 2 and
      abs(Mot7WantedLength - Mot7ActualLength) < 2 and
      abs(Mot8WantedLength - Mot8ActualLength) < 2)
  {
    ActualMainX = WantedMainX;
    ActualMainY = WantedMainY;
    ActualMainZ = WantedMainZ;

    ActualRotX = WantedRotX;
    ActualRotY = WantedRotY;
    ActualRotZ = WantedRotZ;

    Inpos = 1;
  };

  if (Inpos > InposOld)
  {
    NextPos = 1;
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
      if (AutoCnt > 23)
      {
        AutoCnt = 0;
      }
    }
  }
};