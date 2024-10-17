#include "motor_control.h"
#include "servo_control.h"
#include "pid_control.h"
#include "i2c_communication.h"

enum SteeringMode {
    FRONT_TWO_WHEEL_STEERING,
    SIX_WHEEL_IN_PHASE,
    SIX_WHEEL_REVERSE_PHASE,
    ZERO_RADIUS_STEERING,
    LATERAL_PARKING
};

SteeringMode currentSteeringMode = FRONT_TWO_WHEEL_STEERING;

void setup() {
    Serial.begin(115200);

    motorSetup();
    servoSetup();
    pidSetup();
    i2cSetup();

    setSteeringMode(SIX_WHEEL_IN_PHASE);

void loop() {

    processCoordinates();

    if (isNavigating()) {
        processNavigation();
    }

    // Example of handling a command
    if (Serial.available()) {
        char command = Serial.read();
        switch (command) {
            case 'w':
                moveForward();
                break;
            case 's':
                moveBackward();
                break;
            case 'a':
                turnLeft();
                break;
            case 'd':
                turnRight();
                break;
            case 'h':
                stopMoving();
                break;
            case '1':
                setSteeringMode(FRONT_TWO_WHEEL_STEERING);
                Serial.println("Switched to Front Two-Wheel Steering");
                break;
            case '2':
                setSteeringMode(SIX_WHEEL_IN_PHASE);
                Serial.println("Switched to Six-Wheel In-Phase Steering");
                break;
            case '3':
                setSteeringMode(SIX_WHEEL_REVERSE_PHASE);
                Serial.println("Switched to Six-Wheel Reverse-Phase Steering");
                break;
            case '4':
                setSteeringMode(ZERO_RADIUS_STEERING);
                Serial.println("Switched to Zero-Radius Steering");
                break;
            case '5':
                setSteeringMode(LATERAL_PARKING);
                Serial.println("Switched to Lateral Parking Mode");
                break;
        }
    }
}
