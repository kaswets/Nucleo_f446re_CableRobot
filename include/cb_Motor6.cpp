void Motor6()
{
  Mot6Direction = 0;
  if (Mot6WantedLength > Mot6ActualLength)
  {
    Mot6Direction = 1;
  }

  Pulse6 = 0;
  if (Mot6WantedLength != Mot6ActualLength)
  {
    if (Mot6PulseCounter >= (StepSpeed / Mot6PulseProcent))
    {
      Pulse6 = 1;
      Mot6PulseCounter = 0;

      if (Mot6Direction == 1)
      {
        Mot6ActualLength = Mot6ActualLength + 1;
      }
      else
      {
        Mot6ActualLength = Mot6ActualLength - 1;
      };
    };
  };

  Mot6PulseCounter = Mot6PulseCounter + 1;

  digitalWrite(oDir6, Mot6Direction);
  digitalWrite(oStep6, Pulse6);
};
