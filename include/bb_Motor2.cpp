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
    // 16-9-2026 "spaarpot": elke tick het percentage erbij,
    // bij StepSpeed of meer een stap zetten en StepSpeed eraf.
    // Zo loopt elke motor precies in de juiste verhouding.
    int procent = max(Mot2PulseProcent, 1);       // nooit 0, anders komt de motor nooit aan
    Mot2PulseCounter = Mot2PulseCounter + procent;   // spaarpot vullen

    if (Mot2PulseCounter >= (int)StepSpeed)
    {
      Pulse2 = 1;
      Mot2PulseCounter = Mot2PulseCounter - (int)StepSpeed;   // stap betalen

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

  digitalWrite(oDir2, Mot2Direction);
  digitalWrite(oStep2, Pulse2);
};
