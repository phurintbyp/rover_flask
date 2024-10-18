#include "i2c_communication.h"
#include <Wire.h>
#include <math.h>  // For sqrt, pow, fabs, and other math functions
#include "pid_control.h"

// Struct for holding GPS coordinates
struct Coordinates {
    float latitude;
    float longitude;
};

// Array to store received GPS coordinates
Coordinates coords[200];
int maxIndex = 0;
int coordIndex = 0;
bool navigating = false;

String receivedData = "";   // For GPS data or incoming command strings
char lastCommand = 0;       // To store the last received command (e.g., 'w', 'a', etc.)

// Setup function for I2C
void i2cSetup() {
    Wire.begin(0x08);  // Set Arduino as I2C slave at address 0x08
    Wire.onReceive(receiveData);  // Register function to receive I2C data
}

// Function to process received GPS data or commands
void receiveData(int byteCount) {
    receivedData = "";  // Clear previous received data

    while (Wire.available()) {
        char c = Wire.read();  // Read one byte at a time
        if (c >= 32 && c <= 126) {
            receivedData += c;  // Append the valid character to the buffer
        }
    }

    // Handle GPS data if it starts with "v" (e.g., "v lat:... lng:...")
    if (receivedData.startsWith("v")) {
        parseAndStoreCoordinates(receivedData);
        Serial.println("Received GPS data: " + receivedData);  // Debugging output
    }
    // Handle maxIndex data
    else if (receivedData.startsWith("max")) {
        parseMaxIndex(receivedData);
    }
    else if (receivedData.length() == 1) {
        lastCommand = receivedData[0];  // Store the command in lastCommand
        Serial.print("Received Command: ");
        Serial.println(lastCommand);
    }
}

// Function to read the last received command (e.g., 'w', 'a', etc.)
char i2cReadCommand() {
    char command = lastCommand;  // Get the last received command
    lastCommand = 0;  // Clear the command after reading to avoid repeated processing
    return command;   // Return the command for processing
}

// Function to parse and store GPS coordinates
void parseAndStoreCoordinates(String data) {
    if (coordIndex >= 200) {
        Serial.println("Error: Exceeded maximum coordinate storage.");
        return;  // Avoid overflow
    }

    int latIndex = data.indexOf("lat:");
    int lonIndex = data.indexOf("lng:");

    if (latIndex != -1 && lonIndex != -1) {
        // Extract latitude and longitude from the string
        String latString = data.substring(latIndex + 4, data.indexOf(",", latIndex));
        String lonString = data.substring(lonIndex + 4, data.length());

        // Convert strings to floats and store in coords array
        coords[coordIndex].latitude = latString.toFloat();
        coords[coordIndex].longitude = lonString.toFloat();

        // Debugging output
        Serial.println("Parsed Latitude: " + String(coords[coordIndex].latitude));
        Serial.println("Parsed Longitude: " + String(coords[coordIndex].longitude));

        coordIndex++;  // Move to the next index for storing the next set of coordinates
    }
}

// Function to parse maxIndex from data
void parseMaxIndex(String data) {
    int index = data.indexOf("maxIndex");
    if (index != -1) {
        String maxIndexString = data.substring(index + 9);  // Extract maxIndex
        maxIndexString.trim();  // Trim any leading/trailing spaces
        maxIndex = maxIndexString.toInt();
        Serial.println("Parsed max index: " + String(maxIndex));
    }
}

// Function to check if the rover has reached the waypoint
bool reachedWaypoint(float target_lat, float target_lon) {
    float current_lat = 13.7776;  // Placeholder for current latitude from GPS
    float current_lon = 100.3456; // Placeholder for current longitude from GPS

    // Calculate the Euclidean distance between current and target coordinates
    float distance = sqrt(pow(target_lat - current_lat, 2) + pow(target_lon - current_lon, 2));

    // Define a threshold for considering the waypoint reached (e.g., 0.0001 degrees)
    float threshold = 0.0001;

    return (distance < threshold);  // Return true if the distance is below the threshold
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
            Serial.println("Reached waypoint " + String(coordIndex));
        }
    } else {
        navigating = false;  // Stop navigation once all waypoints are reached
        Serial.println("Navigation complete");
    }
}
