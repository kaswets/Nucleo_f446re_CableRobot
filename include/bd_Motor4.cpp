void Motor4()
{
  Mot4Direction = 0;
  if (Mot4WantedLength > Mot4ActualLength)
  {
    Mot4Direction = 1;
  }

  Pulse4 = 0;
  if (Mot4WantedLength != Mot4ActualLength)
  {
    if (Mot4PulseCounter >= (StepSpeed / Mot4PulseProcent))
    {
      Pulse4 = 1;
      Mot4PulseCounter = 0;

      if (Mot4Direction == 1)
      {
        Mot4ActualLength = Mot4ActualLength + 1;
      }
      else
      {
        Mot4ActualLength = Mot4ActualLength - 1;
      };
    };
  };

  Mot4PulseCounter = Mot4PulseCounter + 1;

  digitalWrite(oDir4, Mot4Direction);
  digitalWrite(oStep4, Pulse4);
};
