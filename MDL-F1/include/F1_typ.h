#ifndef F1_TYP_H
#define F1_TYP_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SD.h>


struct F1_mode_typ
{
    // Commands for desired Modes
    bool useSerialComms;                                        // Command for determining if Serial output is to be used
    bool customConfig;                                          // Command for determining the source of initial values for all configurable variables
    bool useCycleTimer;                                         // Mode where RF "ON" signal starts a cycle and a timer, cycles repeat at interval set by timer, Off stops next cycle from completing
    bool runContinuous;                                         // Mode where RF "ON" signal starts motion and "OFF" signal stops motion (at zero point)
    bool turnOffEachCycle;                                      // Mode where RF "ON" signal completes a single cycle, another cycle will not complete until the "OFF" signal is seen AND a new "ON" signal is recieved 
};

struct F1_status_typ
{
    // Statuses
    bool isConfigured;                                          // The state of the configuration
    int modeNum;                                                // Number to indicate which mode is active
    bool error;                                                 // The error state 
    bool rfSignal;                                              // The current state of the digital read on the rfReceiverPin
    bool rfSignal_;                                             // The last scan state of the digital read on the rfReceiverPin
};

struct TB6600StepperDriver_typ
{
    // Pin Type variables
    int enablePin;                                              // GPIO pin connected to stepper motor driver Enable + terminal
    int directionPin;                                           // GPIO pin connected to stepper motor driver Direction + terminal
    int stepPin;                                                // GPIO pin connected to stepper motor driver Pulse + terminal
};


struct F1_internal_typ
{   
    // Configuration variables
    String configFileName = "config.json";
    
    // Pin Type variables
    TB6600StepperDriver_typ motorDriver;                        // Structure containing the GPIO pins connected to stepper motor driver
    int rfReceiverPin;                                          // GPIO pin connected to RF controller output

    // Motor Configuration variables
    int stepsPerRev;                                            // The number of steps for a given motor to complete one full revolution
    int revsPerCycle;                                           // The number of revolutions desired for one cycle (1 cycle being started by an RF input)
    int secBetweenCycles;                                       // The number of seconds between cycles (used in CycleTimer Mode)
    int rpm;                                                    // The speed of rotation in revolutions per min.
    bool rotationDirection;                                     // 1 or 0 for clockwise or counter-clockwise rotation 

};

class F1_typ 
{
public:
    // Member Structures
    F1_mode_typ mode;
    F1_status_typ status; 
    F1_internal_typ internal;
    
    // Class Functions
    void Init();
    void run1Rev();
    void runCycle();

private:
    void loadDefaultConfig();
    void loadCustomConfig();
    void checkMode();
};

#endif // F1_TYP_H
