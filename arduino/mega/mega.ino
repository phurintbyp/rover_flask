#include <Wire.h>
#include <Servo.h>

#define SLAVE_ADDRESS 0x08  // I2C address of the Arduino

Servo myServo;  // Create servo object
char command;
String readString;

void setup() {
  Wire.begin(SLAVE_ADDRESS);  // Join I2C bus as a slave with address 0x08
  Wire.onReceive(receiveData);  // Register function to receive I2C data
  Wire.onRequest(sendData);      // Register function to send I2C data
  myServo.attach(9);  // Attach servo to pin 9
  myServo.write(0);  // Set initial position
  Serial.begin(9600);  // Initialize serial communication for debugging
  Serial1.begin(9600);
}

void loop() {
  readString = "";

  while (Serial1.available()) {
    delay(2);
    if (Serial1.available() > 0) {
      char c = Serial1.read();
      if (isControl(c)) {
        break;
      }
      readString += c;
    }
  }
  if (readString != ""){
    Serial.println(readString);
  }
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    command = Wire.read();  // Read the incoming I2C data
    if (command == 'i') {
      Serial1.print("1");
      Serial.println("Sent: On");
    } else if (command == 'd') {
      Serial1.print("2");
      Serial.println("Sent: Off");
    }
  }
}

void sendData() {
  Wire.write(readString.c_str());
}
