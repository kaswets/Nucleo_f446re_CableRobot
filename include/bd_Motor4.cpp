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
    // 16-9-2026 "spaarpot": elke tick het percentage erbij,
    // bij StepSpeed of meer een stap zetten en StepSpeed eraf.
    // Zo loopt elke motor precies in de juiste verhouding.
    int procent = max(Mot4PulseProcent, 1);       // nooit 0, anders komt de motor nooit aan
    Mot4PulseCounter = Mot4PulseCounter + procent;   // spaarpot vullen

    if (Mot4PulseCounter >= (int)StepSpeed)
    {
      Pulse4 = 1;
      Mot4PulseCounter = Mot4PulseCounter - (int)StepSpeed;   // stap betalen

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

  digitalWrite(oDir4, Mot4Direction);
  digitalWrite(oStep4, Pulse4);
};
