#include "Init.h"


// Functions
void Init(F1_typ* unit) {
    
    if(unit->cmd.useConfig){
        // Assign values based on csv file inputs
        readCsv(*unit);
    }
    else{
        // Define Motor configurable values
        unit->internal.stepsPerRev = 200;                                       // Number of steps required for 1 full revolution of the stepper motor
        unit->internal.revsPerClick = 3;                                        // Number of times to complete a full revolution
        
        // Define Pin Layout
        unit->internal.motorEnable.pin = 1;                                     // GPIO pin connected to stepper motor driver (step pin)
        unit->internal.motorDirection.pin = 2;                                  // GPIO pin connected to stepper motor driver (direction pin)
        unit->internal.rfReceiver.pin = 0;                                      // GPIO pin connected to the RF receiver

        // Define pin initial values
        unit->internal.motorDirection.value = HIGH;                             // HIGH or LOW based on desired rotation direction
        unit->internal.motorEnable.value = LOW;                                 // HIGH or LOW for applying motion/power to the motor 
    }
    
    // Configure the wiring setup/pinMode
    pinMode(unit->internal.motorEnable.pin, OUTPUT);
    pinMode(unit->internal.motorDirection.pin, OUTPUT);
    pinMode(unit->internal.rfReceiver.pin, INPUT);

}