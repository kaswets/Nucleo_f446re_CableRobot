void Motor3()
{
  //Serial.print("Direction : ");
  //Serial.print(Mot3Direction);
  //Serial.print(" Procent : ");
  //Serial.println(Mot3PulseProcent);
 if(Mot3PulseProcent<1)
 {
  Mot3PulseProcent=1;
 }

  Mot3Direction = 0;
  if (Mot3WantedLength > Mot3ActualLength)
  {
    Mot3Direction = 1;
  }

  Pulse3 = 0;
  if (Mot3On == 1)
  {
    if (Pulse == 1)
    {
      if (Mot3WantedLength != Mot3ActualLength)
      {

        if (Mot3PulseCounter >= (StepSpeed / Mot3PulseProcent))
        {
          Pulse3 = 1;
          Mot3PulseCounter = 0;

          if (Mot3Direction == 1)
          {
            Mot3ActualLength = Mot3ActualLength + 1;
          }
          else
          {
            Mot3ActualLength = Mot3ActualLength - 1;
          };
        };
      };

      Mot3PulseCounter = Mot3PulseCounter + 1;

      digitalWrite(oDir3, Mot3Direction);
      digitalWrite(oStep3, Pulse3);
    };
  };
};
