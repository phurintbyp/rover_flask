#include "motor_control.h"
#include "servo_control.h"
#include <L298N.h>
#include <Arduino.h>
#include <math.h>

#define PI 3.14159265358979323846
#define DEG_TO_RAD 0.01745329252

// Define motor drivers for front, middle, and rear wheels (left and right)
L298N motorFrontLeft(8, 9, 10);   // Front Left: ENA = 8, IN1 = 9, IN2 = 10
L298N motorFrontRight(13, 11, 12);  // Front Right: ENB = 13, IN3 = 11, IN4 = 12
L298N motorMiddleLeft(22, 23, 24); // Middle Left: ENA = 22, IN1 = 23, IN2 = 24
L298N motorMiddleRight(27, 25, 26);  // Middle Right: ENB = 27, IN3 = 25, IN4 = 26
L298N motorRearLeft(28, 29, 30);     // Rear Left: ENA = 28, IN1 = 29, IN2 = 30
L298N motorRearRight(33, 31, 32);    // Rear Right: ENB = 33, IN3 = 31, IN4 = 32

int baseSpeed = 150;  // Base speed for all wheels

SteeringMode currentSteeringMode = FRONT_TWO_WHEEL_STEERING;  // Default mode

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

// Crab steering left
void crabSteeringLeft() {
    // All wheels move in the same direction laterally
    motorFrontLeft.forward();
    motorFrontRight.forward();
    motorMiddleLeft.forward();
    motorMiddleRight.forward();
    motorRearLeft.forward();
    motorRearRight.forward();
    
    Serial.println("Crab Steering Left");
}

// Crab steering right
void crabSteeringRight() {
    // All wheels move in the same direction laterally
    motorFrontLeft.backward();
    motorFrontRight.backward();
    motorMiddleLeft.backward();
    motorMiddleRight.backward();
    motorRearLeft.backward();
    motorRearRight.backward();
    
    Serial.println("Crab Steering Right");
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
        case CRAB_STEERING_LEFT:
            crabSteeringLeft();  // Crab steering left
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
        case CRAB_STEERING_RIGHT:
            crabSteeringRight();  // Crab steering right
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
    motorFrontLeft.setSpeed(0);
    motorFrontRight.setSpeed(0);
    motorMiddleLeft.setSpeed(0);
    motorMiddleRight.setSpeed(0);
    motorRearLeft.setSpeed(0);
    motorRearRight.setSpeed(0);
}

// Set steering mode to determine how wheels should move
void setSteeringMode(SteeringMode mode) {
    currentSteeringMode = mode;
}

// void calibrate() {

// }
