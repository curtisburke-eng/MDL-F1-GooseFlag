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

    return;
}

void F1_typ::loadDefaultConfig() {

    // Define Motor configurable values
    internal.stepsPerRev = 200;                                         // Number of steps required for 1 full revolution of the stepper motor
    internal.revsPerCycle = 3;                                          // Number of times to complete a full revolution
    internal.rpm = 60;                                                  // The speed of rotation in revolutions per min.
    internal.rotationDirection = 1;                                     // 1 or -1 for clockwise or counter-clockwise rotation

    // Define Pin Layout
    internal.motorDriverPinIN1 = 2;                                     // GPIO pin connected to stepper motor driver IN1 pin
    internal.motorDriverPinIN2 = 3;                                     // GPIO pin connected to stepper motor driver IN2 pin
    internal.motorDriverPinIN3 = 4;                                     // GPIO pin connected to stepper motor driver IN3 pin
    internal.motorDriverPinIN4 = 5;                                     // GPIO pin connected to stepper motor driver IN4 pin
    internal.rfReceiverPin = 8;                                         // GPIO pin connected to RF controller output
    // Define Mode(s)
    mode.useSerialComms = 0;
    // ONLY 1 of the following can be true at any given time
    mode.useCycleTimer = 1;
    mode.runContinuous = 0;
    mode.turnOffEachCycle = 0;

    internal.secBetweenCycles = 10;                                     // The number of seconds between cycles (used in CycleTimer Mode)

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
    internal.motorDriverPinIN1 = json["motorDriverPinIN1"];
    internal.motorDriverPinIN2 = json["motorDriverPinIN2"];
    internal.motorDriverPinIN3 = json["motorDriverPinIN3"];
    internal.motorDriverPinIN4 = json["motorDriverPinIN4"];
    internal.rfReceiverPin = json["rfReceiverPin"];
    
    // Define Mode
    mode.useSerialComms = json["useSerialComms"];
    mode.useCycleTimer = json["useCycleTimer"];
    mode.runContinuous = json["runContinuous"];
    mode.turnOffEachCycle = json["turnOffEachCycle"];

    // Print values for confirmation
    if(mode.useSerialComms){
        Serial.println("----------------------------");
        Serial.println("CUSTOM CONFIGURATION LOADED");
        Serial.println("----------------------------");
        // Pin Layout
        Serial.print("Motor Driver Pin IN1: ");
        Serial.println(internal.motorDriverPinIN1);
        Serial.print("Motor Driver Pin IN2: ");
        Serial.println(internal.motorDriverPinIN2);
        Serial.print("Motor Driver Pin IN3: ");
        Serial.println(internal.motorDriverPinIN3);
        Serial.print("Motor Driver Pin IN4: ");
        Serial.println(internal.motorDriverPinIN4);
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

void F1_typ::moveStepper1Rev() {
    // Determine the step increment based on direction (only allow 1 or -1)
    int stepIncrement = (internal.rotationDirection == 1) ? 1 : -1;
    // Calculate delay based on desired RPM
    int delayBetweenSteps = 60000 / (internal.stepsPerRev * internal.rpm);          // 60000 is the number of milliseconds in a minute
    
    // Loop through steps
    for (int j = 0; j < abs(internal.stepsPerRev); j++) {
        // Determine the current step in the sequence
        int stepIndex = (j % 4);

        // Set the motor pins according to the step sequence
        digitalWrite(internal.motorDriverPinIN1, internal.stepSequence[stepIndex][0]);
        digitalWrite(internal.motorDriverPinIN2, internal.stepSequence[stepIndex][1]);
        digitalWrite(internal.motorDriverPinIN3, internal.stepSequence[stepIndex][2]);
        digitalWrite(internal.motorDriverPinIN4, internal.stepSequence[stepIndex][3]);

        // Delay to control motor speed based on rpm input
        delay(delayBetweenSteps); 
    }

}
