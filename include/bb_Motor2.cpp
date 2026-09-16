void Motor2()
{
  Mot2Direction = 0;
  if (Mot2WantedLength > Mot2ActualLength)
  {
    Mot2Direction = 1;
  }

  Pulse2 = 0;
  if (Mot2WantedLength != Mot2ActualLength)
  {
    if (Mot2PulseCounter >= (StepSpeed / Mot2PulseProcent))
    {
      Pulse2 = 1;
      Mot2PulseCounter = 0;

      if (Mot2Direction == 1)
      {
        Mot2ActualLength = Mot2ActualLength + 1;
      }
      else
      {
        Mot2ActualLength = Mot2ActualLength - 1;
      };
    };
  };

  Mot2PulseCounter = Mot2PulseCounter + 1;

  digitalWrite(oDir2, Mot2Direction);
  digitalWrite(oStep2, Pulse2);
};
