// include libraries
#include <Arduino.h>
#include "F1_typ.h"

// Declare global variables/objects
F1_typ unit;

// --------------------------------------------------------------------------------------------------------------------
// Initialization procedure (to be run once)
void setup() {
  // Set up pre-initialization variables
  unit.internal.configFileName = "config.json";
  unit.mode.customConfig = 0;

  // If in debug mode, start serial comms
  if(unit.mode.useSerialComms){
    Serial.begin(9600); 
  }

  // Initialize this unit's members
  unit.status.isConfigured = 0;
  unit.Init();
  
}
// --------------------------------------------------------------------------------------------------------------------
// Main executable
void loop() {
  
  // Get the new RF signal
  unit.status.rfSignal = digitalRead(unit.internal.rfReceiverPin);

  if (unit.status.rfSignal == HIGH) {
    
    switch (unit.status.modeNum) {
      case 0: // Mode: useCycleTimer
        unit.runCycle();
        delay(unit.internal.secBetweenCycles*1000);
        break;

      case 1: // Mode: runContinuously
        unit.run1Rev();
        delay(10); 
        break;
      
      case 2: // Mode: turnOffEachCycle
        // Check that the HIGH signal is a new signal (compared to the last scan)
        if(unit.status.rfSignal_ == LOW) {
          unit.runCycle();
        }
        break;
      
      case -1: // Error 
        if(unit.mode.useSerialComms) {
          Serial.println("Error State");
          unit.status.error = 1;
        }
        break;
      }
  }

  // Update the last scan value
  unit.status.rfSignal_ = unit.status.rfSignal;
  delay(20);
  
  return;
}
// --------------------------------------------------------------------------------------------------------------------
