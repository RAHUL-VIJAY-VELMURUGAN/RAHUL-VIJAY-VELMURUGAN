#include <Wire.h>

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

  



  while(micros()-LoopTimer<4000);
  LoopTimer=micros();


}







   

