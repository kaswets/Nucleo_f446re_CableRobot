void Motor1()
{
  Mot1Direction = 0;
  if (Mot1WantedLength > Mot1ActualLength)
  {
    Mot1Direction = 1;
  }

  Pulse1 = 0;
  if (Mot1WantedLength != Mot1ActualLength)
  {
    if (Mot1PulseCounter >= (StepSpeed / Mot1PulseProcent))
    {
      Pulse1 = 1;
      Mot1PulseCounter = 0;

      if (Mot1Direction == 1)
      {
        Mot1ActualLength = Mot1ActualLength + 1;
      }
      else
      {
        Mot1ActualLength = Mot1ActualLength - 1;
      };
    };
  };

  Mot1PulseCounter = Mot1PulseCounter + 1;

  digitalWrite(oDir1, Mot1Direction);
  digitalWrite(oStep1, Pulse1);
};
