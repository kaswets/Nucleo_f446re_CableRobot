void Motor3()
{
  Mot3Direction = 0;
  if (Mot3WantedLength > Mot3ActualLength)
  {
    Mot3Direction = 1;
  }

  Pulse3 = 0;
  if (Mot3WantedLength != Mot3ActualLength)
  {
    // 16-9-2026 "spaarpot": elke tick het percentage erbij,
    // bij StepSpeed of meer een stap zetten en StepSpeed eraf.
    // Zo loopt elke motor precies in de juiste verhouding.
    int procent = max(Mot3PulseProcent, 1);       // nooit 0, anders komt de motor nooit aan
    Mot3PulseCounter = Mot3PulseCounter + procent;   // spaarpot vullen

    if (Mot3PulseCounter >= (int)StepSpeed)
    {
      Pulse3 = 1;
      Mot3PulseCounter = Mot3PulseCounter - (int)StepSpeed;   // stap betalen

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

  digitalWrite(oDir3, Mot3Direction);
  digitalWrite(oStep3, Pulse3);
};
