void ParseCommand(String Command)
{

  // dit gedeelte leest de data welke via Serial binnenkomt,
  // en filter het commando er uit
  //
  // We hebben de onderstaande kommandos
  // Main x,y,z                 new wanted position

  // Xas value                  new wanted X
  // Yas value                  new wanted Y
  // Zas value                  new wanted z

  // Roll                       turn around x-axis
  // Pitch                      turn around y-axis
  // yaw                        tun around z-axis

  String partCommand;
  String partData1;
  String partData2;
  String partData3;

  partCommand = Command.substring(0, Command.indexOf(" "));
  partData1 = Command.substring(Command.indexOf(" ") + 1, Command.indexOf(","));
  partData2 = Command.substring(Command.indexOf(",") + 1, Command.indexOf(","));
  partData2 = Command.substring(Command.indexOf(",") + 1);

  Serial.print(" ParseCommand: ");
  Serial.print(partCommand);
  Serial.print(" ");
  Serial.print(partData1);
  Serial.print(",");
  Serial.println(partData2);
  Serial.print(",");
  Serial.println(partData3);

  int intData1 = partData1.toInt();
  int intData2 = partData2.toInt();
  int intData3 = partData3.toInt();

  
  if (partCommand.equalsIgnoreCase("Report")) // Report To Serial USB
  {
    Serial.print("mot1 ");
    Serial.print(Mot1WantedLength);
    Serial.print(" - ");
    Serial.println(Mot1ActualLength);

    Serial.print("mot2 ");
    Serial.print(Mot2WantedLength);
    Serial.print(" - ");
    Serial.println(Mot2ActualLength);

    Serial.print("mot3 ");
    Serial.print(Mot3WantedLength);
    Serial.print(" - ");
    Serial.println(Mot3ActualLength);

    Serial.print("mot4 ");
    Serial.print(Mot4WantedLength);
    Serial.print(" - ");
    Serial.println(Mot4ActualLength);

    Serial.print("mot5 ");
    Serial.print(Mot5WantedLength);
    Serial.print(" - ");
    Serial.println(Mot5ActualLength);

    Serial.print("mot6 ");
    Serial.print(Mot6WantedLength);
    Serial.print(" - ");
    Serial.println(Mot6ActualLength);

    Serial.print("mot7 ");
    Serial.print(Mot7WantedLength);
    Serial.print(" - ");
    Serial.println(Mot7ActualLength);

    Serial.print("mot8 ");
    Serial.print(Mot8WantedLength);
    Serial.print(" - ");
    Serial.println(Mot8ActualLength);
  }
  else if (partCommand.equalsIgnoreCase("MainX"))
  {
    WantedMainX = intData1;
    Serial.print("_X_:");
    Serial.print(WantedMainX);
    Serial.print(" , Y :");
    Serial.print(WantedMainY);
    Serial.print(" , Z :");
    Serial.println(WantedMainZ);
  }

  else if (partCommand.equalsIgnoreCase("MainY"))
  {
    WantedMainY = intData1;
    Serial.print(" X :");
    Serial.print(WantedMainX);
    Serial.print(" ,_Y_:");
    Serial.print(WantedMainY);
    Serial.print(" , Z :");
    Serial.println(WantedMainZ);
  }
  else if (partCommand.equalsIgnoreCase("MainZ"))
  {
    WantedMainZ = intData1;
    Serial.print(" X :");
    Serial.print(WantedMainX);
    Serial.print(" , Y :");
    Serial.print(WantedMainY);
    Serial.print(" ,_Z_:");
    Serial.println(WantedMainZ);
  }
  else if (partCommand.equalsIgnoreCase("RotX"))
  {
    WantedRotX = intData1;
    Serial.print(" X :");
    Serial.print(WantedMainX);
    Serial.print(" , Y :");
    Serial.print(WantedMainY);
    Serial.print(" , Z :");
    Serial.println(WantedMainZ);
  }
  else if (partCommand.equalsIgnoreCase("RotY"))
  {
    WantedRotY = intData1;
    Serial.print(" X :");
    Serial.print(WantedMainX);
    Serial.print(" , Y :");
    Serial.print(WantedMainY);
    Serial.print(" , Z :");
    Serial.println(WantedMainZ);
  }
  else if (partCommand.equalsIgnoreCase("RotZ"))
  {
    WantedRotZ = intData1;
    Serial.print(" X :");
    Serial.print(WantedMainX);
    Serial.print(" , Y :");
    Serial.print(WantedMainY);
    Serial.print(" , Z :");
    Serial.println(WantedMainZ);
  }
  else if (partCommand.equalsIgnoreCase("AutoOn")) // Report To Serial USB
  {
    AutoMove = 1;
    if (Inpos == 1)
    {
      NextPos = 1;
    }
  }
  else if (partCommand.equalsIgnoreCase("AutoOff")) // Report To Serial USB
  {
    AutoMove = 0;
  }
  else
  {
    Serial.println("Unknow command");
  }
}
