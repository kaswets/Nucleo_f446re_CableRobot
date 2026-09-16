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
    // 16-9-2026 "spaarpot": elke tick het percentage erbij,
    // bij StepSpeed of meer een stap zetten en StepSpeed eraf.
    // Zo loopt elke motor precies in de juiste verhouding.
    int procent = max(Mot1PulseProcent, 1);       // nooit 0, anders komt de motor nooit aan
    Mot1PulseCounter = Mot1PulseCounter + procent;   // spaarpot vullen

    if (Mot1PulseCounter >= (int)StepSpeed)
    {
      Pulse1 = 1;
      Mot1PulseCounter = Mot1PulseCounter - (int)StepSpeed;   // stap betalen

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

  digitalWrite(oDir1, Mot1Direction);
  digitalWrite(oStep1, Pulse1);
};
