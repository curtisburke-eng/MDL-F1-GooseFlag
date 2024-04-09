#include "F1_typ.h"


void F1_typ::Init() {
    
    if(cmd.useCustomConfig){
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
    cmd.useSerialComms = 0;

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
    cmd.useSerialComms = json["useSerialComms"];
    
    file.close(); 

    // Set configured status
    status.isConfigured = 1;

    // Print values for confirmation
    if(cmd.useSerialComms){
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
        Serial.println(cmd.useSerialComms);
        Serial.println("----------------------------");
        
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