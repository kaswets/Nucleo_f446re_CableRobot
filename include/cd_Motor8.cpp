void Motor8()
{
  //Serial.print("Direction : ");
  //Serial.print(Mot8Direction);
  //Serial.print(" Procent : ");
  //Serial.println(Mot8PulseProcent);
 if(Mot8PulseProcent<1)
 {
  Mot8PulseProcent=1;
 }

  Mot8Direction = 0;
  if (Mot8WantedLength > Mot8ActualLength)
  {
    Mot8Direction = 1;
  }

  Pulse8 = 0;
  if (Mot8On == 1)
  {
    if (Pulse == 1)
    {
      if (Mot8WantedLength != Mot8ActualLength)
      {

        if (Mot8PulseCounter >= (StepSpeed / Mot8PulseProcent))
        {
          Pulse8 = 1;
          Mot8PulseCounter = 0;

          if (Mot8Direction == 1)
          {
            Mot8ActualLength = Mot8ActualLength + 1;
          }
          else
          {
            Mot8ActualLength = Mot8ActualLength - 1;
          };
        };
      };

      Mot8PulseCounter = Mot8PulseCounter + 1;

      digitalWrite(oDir8, Mot8Direction);
      digitalWrite(oStep8, Pulse8);
    };
  };
};
