#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

void servoSetup();

// Steering modes
void adjustFrontTwoWheelSteering(int frontAngle);
void adjustSixWheelInPhaseSteering(int angle);
void adjustSixWheelReversePhaseSteering(int frontAngle, int rearAngle);
void adjustZeroRadiusSteering();
void adjustLateralParkingSteering();

#endif
