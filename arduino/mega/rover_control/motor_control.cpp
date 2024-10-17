#include "motor_control.h"
#include "servo_control.h"
#include <L298N.h>

// Define motor drivers for front, middle, and rear wheels (left and right)
// L298N Motor Driver Pin Assignment to Arduino:

L298N motorFrontLeft(2, 3, 4);   // Front Left: ENA = 2, IN1 = 3, IN2 = 4
L298N motorFrontRight(5, 6, 7);  // Front Right: ENB = 5, IN3 = 6, IN4 = 7
L298N motorMiddleLeft(8, 9, 10); // Middle Left: ENA = 8, IN1 = 9, IN2 = 10
L298N motorMiddleRight(11, 12, 13);  // Middle Right: ENB = 11, IN3 = 12, IN4 = 13
L298N motorRearLeft(14, 15, 16);     // Rear Left: ENA = 14, IN1 = 15, IN2 = 16
L298N motorRearRight(17, 18, 19);    // Rear Right: ENB = 17, IN3 = 18, IN4 = 19


int baseSpeed = 150;  // Base speed for all wheels
SteeringMode currentSteeringMode;

// Setup motors and servos for the rover
void motorSetup() {
    motorFrontLeft.setSpeed(baseSpeed);
    motorFrontRight.setSpeed(baseSpeed);
    motorMiddleLeft.setSpeed(baseSpeed);
    motorMiddleRight.setSpeed(baseSpeed);
    motorRearLeft.setSpeed(baseSpeed);
    motorRearRight.setSpeed(baseSpeed);

    servoSetup();  // Initialize servos for steering control
}

// Move forward using all 6 wheels
void moveForward() {
    motorFrontLeft.forward();
    motorFrontRight.forward();
    motorMiddleLeft.forward();
    motorMiddleRight.forward();
    motorRearLeft.forward();
    motorRearRight.forward();
}

// Move backward using all 6 wheels
void moveBackward() {
    motorFrontLeft.backward();
    motorFrontRight.backward();
    motorMiddleLeft.backward();
    motorMiddleRight.backward();
    motorRearLeft.backward();
    motorRearRight.backward();
}

// Turn left, adjusting speeds and steering depending on the steering mode
void turnLeft() {
    switch (currentSteeringMode) {
        case FRONT_TWO_WHEEL_STEERING:
            adjustFrontTwoWheelSteering(45);  // Only front wheels turn left
            break;
        case SIX_WHEEL_IN_PHASE:
            adjustSixWheelInPhaseSteering(45);  // All wheels turn left
            break;
        case SIX_WHEEL_REVERSE_PHASE:
            adjustSixWheelReversePhaseSteering(45, -45);  // Front left, rear right
            break;
        case ZERO_RADIUS_STEERING:
            adjustZeroRadiusSteering();  // Max turn for zero radius
            break;
        case LATERAL_PARKING:
            adjustLateralParkingSteering();  // All wheels turned 90 degrees for lateral movement
            break;
    }

    // Adjust wheel speeds for turning
    motorFrontLeft.setSpeed(baseSpeed / 2);  // Slow down left motor
    motorFrontRight.setSpeed(baseSpeed);     // Right motor moves at base speed
    motorMiddleLeft.setSpeed(baseSpeed / 2);
    motorMiddleRight.setSpeed(baseSpeed);
    motorRearLeft.setSpeed(baseSpeed / 2);
    motorRearRight.setSpeed(baseSpeed);
}

// Turn right, adjusting speeds and steering depending on the steering mode
void turnRight() {
    switch (currentSteeringMode) {
        case FRONT_TWO_WHEEL_STEERING:
            adjustFrontTwoWheelSteering(135);  // Only front wheels turn right
            break;
        case SIX_WHEEL_IN_PHASE:
            adjustSixWheelInPhaseSteering(135);  // All wheels turn right
            break;
        case SIX_WHEEL_REVERSE_PHASE:
            adjustSixWheelReversePhaseSteering(-45, 45);  // Front right, rear left
            break;
        case ZERO_RADIUS_STEERING:
            adjustZeroRadiusSteering();  // Max turn for zero radius
            break;
        case LATERAL_PARKING:
            adjustLateralParkingSteering();  // All wheels turned 90 degrees for lateral movement
            break;
    }

    // Adjust wheel speeds for turning
    motorFrontLeft.setSpeed(baseSpeed);      // Left motor moves at base speed
    motorFrontRight.setSpeed(baseSpeed / 2); // Slow down right motor
    motorMiddleLeft.setSpeed(baseSpeed);
    motorMiddleRight.setSpeed(baseSpeed / 2);
    motorRearLeft.setSpeed(baseSpeed);
    motorRearRight.setSpeed(baseSpeed / 2);
}

// Stop all 6 wheels
void stopMoving() {
    motorFrontLeft.brake();
    motorFrontRight.brake();
    motorMiddleLeft.brake();
    motorMiddleRight.brake();
    motorRearLeft.brake();
    motorRearRight.brake();
}

// Set steering mode to determine how wheels should move
void setSteeringMode(SteeringMode mode) {
    currentSteeringMode = mode;
}
