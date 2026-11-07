#include <Wire.h>
#include <Arduino.h>
#include <ESP32Servo.h>

// Define pins
const int escPinLeft = 18;    
const int escPinRight = 19;   

Servo escLeft;
Servo escRight;

// PID Parameters
float Kp = 2.0;
float Ki = 0.5;
float Kd = 1.0;

float targetAngle = 0.0;      // Desired horizontal angle
float currentAngle = 0.0;
float error = 0.0;
float previousError = 0.0;
float integral = 0.0;
float derivative = 0.0;
unsigned long lastTime = 0;
float pidOutput = 0.0;

// ESC control parameters
const int escMin = 1000;   
const int escMax = 2000;   
const int escNeutral = 1100; 

float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;
float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;

uint32_t LoopTimer;

// Define the predicted angles and uncertainities,
//kalman angles are 0 initially since copter usually takes of from ground
// kalman uncertainities are 2deg off since it is impossible to be perfectly zero level
float KalmanAngleRoll = 0, KalmanUncertainityAngleRoll = 2*2;
float KalmanAnglePitch = 0, KalmanUncertainityAnglePitch = 2*2;

//Initialize the output filter
// first value - angle prediction, second value - uncertainity of the prediction
float Kalman1DOutput[] = {0,0}; 

// Function calcutaling predicted angle and uncertainity using Kalman equations
void kalman_1d(float KalmanState, float KalmanUncertainity, float KalmanInput,
  float KalmanMeasurement){
    KalmanState = KalmanState + 0.004*KalmanInput;
    KalmanUncertainity = KalmanUncertainity + 0.004*0.004*4*4;
    float KalmanGain = KalmanUncertainity/(KalmanUncertainity + 3*3);

    KalmanState = KalmanState + KalmanGain*(KalmanMeasurement-KalmanState);
    KalmanUncertainity = (1-KalmanGain)*KalmanUncertainity;

    Kalman1DOutput[0] = KalmanState;
    Kalman1DOutput[1] = KalmanUncertainity;
  }

void gyro_signals(void){
  //Starting I2C with gyro
  Wire.beginTransmission(0x68);

  // Switch on the low pass filter
  Wire.write(0x1A); // 0x68 is address of the register 
  Wire.write(0x05); // 05 corresponds to 10Hz
  Wire.endTransmission();

  //configure accelerometer output
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();

  //Reading accelerometer sensor values
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t AccXLSB = Wire.read()<<8|Wire.read();
  int16_t AccYLSB = Wire.read()<<8|Wire.read();
  int16_t AccZLSB = Wire.read()<<8|Wire.read();

  Wire.beginTransmission(0x68); 
  Wire.write(0x1B); //Register address for senstivity address
  Wire.write(0x8); // Adjusting sensitivity to 65.5 LSB per degree per second
  Wire.endTransmission();

  Wire.beginTransmission(0x68); 
  Wire.write(0x43);
  Wire.endTransmission();

  Wire.requestFrom(0x68,6);
  int16_t GyroX = Wire.read()<<8 | Wire.read();
  int16_t GyroY = Wire.read()<<8 | Wire.read();
  int16_t GyroZ = Wire.read()<<8 | Wire.read();

  RateRoll = (float)GyroX/65.5;
  RatePitch = (float)GyroY/65.5;
  RateYaw = (float)GyroZ/65.5;

  //correct the acc value by adding or subtracting the excess value
  AccX = (float)AccXLSB/4096;
  AccY = (float)AccYLSB/4096;
  AccZ = (float)AccZLSB/4096;

  AngleRoll = atan(AccY/sqrt(AccX*AccX + AccZ*AccZ))*(180/3.142);
  AnglePitch = -atan(AccX/sqrt(AccY*AccY + AccZ*AccZ))*(180/3.142);

}

