#include <Wire.h>

#define SLAVE_ADDRESS 0x08  // I2C address of the Arduino

char command;
String receivedData= "";

void setup() {
  Wire.begin(SLAVE_ADDRESS);  // Join I2C bus as a slave with address 0x08
  Wire.onReceive(receiveData);  // Register function to receive I2C data
  pinMode(13, OUTPUT);
  Serial.begin(9600);  // Initialize serial communication for debugging
  Serial1.begin(9600);
}

void loop() {

}

void receiveData(int byteCount) {
  receivedData = "";
  while (Wire.available()) {
    char c = Wire.read();  // Read one byte
    if (c >= 32 && c <= 126) {
      receivedData += c;  // Append the valid character to the buffer
    }
  }

  Serial.println("Received Data");
  Serial.println(receivedData);

  // if (receivedData.startsWith("lat")) {
  //   Serial.println("goToCoords called");
  //   // Simulate going to coords function
  //   delay(500);
  //   Serial.println("Reached target");

  //   // Send confirmation to Raspberry Pi
  //   Wire.beginTransmission(SLAVE_ADDRESS);
  //   Wire.write('1');
  //   Wire.endTransmission();
  // }
  // else if (receivedData == "w") {
  //   digitalWrite(13, HIGH);
  // }else if (receivedData == "s") {
  //   digitalWrite(13, LOW);
  // }
}

void goToCoord(String coord){
  float latitude = 0.0f;
  float longitude = 0.0f;

  parseCoords(coord, latitude, longitude);

  Serial.print("Latitude: ");
  Serial.println(latitude);
  Serial.print("Longitude: ");
  Serial.println(longitude);

  // Simulate going to coords function
  delay(500);
  Serial.println("Reached target");

  // Send confirmation to Raspberry Pi
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write('1');
  Wire.endTransmission();
}

void parseCoords(String data, float &latitude, float &longitude) {
  // Find the positions of "lat:" and "lng:"
  int latStart = data.indexOf("lat: ");
  int lngStart = data.indexOf("lng: ");
  
  // If both "lat:" and "lng:" are found
  if (latStart != -1 && lngStart != -1) {
    // Extract the latitude substring
    String latString = data.substring(latStart + 5, lngStart - 2);  // Skip "lat: " and extract until just before "lng"
    String lngString = data.substring(lngStart + 5);  // Skip "lng: " and extract the rest of the string

    // Convert the extracted strings to floats
    latitude = latString.toFloat();
    longitude = lngString.toFloat();

  } else {
    Serial.println("Error: Invalid data format");
  }
}
