void Motor7()
{
  //Serial.print("Direction : ");
  //Serial.print(Mot7Direction);
  //Serial.print(" Procent : ");
  //Serial.println(Mot7PulseProcent);
 if(Mot7PulseProcent<1)
 {
  Mot7PulseProcent=1;
 }

  Mot7Direction = 0;
  if (Mot7WantedLength > Mot7ActualLength)
  {
    Mot7Direction = 1;
  }

  Pulse7 = 0;
  if (Mot7On == 1)
  {
    if (Pulse == 1)
    {
      if (Mot7WantedLength != Mot7ActualLength)
      {

        if (Mot7PulseCounter >= (StepSpeed / Mot7PulseProcent))
        {
          Pulse7 = 1;
          Mot7PulseCounter = 0;

          if (Mot7Direction == 1)
          {
            Mot7ActualLength = Mot7ActualLength + 1;
          }
          else
          {
            Mot7ActualLength = Mot7ActualLength - 1;
          };
        };
      };

      Mot7PulseCounter = Mot7PulseCounter + 1;

      digitalWrite(oDir7, Mot7Direction);
      digitalWrite(oStep7, Pulse7);
    };
  };
};
