#include "readCsv.h"


// File reading procudure (run once)
void readCsv(F1_typ* unit) {
    // Initialize Ethernet Sheild and SD card
    Ethernet.begin(mac);
    
    // Check for error with the SD card
    if (!SD.begin(4)) {
        if(unit->cmd.useSerialComms){
            Serial.println("SD card initialization failed!");
        }
        return;
    }


    // Open the CSV file
    File file = SD.open(unit->internal.configFileName);
    // Check for error opening the file
    if (!file) {
        if(unit->cmd.useSerialComms) {
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
        unit->internal.stepsPerRev = value1_str.toInt();
        unit->internal.revsPerCycle = value2_str.toInt();

        if(unit->cmd.useSerialComms) {
            // Print values
            Serial.println("Values read from file");
            Serial.print("StepsPerRev: ");
            Serial.println(unit->internal.stepsPerRev);
            Serial.print("RevsPerCycle: ");
            Serial.println(unit->internal.revsPerCycle);
        }
        }
    }

    // Close the file
    file.close();
}
