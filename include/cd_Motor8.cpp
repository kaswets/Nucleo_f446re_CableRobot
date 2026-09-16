void Motor8()
{
  Mot8Direction = 0;
  if (Mot8WantedLength > Mot8ActualLength)
  {
    Mot8Direction = 1;
  }

  Pulse8 = 0;
  if (Mot8WantedLength != Mot8ActualLength)
  {
    // 16-9-2026 "spaarpot": elke tick het percentage erbij,
    // bij StepSpeed of meer een stap zetten en StepSpeed eraf.
    // Zo loopt elke motor precies in de juiste verhouding.
    int procent = max(Mot8PulseProcent, 1);       // nooit 0, anders komt de motor nooit aan
    Mot8PulseCounter = Mot8PulseCounter + procent;   // spaarpot vullen

    if (Mot8PulseCounter >= (int)StepSpeed)
    {
      Pulse8 = 1;
      Mot8PulseCounter = Mot8PulseCounter - (int)StepSpeed;   // stap betalen

      if (Mot8Direction == 1)
      {
        Mot8ActualLength = Mot8ActualLength + 1;
      }
      else
      {
        Mot8ActualLength = Mot8ActualLength - 1;
      };
    };
  };

  digitalWrite(oDir8, Mot8Direction);
  digitalWrite(oStep8, Pulse8);
};