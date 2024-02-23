#ifndef F1_TYP_H
#define F1_TYP_H

#include <string>

class F1_typ 
{
public:
    // Structs
    F1_command_typ cmd;
    F1_status_typ status;
    F1_internal_typ internal;
};

struct F1_command_typ
{
    // Commands
    bool useSerialComms;                                        // Command for determining if Serial output is to be used
    bool useConfig;                                             // Command for determining the source of initialvalues for all configurable variables
};

struct F1_status_typ
{
    // Statuses
    bool error;
};

struct F1_internal_typ
{   
    // Configuration variables
    std::string configFileName = "configuration.csv";
    
    // Pin Type variables
    F1_pin_typ motorEnable;                                     // GPIO pin connected to stepper motor driver (step/enable/power pin)
    F1_pin_typ motorDirection;                                  // GPIO pin connected to stepper motor driver (direction pin)
    F1_pin_typ rfReceiver;                                      // GPIO pin connected to the RF receiver or DI from RF receiver 

    // Motor Configuration variables
    int stepsPerRev;                                            // The number of steps for a given motor to complete one full revolution
    int revsPerCycle;                                           // The number of revolutions desired for one cycle (1 cycle being started by an RF input)
};

struct F1_pin_typ
{
    int value;
    int pin;
};



#endif // F1_TYP_H
