#include <Arduino.h>
#include <wiringPi.h>

#define MOTOR_STEP_PIN 1                                       // GPIO pin connected to stepper motor driver (step pin)
#define MOTOR_DIR_PIN 2                                       // GPIO pin connected to stepper motor driver (direction pin)
#define STEPS_PER_REVOLUTION 200                              // Number of steps required for 1 full revolution of the stepper motor

void setup() {
    wiringPiSetup();
    pinMode(MOTOR_STEP_PIN, OUTPUT);
    pinMode(MOTOR_DIR_PIN, OUTPUT);

}

void rotateMotorFullRev(int numRotations) {
    digitalWrite(MOTOR_DIR_PIN, HIGH);                        // Set direction

    // Rotate 1 full rotation X times
    for (int i = 0; i < numRotations; i++) {
      // Rotate motor 1 full revolution
      for (int j = 0; j < STEPS_PER_REVOLUTION; j++) {
          digitalWrite(MOTOR_STEP_PIN, HIGH);
          delayMicroseconds(500);                               // Adjust delay for your motor
          digitalWrite(MOTOR_STEP_PIN, LOW);
          delayMicroseconds(500);                               // Adjust delay for your motor
      }
    }
}

int main() {
    // Run the setup procedure
    setup();

    // Start infinite loop
    while (true) {
        // Rotate motor X full rotations
        rotateMotorFullRev(3);
        delay(100);                                           // Adjust delay for responsiveness
    }

    return 0;
}
