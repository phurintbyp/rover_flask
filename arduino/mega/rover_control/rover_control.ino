#include "motor_control.h"
#include "servo_control.h"
#include "i2c_communication.h"  // Use this for I2C communications

void setup() {
    Serial.begin(9600);  // Start serial communication for debugging
    motorSetup();
    servoSetup();
    i2cSetup();  // Initialize I2C communication as a slave

    setSteeringMode(SIX_WHEEL_IN_PHASE);  // Set the initial steering mode
}

void loop() {
    char command = i2cReadCommand();  // Read command via the centralized I2C handler

    // Process command if a valid one is received
    if (command != 0) {
        switch (command) {
              case 'w':
                  Serial.println("Moving Forward");
                  moveForward();
                  break;
              case 's':
                  Serial.println("Moving Backward");
                  moveBackward();
                  break;
              case 'a':
                  crabServoLeft();
                  setSteeringMode(CRAB_STEERING_LEFT);
                  Serial.println("Turning Left");
                  break;
              case 'd':                            
                  crabServoRight();
                  setSteeringMode(CRAB_STEERING_RIGHT);                                            
                  Serial.println("Turning Right");
                  break;
              case 'h':
                  Serial.println("Stopping Movement");
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
              default:
                  Serial.println("Unknown command received via I2C");
                  break;
        }
    }
}
