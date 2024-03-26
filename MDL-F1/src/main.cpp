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
  unit.mode.customConfig = 1;

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
        break;

      case 1: // Mode: runContinuously
        break;
      
      case 2: // Mode: turnOffEachCycle
        break;
      
      case -1: // Error 
        break;
      }
  }

  // Update the last scan value
  unit.status.rfSignal_ = unit.status.rfSignal;

  return;
}
// --------------------------------------------------------------------------------------------------------------------
