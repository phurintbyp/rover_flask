const int echoPin = 2;
const int trigPin = 3;

float duration, distance;
float previous_error = 0;
float integral = 0;

// PID constants
double kp = 1.0;
double ki = 0.1;
double kd = 0.05;

// Desired distance (setpoint)
float setpoint = 100.0;  // Example: 50 cm

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(13, OUTPUT); 
  Serial.begin(9600);
}

void loop() {
  String message = "";
  String readString;
  String Q;

  // Read ultrasonic sensor distance
  distance = readDistance();

  // PID control
  float error = setpoint - distance;
  integral += error;
  float derivative = error - previous_error;
  float output = kp * error + ki * integral + kd * derivative;

  previous_error = error;

  // Print the distance and PID output to the serial monitor
  message = "Distance: " + String(distance) + " cm, PID output: " + String(output);
  Serial.println(message);

  // Handle Serial input to control the LED
  while (Serial.available()) {
    delay(1); // Small delay to ensure the data is available
    if (Serial.available() > 0) {
      char c = Serial.read();  // Get one byte from the serial buffer
      if (isControl(c)) {
        break;
      }
      readString += c; // Append character to the string
    }
  }
  delay(100); // Delay to prevent flooding the serial output
}

// Function to read distance from ultrasonic sensor
float readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Convert to cm
  return distance;
}
