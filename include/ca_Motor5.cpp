void Motor5()
{
  Mot5Direction = 0;
  if (Mot5WantedLength > Mot5ActualLength)
  {
    Mot5Direction = 1;
  }

  Pulse5 = 0;
  if (Mot5WantedLength != Mot5ActualLength)
  {
    // 16-9-2026 "spaarpot": elke tick het percentage erbij,
    // bij StepSpeed of meer een stap zetten en StepSpeed eraf.
    // Zo loopt elke motor precies in de juiste verhouding.
    int procent = max(Mot5PulseProcent, 1);       // nooit 0, anders komt de motor nooit aan
    Mot5PulseCounter = Mot5PulseCounter + procent;   // spaarpot vullen

    if (Mot5PulseCounter >= (int)StepSpeed)
    {
      Pulse5 = 1;
      Mot5PulseCounter = Mot5PulseCounter - (int)StepSpeed;   // stap betalen

      if (Mot5Direction == 1)
      {
        Mot5ActualLength = Mot5ActualLength + 1;
      }
      else
      {
        Mot5ActualLength = Mot5ActualLength - 1;
      };
    };
  };

  digitalWrite(oDir5, Mot5Direction);
  digitalWrite(oStep5, Pulse5);
};
