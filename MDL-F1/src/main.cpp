// include libraries
#include <Arduino.h>
#include <rc-switch/RCSwitch.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// Define DEBUG constants
// --------------------------------------------------------------------------------------------------------------------
#define DEBUG true                                            // DEBUG true allows for Serial Output
#define USE_CONFIG true                                       // USE_CONFIG true uses the values stored on the SD card in the Ethernet Shield, 
                                                              // USE_CONFIG false uses hardcoded values (for use after testing phase is completed)
// --------------------------------------------------------------------------------------------------------------------

// Define constants (PINS)
#define MOTOR_STEP_PIN 1                                      // GPIO pin connected to stepper motor driver (step pin)
#define MOTOR_DIR_PIN 2                                       // GPIO pin connected to stepper motor driver (direction pin)
#define RF_RECEIVER_PIN 0                                     // GPIO pin connected to the RF receiver
#define CONFIG_FILE "configuration.csv"                       // Assumes the CSV file has two values per line separated by a comma

// Variables to store values from the CSV file (must match all vars in config file)
int stepsPerRev;
int revsPerClick;


// Variable initialization procedure (run once)
void InitVars(bool useConfig, bool debug) {
  if(useConfig){
    // Call the function to read the CSV file and assign values
    readCSV(debug);
  }
  else{
    // Define the variables in code
    stepsPerRev = 200;                                      // Number of steps required for 1 full revolution of the stepper motor
    revsPerClick = 3;                                       // Number of times to complete a full revolution
  }
}

// File reading procudure (run once)
void readCSV(bool debug) {
  // Open the CSV file
  File file = SD.open(CONFIG_FILE);
  // Check for error opening the file
  if (!file) {
    if(debug) {
      Serial.println("Failed to open file!");
    }
    return;
  }

  // Read and parse CSV file line by line
  while (file.available()) {
    // Read a line from the file
    String line = file.readStringUntil('\n');
    
    // Split the line by comma to extract values
    int commaIndex = line.indexOf(',');
    if (commaIndex != -1) {
      // Extract values
      String value1_str = line.substring(0, commaIndex);
      String value2_str = line.substring(commaIndex + 1);

      // Convert string values to integers
      stepsPerRev = value1_str.toInt();
      revsPerClick = value2_str.toInt();

      if(debug) {
        // Print values (optional)
        Serial.print("Variable 1: ");
        Serial.println(stepsPerRev);
        Serial.print("Variable 2: ");
        Serial.println(revsPerClick);
      }
    }
  }

  // Close the file
  file.close();
}

// Motion Control procedure (run on each new RF input)
void rotateMotor(int revs, int steps) {
    digitalWrite(MOTOR_DIR_PIN, HIGH);                        // Set direction

    // Rotate X times
    for (int i = 0; i < revs; i++) {
      // Rotate motor X steps
      for (int j = 0; j < steps; j++) {
          digitalWrite(MOTOR_STEP_PIN, HIGH);
          delayMicroseconds(500);                             // Adjust delay for your motor
          digitalWrite(MOTOR_STEP_PIN, LOW);
          delayMicroseconds(500);                             // Adjust delay for your motor
      }
    }
}

// --------------------------------------------------------------------------------------------------------------------
// Initializzation procedure (to be run once)
void setup() {
  // If in debug mode, start serial comms
  if(DEBUG){
    Serial.begin(9600);
  }

  // Set the Variable values based on config file or hard-coded values
  InitVars(USE_CONFIG, DEBUG);

  // Configure the wiring setup/pinMode
  pinMode(MOTOR_STEP_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  
  // Initialize RF receiver
  RCSwitch rfReceiver = RCSwitch();
  rfReceiver.enableReceive(RF_RECEIVER_PIN);

  // Initialize Ethernet Sheild and SD card
  Ethernet.begin(mac);
  // Check for error with the SD card
  if (!SD.begin(4)) {
    if(DEBUG){
      Serial.println("SD card initialization failed!");
    }
    return;
  }

}
// --------------------------------------------------------------------------------------------------------------------
// Main executable
int main() {
    // Run the setup procedure
    setup();

    // Start infinite loop
    while (true) {
      // New RF signal is detected
      if (digitalRead(RF_RECEIVER_PIN) == HIGH) {
        // Rotate motor X times the number of steps provided
        rotateMotor(revsPerClick, stepsPerRev);
        delay(100);                                           // Adjust delay for responsiveness
      }
    }

    return 0;
}
// --------------------------------------------------------------------------------------------------------------------
