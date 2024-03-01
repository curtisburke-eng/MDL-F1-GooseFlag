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
  
  // Create the stepper object from this unit's members
  Stepper motor(unit.internal.stepsPerRev,unit.internal.motorDriverPinIN1,unit.internal.motorDriverPinIN2,unit.internal.motorDriverPinIN3,unit.internal.motorDriverPinIN4);
  motor.setSpeed(unit.internal.rpm);

  // Check for new RF signal 
  if (digitalRead(unit.internal.rfReceiverPin) == HIGH) {
    // Rotate motor X times the number of steps provided
    motor.step(unit.internal.stepsPerRev);
    delay(100);                                           // Adjust delay for responsiveness
  }


  return;
}
// --------------------------------------------------------------------------------------------------------------------
