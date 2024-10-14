#include <Wire.h>

// Motor Control Pins (L298N)
const int ENA = 10; // PWM for Motor A speed
const int IN1 = 4;  // Motor A direction pin 1
const int IN2 = 5;  // Motor A direction pin 2

// MPU6050 I2C address
#define MPU6050_ADDR 0x68

// PID Control Variables
float targetAngle = 0;  // Target angle (0 degrees for straight movement)
float error = 0, integral = 0, derivative = 0, last_error = 0;
float Kp = 11.0, Ki = 0.09, Kd = 10.0;  // PID gains
float pitch = 0;  // Current pitch angle
float pidOutput = 0;  // PID output

int baseSpeed = 127;  // Base motor speed

// MPU6050 Raw Data Variables
int16_t acc_x, acc_y, acc_z;
int16_t gyro_x, gyro_y, gyro_z;
long gyro_x_cal = 0, gyro_y_cal = 0, gyro_z_cal = 0;
float angle_pitch = 0, angle_pitch_acc = 0;
bool set_gyro_angles = false;
long loop_timer;
int error_count = 0;  // Track repeated I2C failures

void setup() {
  Wire.begin();  // Start I2C communication
  Serial.begin(9600);  // Start Serial Monitor

  // Initialize motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  setup_mpu_6050_registers();  // Configure MPU6050
  calibrate_mpu_6050();  // Perform initial calibration

  loop_timer = micros();  // Initialize loop timer
}

void loop() {
  if (!read_mpu_6050_data()) {
    Serial.println("Error: MPU6050 read failed.");
    return;  // Skip this loop iteration if read fails
  }

  // Subtract gyro offsets
  gyro_x -= gyro_x_cal;
  gyro_y -= gyro_y_cal;
  gyro_z -= gyro_z_cal;

  // Calculate pitch angle using gyro data (integration)
  angle_pitch += gyro_x * 0.0000611;

  // Calculate pitch angle using accelerometer data
  float acc_total_vector = sqrt((acc_x * acc_x) + (acc_y * acc_y) + (acc_z * acc_z)) + 0.0001;
  angle_pitch_acc = asin((float)acc_y / acc_total_vector) * 57.296;

  if (isnan(angle_pitch_acc)) {
    angle_pitch_acc = 0;  // Avoid NaN propagation
  }

  // Apply complementary filter
  if (set_gyro_angles) {
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;
  } else {
    angle_pitch = angle_pitch_acc;
    set_gyro_angles = true;
  }

  // Check if pitch is out of range and recalibrate if needed
  if (angle_pitch > 45 || angle_pitch < -45) {
    Serial.println("Recalibrating MPU6050...");
    calibrate_mpu_6050();  // Recalibrate MPU6050
  }

  // PID Control Logic
  error = targetAngle - angle_pitch;
  integral += error;
  derivative = error - last_error;
  pidOutput = (Kp * error) + (Ki * integral) + (Kd * derivative);
  last_error = error;

  // Adjust motor speeds based on PID output
  if (pidOutput > 0) {
    motorForward(constrain(baseSpeed + pidOutput, 0, 255));
  } else {
    motorBackward(constrain(baseSpeed - abs(pidOutput), 0, 255));
  }

  // Debug output
  Serial.print("Pitch: ");
  Serial.print(angle_pitch);
  Serial.print(" | PID Output: ");
  Serial.println(pidOutput);

  // Ensure loop runs at 250Hz
  while (micros() - loop_timer < 4000);
  loop_timer = micros();
}

// Function to control motor forward
void motorForward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
}

// Function to control motor backward
void motorBackward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
}

// MPU6050 Register Setup
void setup_mpu_6050_registers() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);  // Power management register
  Wire.write(0x00);  // Wake up MPU6050
  if (Wire.endTransmission() != 0) {
    Serial.println("Error: MPU6050 not connected.");
    while (1);  // Halt if MPU6050 is not detected
  }

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1C);  // Accelerometer config register
  Wire.write(0x10);  // Set accelerometer range to +/-8g
  Wire.endTransmission();

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1B);  // Gyro config register
  Wire.write(0x08);  // Set gyro range to +/-500dps
  Wire.endTransmission();
}

// Recalibrate MPU6050 by averaging gyro values
void calibrate_mpu_6050() {
  gyro_x_cal = 0;
  gyro_y_cal = 0;
  gyro_z_cal = 0;

  for (int i = 0; i < 1000; i++) {
    if (read_mpu_6050_data()) {
      gyro_x_cal += gyro_x;
      gyro_y_cal += gyro_y;
      gyro_z_cal += gyro_z;
    }
    delay(3);
  }

  gyro_x_cal /= 1000;
  gyro_y_cal /= 1000;
  gyro_z_cal /= 1000;

  Serial.println("MPU6050 recalibrated.");
}

// Read MPU6050 Data
bool read_mpu_6050_data() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);  // Start with accelerometer data register
  if (Wire.endTransmission() != 0) {
    error_count++;
    if (error_count > 5) {
      Serial.println("Error: Repeated I2C failures.");
      while (1);  // Halt if too many failures occur
    }
    return false;  // Communication failed
  }

  Wire.requestFrom(MPU6050_ADDR, 14);  // Request 14 bytes
  if (Wire.available() < 14) {
    return false;  // Not enough data received
  }

  // Read accelerometer and gyro data
  acc_x = (int16_t)(Wire.read() << 8 | Wire.read());
  acc_y = (int16_t)(Wire.read() << 8 | Wire.read());
  acc_z = (int16_t)(Wire.read() << 8 | Wire.read());
  int temp_raw = (int16_t)(Wire.read() << 8 | Wire.read());  // Temperature (not used)
  gyro_x = (int16_t)(Wire.read() << 8 | Wire.read());
  gyro_y = (int16_t)(Wire.read() << 8 | Wire.read());
  gyro_z = (int16_t)(Wire.read() << 8 | Wire.read());

  return true;  // Data read successfully
}
