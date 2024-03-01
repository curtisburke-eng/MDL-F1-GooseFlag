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

    return;
}

void F1_typ::loadDefaultConfig() {

    // Define Motor configurable values
    internal.stepsPerRev = 200;                                         // Number of steps required for 1 full revolution of the stepper motor
    internal.revsPerCycle = 3;                                          // Number of times to complete a full revolution
    internal.rpm = 60;                                                  // The speed of rotation in revolutions per min.

    // Define Pin Layout
    internal.motorDriverPinIN1 = 2;                                     // GPIO pin connected to stepper motor driver IN1 pin
    internal.motorDriverPinIN2 = 3;                                     // GPIO pin connected to stepper motor driver IN2 pin
    internal.motorDriverPinIN3 = 4;                                     // GPIO pin connected to stepper motor driver IN3 pin
    internal.motorDriverPinIN4 = 5;                                     // GPIO pin connected to stepper motor driver IN4 pin
    internal.rfReceiverPin = 8;                                         // GPIO pin connected to RF controller output

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
    // Motor configurable values
    internal.stepsPerRev = json["stepsPerRev"];
    internal.revsPerCycle = json["revsPerCycle"];
    internal.rpm = json["rpm"];

    // Define Pin Layout
    internal.motorDriverPinIN1 = json["motorDriverPinIN1"];
    internal.motorDriverPinIN2 = json["motorDriverPinIN2"];
    internal.motorDriverPinIN3 = json["motorDriverPinIN3"];
    internal.motorDriverPinIN4 = json["motorDriverPinIN4"];
    internal.rfReceiverPin = json["rfReceiverPin"];

    // Print values for confirmation
    if(cmd.useSerialComms){
        Serial.println("----------------------------");
        Serial.println("CUSTOM CONFIGURATION LOADED");
        Serial.println("----------------------------");
        Serial.print("Motor Driver Pin IN1: ");
        Serial.println(internal.motorDriverPinIN1);
        Serial.print("Motor Driver Pin IN2: ");
        Serial.println(internal.motorDriverPinIN2);
        Serial.print("Motor Driver Pin IN3: ");
        Serial.println(internal.motorDriverPinIN3);
        Serial.print("Motor Driver Pin IN4: ");
        Serial.println(internal.motorDriverPinIN4);
        Serial.println();
        Serial.print("RF Receiver Pin: ");
        Serial.println(internal.rfReceiverPin);
        Serial.println();
        Serial.print("Steps per Revolution: ");
        Serial.println(internal.stepsPerRev);
        Serial.print("Revolutions per Cycle: ");
        Serial.println(internal.revsPerCycle);
        Serial.print("Motor Speed (RPM): ");
        Serial.println(internal.rpm);
        Serial.println("----------------------------");
        
    }
}
