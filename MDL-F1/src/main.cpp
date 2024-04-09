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
  unit.cmd.useCustomConfig = 0;

  // If in debug mode, start serial comms
  if(unit.cmd.useSerialComms){
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

    unit.runCycle();
    delay(unit.internal.secBetweenCycles*1000);

  }

  // Update the last scan value
  unit.status.rfSignal_ = unit.status.rfSignal;
  delay(20);
  
  return;
}
// --------------------------------------------------------------------------------------------------------------------
