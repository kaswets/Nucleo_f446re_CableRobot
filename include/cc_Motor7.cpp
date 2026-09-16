void Motor7()
{
  Mot7Direction = 0;
  if (Mot7WantedLength > Mot7ActualLength)
  {
    Mot7Direction = 1;
  }

  Pulse7 = 0;
  if (Mot7WantedLength != Mot7ActualLength)
  {
    // 16-9-2026 "spaarpot": elke tick het percentage erbij,
    // bij StepSpeed of meer een stap zetten en StepSpeed eraf.
    // Zo loopt elke motor precies in de juiste verhouding.
    int procent = max(Mot7PulseProcent, 1);       // nooit 0, anders komt de motor nooit aan
    Mot7PulseCounter = Mot7PulseCounter + procent;   // spaarpot vullen

    if (Mot7PulseCounter >= (int)StepSpeed)
    {
      Pulse7 = 1;
      Mot7PulseCounter = Mot7PulseCounter - (int)StepSpeed;   // stap betalen

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

  digitalWrite(oDir7, Mot7Direction);
  digitalWrite(oStep7, Pulse7);
};