void setup(){
  Serial.begin(115200);
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH); 

  Wire.setClock(400000); //clock speed of MPU6050
  Wire.begin();
  delay(250); //delay time for MPU6050 to start

  //Start Gyro Power mode
  Wire.beginTransmission(0x68); 
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  RateCalibrationRoll = 0;
  RateCalibrationPitch = 0;
  RateCalibrationYaw = 0;


  for (RateCalibrationNumber =0;RateCalibrationNumber<2000;RateCalibrationNumber++){
    gyro_signals();
    RateCalibrationRoll += RateRoll;
    RateCalibrationPitch += RatePitch;
    RateCalibrationYaw += RateYaw;
    delay(4);
  }

  RateCalibrationRoll /= 2000;
  RateCalibrationPitch /= 2000;
  RateCalibrationYaw /= 2000;
  
  escLeft.attach(escPinLeft, escMin, escMax);
  escRight.attach(escPinRight, escMin, escMax);

  // Initialize ESCs at neutral
  escLeft.writeMicroseconds(escNeutral);
  escRight.writeMicroseconds(escNeutral);

  // Step 1: Send max throttle
  escLeft.writeMicroseconds(2000);  // Max throttle (adjust if needed)
  escRight.writeMicroseconds(2000);
  Serial.println("Sending MAX throttle (2000us) for calibration...");
  delay(5000);  // Wait 5 seconds for ESC to enter calibration mode

  // Step 2: Send min throttle
  escLeft.writeMicroseconds(1000);  // Min throttle
  escRight.writeMicroseconds(1000);
  Serial.println("Sending MIN throttle (1000us) to set min throttle...");
  delay(5000);  // Wait 5 seconds for ESC to record min throttle

  Serial.println("Calibration sequence complete.");

  // Step 3: Test low-speed spin
  escLeft.writeMicroseconds(1150);  // Low speed (adjust based on your ESC)
  escRight.writeMicroseconds(1150);
  Serial.println("Running motor at low speed...");

  lastTime = millis();

  LoopTimer = micros();

}

void loop(){
  gyro_signals();

  RateRoll -= RateCalibrationRoll;
  RatePitch -= RateCalibrationPitch;
  RateYaw -= RateCalibrationYaw;

  kalman_1d(KalmanAngleRoll, KalmanUncertainityAngleRoll, RateRoll, AngleRoll);
  KalmanAngleRoll = Kalman1DOutput[0];
  KalmanUncertainityAngleRoll = Kalman1DOutput[1];

  kalman_1d(KalmanAnglePitch, KalmanUncertainityAnglePitch, RatePitch, AnglePitch);
  KalmanAnglePitch = Kalman1DOutput[0];
  KalmanUncertainityAnglePitch = Kalman1DOutput[1];

  Serial.print("Roll Angle [deg]: ");
  Serial.print(KalmanAngleRoll);
  Serial.print(" | Pitch Angle [deg]: ");
  Serial.println(KalmanAnglePitch);

  //PID 
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;  // Convert to seconds
  lastTime = now;

  currentAngle = KalmanAngleRoll;   // User-supplied function to get the current angle

  error = targetAngle - currentAngle;
  integral += error * dt;
  derivative = (error - previousError) / dt;
  previousError = error;

  pidOutput = (Kp * error) + (Ki * integral) + (Kd * derivative);

  // Limit PID output to avoid over-driving motors
  pidOutput = constrain(pidOutput, -100, 100);

  int escSignalLeft = escNeutral + pidOutput;
  int escSignalRight = escNeutral - pidOutput;

  escSignalLeft = constrain(escSignalLeft, escMin, escMax);
  escSignalRight = constrain(escSignalRight, escMin, escMax);

  escLeft.writeMicroseconds(escSignalLeft);
  escRight.writeMicroseconds(escSignalRight);

  Serial.print("Angle: "); Serial.print(currentAngle);
  Serial.print("\tPID Output: "); Serial.print(pidOutput);
  Serial.print("\tESC Left: "); Serial.print(escSignalLeft);
  Serial.print("\tESC Right: "); Serial.println(escSignalRight);


  //250Hz loop
  while(micros()-LoopTimer<4000);
  LoopTimer=micros();


}
