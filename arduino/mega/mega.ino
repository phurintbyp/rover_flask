#include <Wire.h>

#define SLAVE_ADDRESS 0x08  // I2C address of the Arduino

char command;
String receivedData= "";
struct Coordinates {
  float latitude;
  float longitude;
};

// Array to store coordinates (adjust size as needed)
Coordinates coords[200];
int maxIndex = 0;
int coordIndex = 0;

void setup() {
  Wire.begin(SLAVE_ADDRESS);  // Join I2C bus as a slave with address 0x08
  Wire.onReceive(receiveData);  // Register function to receive I2C data
  pinMode(13, OUTPUT);
  Serial.begin(9600);  // Initialize serial communication for debugging
  Serial1.begin(9600);
}

void loop() {

}

// Read data via I2C
void receiveData(int byteCount) {
  receivedData = "";
  while (Wire.available()) {
    char c = Wire.read();  // Read one byte
    if (c >= 32 && c <= 126) {
      receivedData += c;  // Append the valid character to the buffer
    }
  }

  Serial.println(receivedData);
  if (receivedData.length() != 1){
    if (receivedData.startsWith("max")){
      parseMaxIndex(receivedData);
    } else{
      parseAndStoreCoordinates(receivedData);
    }
  }

  if (receivedData == "w") {
    for (int i = 0; i < maxIndex; i++) {
      Serial.print("Lat: ");
      Serial.print(coords[i].latitude, 6);
      Serial.print(", Lon: ");
      Serial.println(coords[i].longitude, 6);
    }
  }else if (receivedData == "s") {
    clearCoordsArray();
  }
}

// Read data from coordiantes
void parseAndStoreCoordinates(String data) {
  // Split the data by spaces to extract latitude and longitude
  int firstSpace = data.indexOf(' ');
  int secondSpace = data.indexOf(' ', firstSpace + 1);

  if (firstSpace != -1 && secondSpace != -1) {
    String latStr = data.substring(firstSpace + 1, secondSpace);
    String lonStr = data.substring(secondSpace + 1);

    // Convert latitude and longitude strings to float
    float latitude = latStr.toFloat();
    float longitude = lonStr.toFloat();

    // Store in the array if there's space
    if (coordIndex < 200) {  // Ensure we don't exceed array bounds
      coords[coordIndex].latitude = latitude;
      coords[coordIndex].longitude = longitude;
      coordIndex++;  // Increment index for the next coordinate
    } else {
      Serial.println("Coordinate storage full!");
    }
  } else {
    Serial.println("Invalid data format!");
  }
}

// Read max index
void parseMaxIndex(String data) {
    // Extract the value after "max: "
    int maxStart = data.indexOf("max: ");
    if (maxStart != -1) {
        String maxString = data.substring(maxStart + 5);  // Skip "max: "
        maxIndex = maxString.toInt();  // Convert the string to an integer
        Serial.print("Max value set to: ");
        Serial.println(maxIndex);
    } else {
        Serial.println("Error: Invalid max value format");
    }
}

// clear coords array
void clearCoordsArray() {
  for (int i = 0; i < maxIndex; i++) {
    coords[i].latitude = 0.0;
    coords[i].longitude = 0.0;
  }
  coordIndex = 0;  // Reset the index
  Serial.println("Coordinates array cleared.");
}