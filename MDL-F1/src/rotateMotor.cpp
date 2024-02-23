#include "rotateMotor.h"

// Motion Control procedure (run on each new RF input)
void rotateMotor(F1_typ* unit) {
    digitalWrite(unit.internal.pin.motorDir, unit.internal.motorDir);                        // Set direction

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