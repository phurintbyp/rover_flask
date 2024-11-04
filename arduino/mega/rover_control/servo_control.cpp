#include "servo_control.h"
#include <Servo.h>
#include <math.h>
#include <Arduino.h>

// Define servos for each of the 6 wheels
Servo servo1, servo2, servo3, servo4, servo5, servo6;

void servoSetup() {
    // Attach servos to pins
    servo1.attach(2);  // Front Left
    servo2.attach(3);  // Front Right
    servo3.attach(4);  // Middle Left
    servo4.attach(5);  // Middle Right
    servo5.attach(6);  // Rear Left
    servo6.attach(7);  // Rear Right
}

// Adjust steering angles for front two wheels only (for Front Two-Wheel Steering mode)
void adjustFrontTwoWheelSteering(int frontAngle) {
    servo1.write(frontAngle);  // Front Left
    servo2.write(frontAngle);  // Front Right

    // Keep other wheels straight
    servo3.write(90);  // Middle Left (Straight)
    servo4.write(90);  // Middle Right (Straight)
    servo5.write(90);  // Rear Left (Straight)
    servo6.write(90);  // Rear Right (Straight)
}

// Adjust steering angles for all six wheels in phase (used in Six-Wheel In-Phase mode)
void adjustSixWheelInPhaseSteering(int angle) {
    servo1.write(angle);  // Front Left
    servo2.write(angle);  // Front Right
    servo3.write(angle);  // Middle Left
    servo4.write(angle);  // Middle Right
    servo5.write(angle);  // Rear Left
    servo6.write(angle);  // Rear Right
}

// Adjust front and rear steering angles for reverse-phase steering (used in Six-Wheel Reverse-Phase mode)
void adjustSixWheelReversePhaseSteering(int frontAngle, int rearAngle) {
    servo1.write(frontAngle);  // Front Left
    servo2.write(frontAngle);  // Front Right
    
    // Middle wheels stay straight in reverse phase mode
    servo3.write(90);  // Middle Left (Straight)
    servo4.write(90);  // Middle Right (Straight)

    // Rear wheels turn opposite to front wheels
    servo5.write(rearAngle);  // Rear Left
    servo6.write(rearAngle);  // Rear Right
}

// Adjust front and rear steering angles for zero-radius turning
void adjustZeroRadiusSteering() {
    servo1.write(90);   // Front Left (Max left turn)
    servo2.write(90);   // Front Right (Max left turn)
    servo5.write(90);   // Rear Left (Max right turn)
    servo6.write(90);   // Rear Right (Max right turn)

    // Middle wheels remain straight for zero-radius turning
    servo3.write(90);   // Middle Left (Straight)
    servo4.write(90);   // Middle Right (Straight)
}

// Adjust all wheels for lateral parking (side movement)
void adjustLateralParkingSteering() {
    servo1.write(90);  // All wheels turn 90 degrees for side movement
    servo2.write(90);
    servo3.write(90);
    servo4.write(90);
    servo5.write(90);
    servo6.write(90);
}

// Crab steering left (all wheels move left)
void crabServoLeft() {
    servo1.write(180);  // Front Left
    servo2.write(180);  // Front Right
    servo3.write(180);  // Middle Left
    servo4.write(180);  // Middle Right
    servo5.write(180);  // Rear Left
    servo6.write(180);  // Rear Right

    // This aligns all wheels to steer the rover left without changing orientation.
    Serial.println("Crab Steering Left");
}

// Crab steering right (all wheels move right)
void crabServoRight() {
    servo1.write(0);  // Front Left
    servo2.write(0);  // Front Right
    servo3.write(0);  // Middle Left
    servo4.write(0);  // Middle Right
    servo5.write(0);  // Rear Left
    servo6.write(0);  // Rear Right

    // This aligns all wheels to steer the rover right without changing orientation.
    Serial.println("Crab Steering Right");
}
