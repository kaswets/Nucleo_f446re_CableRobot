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
    // 16-9-2026 "spaarpot": elke tick het percentage erbij,
    // bij StepSpeed of meer een stap zetten en StepSpeed eraf.
    // Zo loopt elke motor precies in de juiste verhouding.
    int procent = max(Mot6PulseProcent, 1);       // nooit 0, anders komt de motor nooit aan
    Mot6PulseCounter = Mot6PulseCounter + procent;   // spaarpot vullen

    if (Mot6PulseCounter >= (int)StepSpeed)
    {
      Pulse6 = 1;
      Mot6PulseCounter = Mot6PulseCounter - (int)StepSpeed;   // stap betalen

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

  digitalWrite(oDir6, Mot6Direction);
  digitalWrite(oStep6, Pulse6);
};
