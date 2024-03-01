#ifndef F1_TYP_H
#define F1_TYP_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>

class F1_typ 
{
public:
    // Member Structures
    F1_command_typ cmd;
    F1_status_typ status;
    F1_internal_typ internal;
    
    // Class Functions
    void Init();
    void moveStepper1Rev();

private:
    void loadDefaultConfig();
    void loadCustomConfig();
};

struct F1_command_typ
{
    // Commands
    bool useSerialComms;                                        // Command for determining if Serial output is to be used
    bool useCustomConfig;                                       // Command for determining the source of initial values for all configurable variables
};

struct F1_status_typ
{
    // Statuses
    bool error;
};

struct F1_internal_typ
{   
    // Configuration variables
    String configFileName = "config.json";
    
    // Pin Type variables
    int motorDriverPinIN1;                                      // GPIO pin connected to stepper motor driver IN1 pin
    int motorDriverPinIN2;                                      // GPIO pin connected to stepper motor driver IN2 pin
    int motorDriverPinIN3;                                      // GPIO pin connected to stepper motor driver IN3 pin
    int motorDriverPinIN4;                                      // GPIO pin connected to stepper motor driver IN4 pin
    int rfReceiverPin;                                          // GPIO pin connected to RF controller output

    // Motor Configuration variables
    int stepsPerRev;                                            // The number of steps for a given motor to complete one full revolution
    int revsPerCycle;                                           // The number of revolutions desired for one cycle (1 cycle being started by an RF input)
    int rpm;                                                    // The speed of rotation in revolutions per min.
    int rotationDirection;                                      // 1 or -1 for clockwise or counter-clockwise rotation

    const int stepSequence[4][4] = {
        {HIGH, LOW, LOW, HIGH},
        {HIGH, HIGH, LOW, LOW},
        {LOW, HIGH, HIGH, LOW},
        {LOW, LOW, HIGH, HIGH}
    };
};



#endif // F1_TYP_H
