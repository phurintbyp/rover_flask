#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
#include <L298N.h>

enum SteeringMode {
    FRONT_TWO_WHEEL_STEERING,
    SIX_WHEEL_IN_PHASE,
    SIX_WHEEL_REVERSE_PHASE,
    ZERO_RADIUS_STEERING,
    LATERAL_PARKING,
    CRAB_STEERING_LEFT,      // Added for crab steering
    CRAB_STEERING_RIGHT      // Added for crab steering
};

extern L298N motorFrontLeft;
extern L298N motorFrontRight;
extern L298N motorMiddleLeft;
extern L298N motorMiddleRight;
extern L298N motorRearLeft;
extern L298N motorRearRight;

// Function prototypes for motor control
void motorSetup();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMoving();
void setSteeringMode(SteeringMode mode);
void adjustMotorSpeedsForCurvedPath(double correction, int diameter);

// Added functions for crab steering
void crabSteeringLeft();     // Prototype for crab steering to the left
void crabSteeringRight();    // Prototype for crab steering to the right

// Function prototypes for specific steering adjustments
void adjustFrontTwoWheelSteering(int angle);
void adjustSixWheelInPhaseSteering(int angle);
void adjustSixWheelReversePhaseSteering(int frontAngle, int rearAngle);
void adjustZeroRadiusSteering();
void adjustLateralParkingSteering();

#endif
