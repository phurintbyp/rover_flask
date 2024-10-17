#include "i2c_communication.h"
#include <Wire.h>
#include "pid_control.h"

// Struct for holding coordinates
struct Coordinates {
    float latitude;
    float longitude;
};

// Array to store received GPS coordinates
Coordinates coords[200];
int maxIndex = 0;
int coordIndex = 0;
bool navigating = false;

// Variables for diameter and coordinate frequency
int desired_diameter = 6;      // Default diameter in meters
int coord_frequency = 1000;    // Default frequency for GPS updates in ms
String receivedData = "";

// I2C setup
void i2cSetup() {
    Wire.begin(8);  // Set Arduino as I2C slave at address 0x08
    Wire.onReceive(receiveData);  // Register function to receive I2C data
}

// Function to process received GPS data or commands
void receiveData(int byteCount) {
    receivedData = "";
    while (Wire.available()) {
        char c = Wire.read();  // Read one byte
        if (c >= 32 && c <= 126) {
            receivedData += c;  // Append the valid character to the buffer
        }
    }

    // Process diameter and frequency values from I2C
    if (receivedData.length() != 1) {
        if (receivedData.startsWith("max")) {
            parseMaxIndex(receivedData);
        } else if (receivedData.startsWith("lat")) {
            parseAndStoreCoordinates(receivedData);
            Serial.println(receivedData);
        } else if (receivedData.startsWith("diam")) {
            // Process diameter and frequency
            parseDiameterAndFrequency(receivedData);
        }
    }

    // Command handling ('w' for waypoints, 's' to clear coordinates)
    if (receivedData == "w") {
        navigating = true;  // Start navigating through waypoints
    } else if (receivedData == "s") {
        clearCoordsArray();  // Clear coordinates
    }
}

// Function to check if the rover is navigating
bool isNavigating() {
    return navigating;
}

// Function to process navigation through waypoints
void processNavigation() {
    if (coordIndex < maxIndex) {
        float target_lat = coords[coordIndex].latitude;
        float target_lon = coords[coordIndex].longitude;

        // Update the target coordinates for PID control
        updateTargetCoordinates(target_lat, target_lon);

        // Check if the waypoint is reached and move to the next one
        if (reachedWaypoint(target_lat, target_lon)) {
            coordIndex++;
        }
    } else {
        navigating = false;  // Stop navigation once all waypoints are reached
    }
}

// Placeholder for checking if the rover reached the waypoint
bool reachedWaypoint(float target_lat, float target_lon) {
    // Add your actual GPS distance-check logic here
    return false;
}

// Parse and store diameter and frequency from I2C
void parseDiameterAndFrequency(String data) {
    int diamIndex = data.indexOf("diam: ");
    int freqIndex = data.indexOf("freq: ");

    if (diamIndex != -1 && freqIndex != -1) {
        // Extract and convert diameter and frequency
        String diameterString = data.substring(diamIndex + 6, freqIndex).trim();
        String frequencyString = data.substring(freqIndex + 6).trim();

        desired_diameter = diameterString.toInt();
        coord_frequency = frequencyString.toInt();

        Serial.print("Received desired diameter: ");
        Serial.println(desired_diameter);
        Serial.print("Received coordinate frequency: ");
        Serial.println(coord_frequency);
    } else {
        Serial.println("Error: Invalid diameter/frequency format");
    }
}
