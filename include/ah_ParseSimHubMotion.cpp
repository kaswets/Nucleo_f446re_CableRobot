// ah_ParseSimHubMotion.cpp - COMPLETE FILE for SH prefix and ±25mm workspace

void parseSimHubMotion(String data) {
    // Handle both "SH" (new) and "M" (legacy) prefixes
    if (data.startsWith("SH")) {
        data = data.substring(2); // strip 'SH'
    } else if (data.startsWith("M")) {
        data = data.substring(1); // strip 'M'
    }

    float values[4] = {127, 127, 127, 127};  // FL, FR, RL, RR defaults
    int index = 0;
    int startPos = 0;

    // Parse comma-separated values
    for (int i = 0; i <= data.length() && index < 4; i++) {
        if (data.charAt(i) == ',' || i == data.length()) {
            values[index] = data.substring(startPos, i).toFloat();
            index++;
            startPos = i + 1;
        }
    }

    float FL = values[0];  // Front Left corner
    float FR = values[1];  // Front Right corner  
    float RL = values[2];  // Rear Left corner
    float RR = values[3];  // Rear Right corner

    // Bereken bewegingscomponenten via gemiddelden
    float avgFront = (FL + FR) / 2.0;  // Front corners average
    float avgRear  = (RL + RR) / 2.0;  // Rear corners average
    float avgLeft  = (FL + RL) / 2.0;  // Left corners average
    float avgRight = (FR + RR) / 2.0;  // Right corners average
    float avgAll   = (FL + FR + RL + RR) / 4.0;  // Overall average

    // Motion calculations aangepast voor RELATIEVE beweging (center = geen beweging)
    // Heave: Overall platform height - relative to center position
    float heaveMotion = map(avgAll, 0, 255, -20, 20);  // Convert to motion range
    WantedMainZ = heaveMotion;  // Direct relative motion
    
    // Roll: Left/Right tilt (verschil tussen left en right gemiddelden)
    float rollDiff = avgLeft - avgRight;  // Positive = left side higher
    float rollMotion = constrain(rollDiff * 0.15, -6, 6);  // Scale to ±6 degrees
    WantedRotX = rollMotion;  // Direct relative motion
    
    // Pitch: Front/Back tilt (verschil tussen front en rear gemiddelden)  
    float pitchDiff = avgFront - avgRear;  // Positive = front higher
    float pitchMotion = constrain(pitchDiff * 0.15, -6, 6);  // Scale to ±6 degrees
    WantedRotY = pitchMotion;  // Direct relative motion
    
    // Center detection - return to 0,0,0 when values are close to center (127)
    // Check if all individual values are close to 127 (within ±3 points)
    bool isNearCenter = (abs(FL - 127) <= 3 && abs(FR - 127) <= 3 && 
                        abs(RL - 127) <= 3 && abs(RR - 127) <= 3);
    
    if (isNearCenter) {
        // Gradual return to center for smooth motion
        WantedMainZ = WantedMainZ * 0.8;  // Fade to zero
        WantedRotX = WantedRotX * 0.8;   // Fade to zero
        WantedRotY = WantedRotY * 0.8;   // Fade to zero
        
        // Force to exact zero when very close
        if (abs(WantedMainZ) < 0.5) WantedMainZ = 0;
        if (abs(WantedRotX) < 0.2) WantedRotX = 0;
        if (abs(WantedRotY) < 0.2) WantedRotY = 0;
    }

    // Anti-tril filtering (verbeterd van jouw originele code)
    if (abs(WantedMainZ - ActualMainZ) < 1) WantedMainZ = ActualMainZ;
    if (abs(WantedRotX) < 0.5) WantedRotX = 0;  // Smaller deadzone for rotations
    if (abs(WantedRotY) < 0.5) WantedRotY = 0;

    // Enhanced debug output
    Serial.print("SH Raw: FL=");
    Serial.print(FL); Serial.print(" FR="); Serial.print(FR);
    Serial.print(" RL="); Serial.print(RL); Serial.print(" RR=");
    Serial.print(RR);
    
    Serial.print(" | Avg: All="); Serial.print(avgAll, 1);
    Serial.print(" L="); Serial.print(avgLeft, 1); 
    Serial.print(" R="); Serial.print(avgRight, 1);
    Serial.print(" F="); Serial.print(avgFront, 1);
    Serial.print(" Rear="); Serial.println(avgRear, 1);

    Serial.print("SH Motion: Z=");
    Serial.print(WantedMainZ); Serial.print("mm");
    Serial.print(" RotX="); Serial.print(WantedRotX, 1); Serial.print("°");
    Serial.print(" RotY="); Serial.print(WantedRotY, 1); Serial.println("°");
    
    // Safety check - emergency stop bij extreme waarden
    if (abs(WantedMainZ) > 25 || abs(WantedRotX) > 8 || abs(WantedRotY) > 8) {
        WantedMainZ = constrain(WantedMainZ, -20, 20);  // Force within safe limits
        WantedRotX = constrain(WantedRotX, -6, 6); 
        WantedRotY = constrain(WantedRotY, -6, 6);
        Serial.println("SH SAFETY: Motion limited to safe range!");
    }
    
    // Update SimHub activity timestamp
    lastSimHubData = millis();

    Serial.println("SH PARSED!"); // Simple test message
}