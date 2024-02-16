#include <Arduino.h>
#include <wiringPi.h>

#define STEPPER_PIN_1 1                                       // GPIO pin connected to stepper motor driver (step pin)
#define STEPPER_PIN_2 2                                       // GPIO pin connected to stepper motor driver (direction pin)
#define STEPS_PER_REVOLUTION 200                              // Number of steps required for 1 full revolution of the stepper motor

void setup() {
    wiringPiSetup();
    pinMode(STEPPER_PIN_1, OUTPUT);
    pinMode(STEPPER_PIN_2, OUTPUT);

}

void rotateMotorFullRev() {
    // Rotate stepper motor for 1 full revolution
    digitalWrite(STEPPER_PIN_2, HIGH);                        // Set direction
    for (int i = 0; i < STEPS_PER_REVOLUTION; i++) {
        digitalWrite(STEPPER_PIN_1, HIGH);
        delayMicroseconds(500);                               // Adjust delay for your motor
        digitalWrite(STEPPER_PIN_1, LOW);
        delayMicroseconds(500);                               // Adjust delay for your motor
    }
}

int main() {
    // Run the setup procedure
    setup();

    // Start infinite loop
    while (true) {
        // Rotate motor X full rotations
        rotateMotorFullRev();
        delay(100);                                           // Adjust delay for responsiveness
    }

    return 0;
}
