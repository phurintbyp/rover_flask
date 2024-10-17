#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

enum SteeringMode {
    FRONT_TWO_WHEEL_STEERING,
    SIX_WHEEL_IN_PHASE,
    SIX_WHEEL_REVERSE_PHASE,
    ZERO_RADIUS_STEERING,
    LATERAL_PARKING
};

// Function prototypes for motor control
void motorSetup();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMoving();
void setSteeringMode(SteeringMode mode);

#endif