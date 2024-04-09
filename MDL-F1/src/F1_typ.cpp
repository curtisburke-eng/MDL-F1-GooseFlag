#include "F1_typ.h"


void F1_typ::Init() {
    
    // Assign values based on hardcoded inputs
    loadDefaultConfig();

    if(status.isConfigured) {
        // Set pin mode for inputs/outputs
        pinMode(internal.motorDriver.directionPin, OUTPUT);
        pinMode(internal.motorDriver.stepPin, OUTPUT);
        pinMode(internal.rfReceiverPin, INPUT);
    }
    else {
        // Throw an error if not configured
        status.error = 1;
    }
    return;
}

void F1_typ::loadDefaultConfig() {

    // Define Motor configurable values
    internal.stepsPerRev = 200;                                         // Number of steps required for 1 full revolution of the stepper motor
    internal.rpm = 1000;                                                // The speed of rotation in revolutions per min.
    internal.rotationDirection = 0;                                     // 1 or 0 for clockwise or counter-clockwise rotation

    // Define Pin Layout
    internal.motorDriver.directionPin = 3;                              // GPIO pin connected to stepper motor driver Direction + terminal
    internal.motorDriver.stepPin = 2;                                   // GPIO pin connected to stepper motor driver Pulse + terminal
    internal.rfReceiverPin = 8;                                         // GPIO pin connected to RF controller output

    // Define Mode(s)
    cmd.useSerialComms = 0;
    
    // Define Cycle Mode variables 
    internal.revsPerCycle = 3;                                          // Number of times to complete 1 full revolution
    internal.secBetweenCycles = 3;                                      // The number of seconds between cycles (used in CycleTimer Mode)

    // Set configured status
    status.isConfigured = 1;                                            

    return;
}

void F1_typ::run1Rev() {
    
    // Calculate delay based on desired RPM
    int delayBetweenSteps = (int)floor(60000000 / (internal.stepsPerRev * internal.rpm));
    
    // Set rotation direction
    if(internal.rotationDirection) { // rotationDirection is 1
        digitalWrite(internal.motorDriver.directionPin, HIGH);
    } 
    else {
        digitalWrite(internal.motorDriver.directionPin, LOW);
    }

    // Move the motor (Loop through steps)
    for (int j = 0; j < abs(internal.stepsPerRev); j++) {

        // Delay to control motor speed based on rpm input
        digitalWrite(internal.motorDriver.stepPin, HIGH);
        delayMicroseconds(delayBetweenSteps);
        digitalWrite(internal.motorDriver.stepPin, LOW);
        delayMicroseconds(delayBetweenSteps);
    }

}

void F1_typ::runCycle() {
    for(int i = 0; i<internal.revsPerCycle; i++) {
        run1Rev();
    }
}