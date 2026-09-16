// ag_ParseData.cpp - COMPLETE VERSIE MET SIMHUB + SIMTOOLS SUPPORT
// Globale variabelen staan in main file!

// Helper functie: tel komma's
int countCommas(String str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str.charAt(i) == ',') {
      count++;
    }
  }
  return count;
}

// SimHub data parser (call your existing ah_ParseSimHubMotion function)
void parseSimHubData(String data) {
  Serial.print("SimHub data: ");
  Serial.println(data);
  
  // Call your existing SimHub parser function
  parseSimHubMotion(data);  // This calls your ah_ParseSimHubMotion.cpp function
}

// SimTools data parser
void parseSimToolsData(String data) {
  Serial.print("SimTools data: ");
  Serial.println(data);
  
  // Parse 6 comma-separated values
  float values[6];
  int valueIndex = 0;
  int startPos = 0;
  
  for (int i = 0; i <= data.length() && valueIndex < 6; i++) {
    if (data.charAt(i) == ',' || i == data.length()) {
      String valueStr = data.substring(startPos, i);
      values[valueIndex] = valueStr.toFloat();
      startPos = i + 1;
      valueIndex++;
    }
  }
  
  if (valueIndex == 6) {
    // Convert SimTools range (0-1000, center=500) to motion values
    float roll = (values[0] - 500.0) / 500.0;    // -1.0 to +1.0
    float pitch = (values[1] - 500.0) / 500.0;
    float yaw = (values[2] - 500.0) / 500.0;
    float surge = (values[3] - 500.0) / 500.0;
    float sway = (values[4] - 500.0) / 500.0;
    float heave = (values[5] - 500.0) / 500.0;
    
    // Apply scaling and limits
    float scaledRotX = constrain(roll * SIMTOOLS_ROTATION_SCALE * MAX_ROTATION, 
                                -MAX_ROTATION, MAX_ROTATION);
    float scaledRotY = constrain(pitch * SIMTOOLS_ROTATION_SCALE * MAX_ROTATION, 
                                -MAX_ROTATION, MAX_ROTATION);
    float scaledRotZ = constrain(yaw * SIMTOOLS_ROTATION_SCALE * MAX_ROTATION, 
                                -MAX_ROTATION, MAX_ROTATION);
    
    float scaledX = constrain(surge * SIMTOOLS_TRANSLATION_SCALE * MAX_TRANSLATION, 
                             -MAX_TRANSLATION, MAX_TRANSLATION);
    float scaledY = constrain(sway * SIMTOOLS_TRANSLATION_SCALE * MAX_TRANSLATION, 
                             -MAX_TRANSLATION, MAX_TRANSLATION);
    float scaledZ = constrain(heave * SIMTOOLS_TRANSLATION_SCALE * MAX_TRANSLATION, 
                             -MAX_TRANSLATION, MAX_TRANSLATION);
    
    // Set nieuwe motion targets
    WantedRotX = scaledRotX;
    WantedRotY = scaledRotY;
    WantedRotZ = scaledRotZ;
    
    WantedMainX = scaledX;  // Relatief ten opzichte van center
    WantedMainY = scaledY;
    WantedMainZ = scaledZ;
    
    // Debug output
    Serial.print("Motion -> Roll:");
    Serial.print(scaledRotX, 1);
    Serial.print(" Pitch:");
    Serial.print(scaledRotY, 1);
    Serial.print(" Yaw:");
    Serial.print(scaledRotZ, 1);
    Serial.print(" X:");
    Serial.print(scaledX, 1);
    Serial.print(" Y:");
    Serial.print(scaledY, 1);
    Serial.print(" Z:");
    Serial.println(scaledZ, 1);
    
    // Safety check
    float totalRotation = abs(WantedRotX) + abs(WantedRotY) + abs(WantedRotZ);
    float totalTranslation = abs(WantedMainX) + abs(WantedMainY) + abs(WantedMainZ);
    
    if (totalRotation > MAX_ROTATION * 2.0 || totalTranslation > MAX_TRANSLATION * 2.0) {
      // Emergency stop
      WantedMainX = 0;
      WantedMainY = 0;
      WantedMainZ = 0;
      WantedRotX = 0;
      WantedRotY = 0;
      WantedRotZ = 0;
      Serial.println("EMERGENCY STOP - Motion limits exceeded");
    }
  }
}

// checkSimToolsTimeout() functie staat nu in main file

