#include <Arduino.h>

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// remove Serial prints for debugging
// test

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
float WidthBlockTop = 100;

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
// DEBUG 16-9-2026: tijdmeting per motor
unsigned long moveStart = 0;
unsigned long doneTime[8];
bool doneFlag[8];

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

constexpr int MaxMainX = 25;
constexpr int MaxMainY = 20;
constexpr int MaxMainZ = 25;
constexpr int MaxRotX  = 20;
constexpr int MaxRotY  = 20;
constexpr int MaxRotZ  = 20;

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

// FIX 19-9-2026: vaste, ongeroteerde basispositie van elke hoek t.o.v. het platformcentrum.
// Hieruit wordt ELKE cyclus vers de actuele hoekpositie herberekend (nooit incrementeel
// vanaf de vorige stand), zodat er geen afrondingsfouten kunnen opstapelen.
float BaseCornerX[8];
float BaseCornerY[8];
float BaseCornerZ[8];

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
void RecomputeCorners();  // FIX 19-9-2026: hoekpunten elke cyclus vers vanaf de basisvorm

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

// FIX 19-9-2026: berekent de 8 hoekpunten (ObjectCorner) en de bijbehorende
// motor-doellengtes ELKE keer vers vanaf de vaste, ongeroteerde basisvorm
// (BaseCornerX/Y/Z) plus de HUIDIGE gewenste pose (WantedMainX/Y/Z, WantedRotX/Y/Z).
// Er wordt nooit meer vanaf de vorige ObjectCorner-stand verder gerekend, zodat er
// geen afrondingsfouten kunnen opstapelen, ongeacht hoe vaak dit wordt aangeroepen.
void RecomputeCorners()
{
  for (int j = 0; j < 8; j++)
  {
    // Start steeds vanaf de vaste basispositie van deze hoek
    ObjectCorner[j].X = BaseCornerX[j];
    ObjectCorner[j].Y = BaseCornerY[j];
    ObjectCorner[j].Z = BaseCornerZ[j];

    // Absolute rotatie toepassen (zelfde teken-conventie als de oorspronkelijke code,
    // die "ActualRot - WantedRot" gebruikte; hier is de basis altijd op 0 gedefinieerd)
    matrixRotX(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, -WantedRotX, j);
    matrixRotY(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, -WantedRotY, j);
    matrixRotZ(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, -WantedRotZ, j);

    // Absolute verplaatsing toepassen (zelfde teken-conventie als de oorspronkelijke code)
    matrixTrans(ObjectCorner[j].X, ObjectCorner[j].Y, ObjectCorner[j].Z, -WantedMainX, -WantedMainY, WantedMainZ, j);
  }

  Mot1WantedLength = Mot1(ObjectCorner[0].X, ObjectCorner[0].Y, ObjectCorner[0].Z);
  Mot2WantedLength = Mot2(ObjectCorner[1].X, ObjectCorner[1].Y, ObjectCorner[1].Z);
  Mot3WantedLength = Mot3(ObjectCorner[2].X, ObjectCorner[2].Y, ObjectCorner[2].Z);
  Mot4WantedLength = Mot4(ObjectCorner[3].X, ObjectCorner[3].Y, ObjectCorner[3].Z);

  Mot5WantedLength = Mot5(ObjectCorner[4].X, ObjectCorner[4].Y, ObjectCorner[4].Z);
  Mot6WantedLength = Mot6(ObjectCorner[5].X, ObjectCorner[5].Y, ObjectCorner[5].Z);
  Mot7WantedLength = Mot7(ObjectCorner[6].X, ObjectCorner[6].Y, ObjectCorner[6].Z);
  Mot8WantedLength = Mot8(ObjectCorner[7].X, ObjectCorner[7].Y, ObjectCorner[7].Z);
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

  // FIX 19-9-2026: vaste basispositie van elke hoek (ongeroteerd, t.o.v. platformcentrum = 0,0,0).
  // Deze waarden veranderen nooit meer tijdens het draaien - alle beweging wordt hieruit
  // vers herberekend, zodat er geen fouten kunnen opstapelen over meerdere commando's heen.
  BaseCornerX[0] = -(LengthBlockBottom / 2.0); BaseCornerY[0] = -(WidthBlockBottom / 2.0); BaseCornerZ[0] = -(HighBlock / 2.0);
  BaseCornerX[1] = -(LengthBlockBottom / 2.0); BaseCornerY[1] =  (WidthBlockBottom / 2.0); BaseCornerZ[1] = -(HighBlock / 2.0);
  BaseCornerX[2] =  (LengthBlockBottom / 2.0); BaseCornerY[2] = -(WidthBlockBottom / 2.0); BaseCornerZ[2] = -(HighBlock / 2.0);
  BaseCornerX[3] =  (LengthBlockBottom / 2.0); BaseCornerY[3] =  (WidthBlockBottom / 2.0); BaseCornerZ[3] = -(HighBlock / 2.0);

  BaseCornerX[4] = -(LengthBlockTop / 2.0); BaseCornerY[4] = -(WidthBlockTop / 2.0); BaseCornerZ[4] = (HighBlock / 2.0);
  BaseCornerX[5] = -(LengthBlockTop / 2.0); BaseCornerY[5] =  (WidthBlockTop / 2.0); BaseCornerZ[5] = (HighBlock / 2.0);
  BaseCornerX[6] =  (LengthBlockTop / 2.0); BaseCornerY[6] = -(WidthBlockTop / 2.0); BaseCornerZ[6] = (HighBlock / 2.0);
  BaseCornerX[7] =  (LengthBlockTop / 2.0); BaseCornerY[7] =  (WidthBlockTop / 2.0); BaseCornerZ[7] = (HighBlock / 2.0);

  // Hoekpunten en motorlengtes voor de start-pose (alles 0) berekenen en als "aangekomen" markeren
  RecomputeCorners();
  Mot1ActualLength = Mot1WantedLength;
  Mot2ActualLength = Mot2WantedLength;
  Mot3ActualLength = Mot3WantedLength;
  Mot4ActualLength = Mot4WantedLength;
  Mot5ActualLength = Mot5WantedLength;
  Mot6ActualLength = Mot6WantedLength;
  Mot7ActualLength = Mot7WantedLength;
  Mot8ActualLength = Mot8WantedLength;

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
  // FIX 19-9-2026: hoekpunten en motor-doellengtes ELKE cyclus vers herberekenen vanaf de
  // vaste basisvorm (zie RecomputeCorners hieronder) - nooit meer incrementeel vanaf de
  // vorige, mogelijk al afgeweken stand. Dit voorkomt dat kleine afrondingsfouten zich
  // opstapelen, ook als er (zoals bij SimHub/een spel) continu nieuwe doelen binnenkomen
  // terwijl het platform nog niet "klaar" is met de vorige beweging.
  RecomputeCorners();

  // Snelheidsverhouding tussen de motoren elke cyclus opnieuw bepalen, zodat ze synchroon
  // blijven aankomen - ook als het doel ondertussen weer verandert.
  TotalMove[0] = abs(Mot1WantedLength - Mot1ActualLength);
  TotalMove[1] = abs(Mot2WantedLength - Mot2ActualLength);
  TotalMove[2] = abs(Mot3WantedLength - Mot3ActualLength);
  TotalMove[3] = abs(Mot4WantedLength - Mot4ActualLength);

  TotalMove[4] = abs(Mot5WantedLength - Mot5ActualLength);
  TotalMove[5] = abs(Mot6WantedLength - Mot6ActualLength);
  TotalMove[6] = abs(Mot7WantedLength - Mot7ActualLength);
  TotalMove[7] = abs(Mot8WantedLength - Mot8ActualLength);

  long maxMove = 0;
  for (int i = 0; i < 8; i++)
  {
    if (TotalMove[i] > maxMove) maxMove = TotalMove[i];
  }

  if (maxMove >= 1)
  {
    if (Inpos == 1)
    {
      // Nieuwe beweging start vanuit stilstand: tijdmeting resetten
      moveStart = millis();
      for (int i = 0; i < 8; i++) { doneFlag[i] = false; doneTime[i] = 0; }
    }
    Inpos = 0;

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
  // DEBUG 16-9-2026: noteer per motor wanneer hij klaar is
  if (Inpos == 0)
  {
    long rest[8] = {Mot1WantedLength - Mot1ActualLength, Mot2WantedLength - Mot2ActualLength,
                    Mot3WantedLength - Mot3ActualLength, Mot4WantedLength - Mot4ActualLength,
                    Mot5WantedLength - Mot5ActualLength, Mot6WantedLength - Mot6ActualLength,
                    Mot7WantedLength - Mot7ActualLength, Mot8WantedLength - Mot8ActualLength};
    for (int i = 0; i < 8; i++)
    {
      if (!doneFlag[i] && abs(rest[i]) <= 1)
      {
        doneFlag[i] = true;
        doneTime[i] = millis() - moveStart;
      }
    }
  }

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
    // DEBUG 16-9-2026: beweging klaar, toon eindstand per motor
    Serial.println("---- KLAAR ----");
    long wl2[8] = {Mot1WantedLength, Mot2WantedLength, Mot3WantedLength, Mot4WantedLength,
                   Mot5WantedLength, Mot6WantedLength, Mot7WantedLength, Mot8WantedLength};
    long al2[8] = {Mot1ActualLength, Mot2ActualLength, Mot3ActualLength, Mot4ActualLength,
                   Mot5ActualLength, Mot6ActualLength, Mot7ActualLength, Mot8ActualLength};
    for (int i = 0; i < 8; i++)
    {
      Serial.print("M"); Serial.print(i + 1);
      Serial.print("  staat op="); Serial.print(al2[i]);
      Serial.print("  verschil met doel="); Serial.print(wl2[i] - al2[i]);
      Serial.print("  klaar na "); Serial.print(doneTime[i]); Serial.println(" ms");
    }
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