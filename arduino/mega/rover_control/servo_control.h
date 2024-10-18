#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

void servoSetup();  // Function to set up the servos

// Steering modes
void adjustFrontTwoWheelSteering(int frontAngle);        // Adjust front wheels for two-wheel steering
void adjustSixWheelInPhaseSteering(int angle);           // Adjust all wheels in phase steering
void adjustSixWheelReversePhaseSteering(int frontAngle, int rearAngle);  // Adjust front/rear for reverse-phase steering
void adjustZeroRadiusSteering();                         // Adjust for zero-radius turning
void adjustLateralParkingSteering();                     // Adjust for lateral parking (90 degrees for all wheels)

// Crab steering modes
void crabServoLeft();                                 // Adjust all wheels for crab steering to the left
void crabServoRight();                                // Adjust all wheels for crab steering to the right

#endif
