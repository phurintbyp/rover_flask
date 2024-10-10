#include <Wire.h>
#include <Servo.h>

#define SLAVE_ADDRESS 0x08  // I2C address of the Arduino

Servo myServo;  // Create servo object
char command;

void setup() {
  Wire.begin(SLAVE_ADDRESS);  // Join I2C bus as a slave with address 0x08
  Wire.onReceive(receiveData);  // Register function to receive I2C data
  myServo.attach(9);  // Attach servo to pin 9
  myServo.write(0);  // Set initial position
  Serial.begin(9600);  // Initialize serial communication for debugging
  Serial1.begin(9600);
}

void loop() {
  // Nothing to do here, servo control happens in receiveData()
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    command = Wire.read();  // Read the incoming I2C data
    if (command == 'i') {
      Serial1.print("1");
      Serial.println("Sent:On");  
    } else if (command == 'd') {
      Serial1.print("2");  
      Serial.println("Sent:Off");   
    }
  }
}
