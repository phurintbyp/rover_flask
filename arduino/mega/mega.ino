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
  Serial.println(receivedData);

  if (receivedData == 'w') {
    digitalWrite(13, HIGH);
  }else if (receivedData == 'a') {
    digitalWrite(13, LOW);
  }
}

bool isCoordinateString(const String& str) {
  return str.startsWith("lat: ") && str.indexOf("long:") != -1;
}