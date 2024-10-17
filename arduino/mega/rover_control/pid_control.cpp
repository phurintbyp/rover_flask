#include <Wire.h>
#include "pid_control.h"
#include <MPU9250.h>
#include <PID_v1.h>
#include "motor_control.h"

// MPU9250 setup for heading control
MPU9250 mpu;

// PID control variables
double Setpoint, Input, Output;
double Kp = 11, Ki = 0.09, Kd = 10;  // PID coefficients for heading
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// GPS-based PID control variables (for coordinates)
double current_lat = 13.7776;  // Placeholder current latitude (to be updated from GPS)
double current_lon = 100.3456; // Placeholder current longitude (to be updated from GPS)
double target_lat, target_lon; // Target GPS coordinates

// Target and current yaw for straight-line movement
double targetYaw = 0;    // Target yaw (will be set by GPS direction)
double currentYaw = 0;   // Current yaw from MPU9250

// Variables to be received via I2C
int desired_diameter = 6;      // Default diameter in meters for circular paths
int coord_frequency = 1000;    // Default frequency for GPS coordinate updates (in ms)

// Function prototypes
void receiveI2CData(int numBytes);
void updatePID();
void adjustMotorSpeedsForCurvedPath(double correction, int diameter);
double calculateHeading(double lat1, double lon1, double lat2, double lon2);

// Setup function for PID and I2C
void pidSetup() {
    // Initialize MPU9250 (connected via I2C)
    if (!mpu.setup(0x68)) {
        Serial.println("MPU9250 connection failed!");
        while (1);
    }

    // Set the PID controller to automatic mode
    myPID.SetMode(AUTOMATIC);

    // Initial Setpoint (will be updated based on GPS waypoint)
    Setpoint = targetYaw;

    // Initialize I2C for receiving diameter and coordinate frequency
    Wire.begin(8);  // I2C slave address 8
    Wire.onReceive(receiveI2CData);  // Register function to receive data from master

    // Debugging output
    Serial.begin(9600);
    Serial.println("PID Setup Complete. Waiting for I2C data...");
}

// I2C function to receive desired diameter and frequency
void receiveI2CData(int numBytes) {
    while (Wire.available()) {
        // First byte received: desired diameter
        desired_diameter = Wire.read();
        
        // Second byte received: coordinate update frequency
        coord_frequency = Wire.read();

        Serial.print("Received desired diameter: ");
        Serial.println(desired_diameter);
        Serial.print("Received coordinate frequency: ");
        Serial.println(coord_frequency);
    }
}

// Update PID based on IMU and GPS data
void updatePID() {
    // Update MPU9250 data for IMU-based heading control
    mpu.update();
    
    // Get the current yaw (heading) from MPU9250
    currentYaw = mpu.getYaw();

    // Calculate the desired heading (Setpoint) to the next GPS waypoint
    targetYaw = calculateHeading(current_lat, current_lon, target_lat, target_lon);
    Setpoint = targetYaw;

    // Feed the current yaw to the PID controller
    Input = currentYaw;

    // Compute the PID correction for course adjustment
    myPID.Compute();

    // Adjust motor speeds based on PID output and desired diameter
    adjustMotorSpeedsForCurvedPath(Output, desired_diameter);
}

// Adjust motor speeds for curved path (U-turn or circular path) based on PID correction and diameter
void adjustMotorSpeedsForCurvedPath(double correction, int diameter) {
    int baseSpeed = 150;  // Base motor speed
    double radius = diameter / 2.0;  // Calculate radius from diameter

    // Adjust motor speeds based on the correction and desired turning radius
    int leftSpeed = baseSpeed - correction * (1 / radius);   // Decrease left motor speed
    int rightSpeed = baseSpeed + correction * (1 / radius);  // Increase right motor speed

    // Constrain motor speeds to valid range (0-255)
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    // Set motor speeds and move motors forward
    motorFrontLeft.setSpeed(leftSpeed);
    motorFrontRight.setSpeed(rightSpeed);
    motorMiddleLeft.setSpeed(leftSpeed);
    motorMiddleRight.setSpeed(rightSpeed);
    motorRearLeft.setSpeed(leftSpeed);
    motorRearRight.setSpeed(rightSpeed);

    motorFrontLeft.run(FORWARD);
    motorFrontRight.run(FORWARD);
    motorMiddleLeft.run(FORWARD);
    motorMiddleRight.run(FORWARD);
    motorRearLeft.run(FORWARD);
    motorRearRight.run(FORWARD);
}

// Update GPS target coordinates for navigation
void updateTargetCoordinates(double lat, double lon) {
    target_lat = lat;
    target_lon = lon;
}

// Calculate heading between current and target GPS coordinates
double calculateHeading(double lat1, double lon1, double lat2, double lon2) {
    double dLon = (lon2 - lon1);
    double y = sin(dLon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
    double heading = atan2(y, x);
    heading = heading * 180 / M_PI;
    if (heading < 0) {
        heading += 360;
    }
    return heading;
}

void loop() {
    // Continuously update the PID based on GPS and IMU data
    updatePID();
    
    // Delay between updates based on received coordinate frequency (in ms)
    delay(coord_frequency);
}
