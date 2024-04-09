#include "F1_typ.h"


void F1_typ::Init() {
    
    if(mode.customConfig){
        // Assign values based on json file inputs
        loadCustomConfig();
    }
    else{
        // Assign values based on hardcoded inputs
        loadDefaultConfig();
    }
    // Set pin mode for inputs/outputs
    //pinMode(internal.motorDriver.enablePin, OUTPUT);
    pinMode(internal.motorDriver.directionPin, OUTPUT);
    pinMode(internal.motorDriver.stepPin, OUTPUT);
    pinMode(internal.rfReceiverPin, INPUT);


    // Check that only 1 mode is acitve
    checkMode();

    return;
}

void F1_typ::loadDefaultConfig() {

    // Define Motor configurable values
    internal.stepsPerRev = 200;                                         // Number of steps required for 1 full revolution of the stepper motor
    internal.revsPerCycle = 3;                                          // Number of times to complete a full revolution
    internal.rpm = 1000;                                                // The speed of rotation in revolutions per min.
    internal.rotationDirection = 0;                                     // 1 or 0 for clockwise or counter-clockwise rotation

    // Define Pin Layout
    internal.motorDriver.enablePin = 0;                                 // GPIO pin connected to stepper motor driver Enable + terminal
    internal.motorDriver.directionPin = 3;                              // GPIO pin connected to stepper motor driver Direction + terminal
    internal.motorDriver.stepPin = 2;                                   // GPIO pin connected to stepper motor driver Pulse + terminal

    internal.rfReceiverPin = 8;                                         // GPIO pin connected to RF controller output

    // Define Mode(s)
    mode.useSerialComms = 0;
    // ONLY 1 of the following can be true at any given time
    mode.useCycleTimer = 1;
    mode.runContinuous = 0;
    mode.turnOffEachCycle = 0;

    internal.secBetweenCycles = 3;                                     // The number of seconds between cycles (used in CycleTimer Mode)

    // Set configured status
    status.isConfigured = 1;                                            

    return;
}


void F1_typ::loadCustomConfig() {
    // Open file for reading 
    File file = SD.open(internal.configFileName);
    // Allocate the JSON document
    JsonDocument json;

    // Deserialize the JSON document
    DeserializationError deserializeError = deserializeJson(json, file);

    // Check for parsing error
    if (deserializeError) {
        Serial.print(F("Failed to read file. Using default configuration."));
        loadDefaultConfig();
        return;
    }

    // --- Parse values from file ---
    // TODO: There should be some checking for if the value exists, and setting to default if not

    // Motor configurable values
    internal.stepsPerRev = json["stepsPerRev"];
    internal.revsPerCycle = json["revsPerCycle"];
    internal.secBetweenCycles = json["secBetweenCycles"];
    internal.rpm = json["rpm"];
    internal.rotationDirection = json["rotationDirection"];
    
    // Define Pin Layout
    internal.motorDriver.enablePin = json["motorDriver.enablePin"];
    internal.motorDriver.directionPin = json["motorDriver.directionPin"];
    internal.motorDriver.stepPin = json["motorDriver.stepPin"];
    internal.rfReceiverPin = json["rfReceiverPin"];

    // Define Mode
    mode.useSerialComms = json["useSerialComms"];
    mode.useCycleTimer = json["useCycleTimer"];
    mode.runContinuous = json["runContinuous"];
    mode.turnOffEachCycle = json["turnOffEachCycle"];
    
    file.close(); 

    // Set configured status
    status.isConfigured = 1;

    // Print values for confirmation
    if(mode.useSerialComms){
        Serial.println("----------------------------");
        Serial.println("CUSTOM CONFIGURATION LOADED");
        Serial.println("----------------------------");
        // Pin Layout
        Serial.print("Motor Driver Enable Pin: ");
        Serial.println(internal.motorDriver.enablePin);
        Serial.print("Motor Driver Direction Pin: ");
        Serial.println(internal.motorDriver.directionPin);
        Serial.print("Motor Driver Pulse Pin: ");
        Serial.println(internal.motorDriver.stepPin);
        Serial.print("RF Receiver Pin: ");
        Serial.println(internal.rfReceiverPin);
        // Motor Params
        Serial.println();
        Serial.print("Steps per Revolution: ");
        Serial.println(internal.stepsPerRev);
        Serial.print("Revolutions per Cycle: ");
        Serial.println(internal.revsPerCycle);
        Serial.print("Seconds between Cycles: ");
        Serial.println(internal.secBetweenCycles);
        Serial.print("Motor Speed (RPM): ");
        Serial.println(internal.rpm);
        Serial.print("Motor Rotation Direction: ");
        Serial.println(internal.rotationDirection);
        // Modes
        Serial.println();
        Serial.print("Use Serial Comms: ");                     // won't see this if not...
        Serial.println(mode.useSerialComms);
        Serial.print("Use Cycle Timer Mode: ");
        Serial.println(mode.useCycleTimer);
        Serial.print("Use Continuous Mode: ");
        Serial.println(mode.runContinuous);
        Serial.print("Use ON/OFF Mode: ");
        Serial.println(mode.turnOffEachCycle);
        Serial.println("----------------------------");
        
    }
}

void F1_typ::checkMode() {
    if(status.isConfigured){
        // Only 1 mode can be true at a given time
        // Check if all three modes are true or If any combination of two modes are true
        if((mode.runContinuous && mode.turnOffEachCycle && mode.useCycleTimer) ||
            (mode.runContinuous && mode.turnOffEachCycle) ||
            (mode.runContinuous && mode.useCycleTimer) ||
            (mode.turnOffEachCycle && mode.useCycleTimer)   ) {

            // Use default mode
            mode.useCycleTimer = 1;
            mode.runContinuous = 0;
            mode.turnOffEachCycle = 0;
        }
        
        // Set the mode number based on the bool states
        if(mode.useCycleTimer) {
            status.modeNum = 0;
        }
        else if(mode.runContinuous) {
            status.modeNum = 1;
        }
        else if(mode.turnOffEachCycle) {
            status.modeNum = 2;
        }
        else {
            status.modeNum = -1;
            status.error = 1;
        }
        
    }
    else {
        status.error = 1;
    }
}

void F1_typ::run1Rev() {
    if(status.isConfigured) {
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
    else {
        // Throw an error if not configured
        status.error = 1;
    }

}

void F1_typ::runCycle() {
    if(status.isConfigured){
        for(int i = 0; i<internal.revsPerCycle; i++) {
            run1Rev();
        }
    }
}