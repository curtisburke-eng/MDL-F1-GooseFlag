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
  unit.cmd.useSerialComms = 1;
  unit.cmd.useCustomConfig = 1;

  // If in debug mode, start serial comms
  if(unit.cmd.useSerialComms){
    Serial.begin(9600); 
  }

  // Initialize this unit's members
  unit.Init();
  

}
// --------------------------------------------------------------------------------------------------------------------
// Main executable
void loop() {
  
  // Check for new RF signal                              // TODO: Store the last RF signal and compare to it. Then if different from previous 
  if (digitalRead(unit.internal.rfReceiverPin) == HIGH) {
    // Rotate motor i times the number of steps provided
    for (int i = 0; i < unit.internal.revsPerCycle; i++) {
      unit.moveStepper1Rev();
    }
    delay(100); // Adjust delay for responsiveness        // TODO: Could make the delay between cycles configurable
  }


  // TODO: change command and status to mode type
  // TODO: add useTimer and useOnOff modes
  // TODO: add a check such that both modes cannot be true

  return;
}
// --------------------------------------------------------------------------------------------------------------------
