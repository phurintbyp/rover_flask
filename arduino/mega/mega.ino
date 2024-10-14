#include <Wire.h>
#include <Servo.h>

#define SLAVE_ADDRESS 0x08  // I2C address of the Arduino

Servo myServo;  // Create servo object
char command;
String readString;

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
  while (Wire.available()) {
    command = Wire.read();  // Read the incoming I2C data
    if (command == 'i') {
      digitalWrite(13, HIGH);
    } else if (command == 'd') {
      digitalWrite(13, LOW);
    }
  }
}