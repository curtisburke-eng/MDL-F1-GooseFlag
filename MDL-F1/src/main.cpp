// include libraries
#include <Arduino.h>
#include "F1_typ.h"

// Declare global variables/objects
F1_typ unit;

// --------------------------------------------------------------------------------------------------------------------
// Initializzation procedure (to be run once)
void setup() {
  // If in debug mode, start serial comms
  if(unit.cmd.useSerialComms){
    Serial.begin(9600);
  }

  // Initialize this unit
  Init(&unit);

}
// --------------------------------------------------------------------------------------------------------------------
// Main executable
int main() {
    // Run the setup procedure
    setup();

    // Start infinite loop
    while (true) {
      // New RF signal is detected
      if (digitalRead(unit.internal.rfReceiver.value) == HIGH) {
        // Rotate motor X times the number of steps provided
        rotateMotor(&unit);
        delay(100);                                           // Adjust delay for responsiveness
      }
    }

    return 0;
}
// --------------------------------------------------------------------------------------------------------------------