void ParseCommand(String Command)
{
  // DEBUG: Laat alle binnenkomende data zien
  Serial.print("Received: '");
  Serial.print(Command);
  Serial.print("' (length: ");
  Serial.print(Command.length());
  Serial.println(")");
  
  // 1. CHECK: SimHub data (SH prefix + 3 commas = 4 values)
  if (Command.startsWith("SH") && countCommas(Command) == 3) {
    parseSimHubData(Command);
    simToolsMode = false;  // We're in SimHub mode, not SimTools
    lastSimHubData = millis();  // Update timestamp (add this variable to main)
    return;  // Stop processing, SimHub data handled
  }
  
  // 2. CHECK: SimTools comma-separated data (6 values, no prefix)
  if (Command.indexOf(",") > 0 && countCommas(Command) == 5 && !Command.startsWith("SH")) {
    parseSimToolsData(Command);
    simToolsMode = true;
    lastSimToolsData = millis();
    return;  // Stop processing, SimTools data handled
  }
  
  // 3. Manual commands (jouw bestaande code, opgeschoond)
  String partCommand;
  String partData1;
  
  partCommand = Command.substring(0, Command.indexOf(" "));
  
  if (Command.indexOf(" ") > 0) {
    partData1 = Command.substring(Command.indexOf(" ") + 1);
  }
  
  int intData1 = partData1.toInt();
  
  Serial.print("Manual command: ");
  Serial.print(partCommand);
  Serial.print(" = ");
  Serial.println(intData1);
  
  // Manual command processing
  if (partCommand.equalsIgnoreCase("Report")) {
    Serial.println("=== MOTOR STATUS ===");
    Serial.print("mot1: "); Serial.print(Mot1WantedLength); Serial.print(" - "); Serial.println(Mot1ActualLength);
    Serial.print("mot2: "); Serial.print(Mot2WantedLength); Serial.print(" - "); Serial.println(Mot2ActualLength);
    Serial.print("mot3: "); Serial.print(Mot3WantedLength); Serial.print(" - "); Serial.println(Mot3ActualLength);
    Serial.print("mot4: "); Serial.print(Mot4WantedLength); Serial.print(" - "); Serial.println(Mot4ActualLength);
    Serial.print("mot5: "); Serial.print(Mot5WantedLength); Serial.print(" - "); Serial.println(Mot5ActualLength);
    Serial.print("mot6: "); Serial.print(Mot6WantedLength); Serial.print(" - "); Serial.println(Mot6ActualLength);
    Serial.print("mot7: "); Serial.print(Mot7WantedLength); Serial.print(" - "); Serial.println(Mot7ActualLength);
    Serial.print("mot8: "); Serial.print(Mot8WantedLength); Serial.print(" - "); Serial.println(Mot8ActualLength);
    Serial.println("=== END REPORT ===");
  }
  else if (partCommand.equalsIgnoreCase("MainX")) {
    WantedMainX = intData1;
    Serial.print("Manual MainX set to: "); Serial.println(WantedMainX);
  }
  else if (partCommand.equalsIgnoreCase("MainY")) {
    WantedMainY = intData1;
    Serial.print("Manual MainY set to: "); Serial.println(WantedMainY);
  }
  else if (partCommand.equalsIgnoreCase("MainZ")) {
    WantedMainZ = intData1;
    Serial.print("Manual MainZ set to: "); Serial.println(WantedMainZ);
  }
  else if (partCommand.equalsIgnoreCase("RotX")) {
    WantedRotX = intData1;
    Serial.print("Manual RotX set to: "); Serial.println(WantedRotX);
  }
  else if (partCommand.equalsIgnoreCase("RotY")) {
    WantedRotY = intData1;
    Serial.print("Manual RotY set to: "); Serial.println(WantedRotY);
  }
  else if (partCommand.equalsIgnoreCase("RotZ")) {
    WantedRotZ = intData1;
    Serial.print("Manual RotZ set to: "); Serial.println(WantedRotZ);
  }
  else if (partCommand.equalsIgnoreCase("AutoOn")) {
    AutoMove = 1;
    Serial.println("Auto movement ON");
    if (Inpos == 1) {
      NextPos = 1;
    }
  }
  else if (partCommand.equalsIgnoreCase("AutoOff")) {
    AutoMove = 0;
    Serial.println("Auto movement OFF");
  }
  else if (partCommand.equalsIgnoreCase("Status")) {
    Serial.println("=== CURRENT STATUS ===");
    Serial.print("Position: X="); Serial.print(ActualMainX); 
    Serial.print(" Y="); Serial.print(ActualMainY); 
    Serial.print(" Z="); Serial.println(ActualMainZ);
    Serial.print("Rotation: RX="); Serial.print(ActualRotX); 
    Serial.print(" RY="); Serial.print(ActualRotY); 
    Serial.print(" RZ="); Serial.println(ActualRotZ);
    Serial.print("InPos: "); Serial.println(Inpos ? "YES" : "NO");
    Serial.print("AutoMove: "); Serial.println(AutoMove ? "ON" : "OFF");
    Serial.print("SimTools: "); Serial.println(simToolsMode ? "ACTIVE" : "INACTIVE");
    
    // Add SimHub status
    Serial.print("SimHub: ");
    if (millis() - lastSimHubData < 2000) {
      Serial.println("ACTIVE");
    } else {
      Serial.println("INACTIVE");
    }
    Serial.println("=== END STATUS ===");
  }
  else if (partCommand.equalsIgnoreCase("Center")) {
    // Return to center position
    WantedMainX = 0;
    WantedMainY = 0;
    WantedMainZ = 0;
    WantedRotX = 0;
    WantedRotY = 0;
    WantedRotZ = 0;
    Serial.println("Returning to center position");
  }
  else {
    Serial.print("Unknown command: ");
    Serial.println(partCommand);
    Serial.println("Available commands: MainX, MainY, MainZ, RotX, RotY, RotZ, AutoOn, AutoOff, Report, Status, Center");
  }
}