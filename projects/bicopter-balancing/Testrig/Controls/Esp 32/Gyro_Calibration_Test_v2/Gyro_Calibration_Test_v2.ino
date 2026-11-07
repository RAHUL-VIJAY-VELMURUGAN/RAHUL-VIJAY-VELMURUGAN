#include <Wire.h>

float RateRoll, RatePitch, RateYaw;

float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber; //number of values i am going to take average of


void gyro_signals(void){
  //Starting I2C with gyro
  Wire.beginTransmission(0x68);

  // Switch on the low pass filter
  Wire.write(0x1A); // 0x68 is address of the register 
  Wire.write(0x05); // 05 corresponds to 10Hz
  Wire.endTransmission();

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

}

void loop(){
  gyro_signals();

  RateRoll -= RateCalibrationRoll;
  RatePitch -= RateCalibrationPitch;
  RateYaw -= RateCalibrationYaw;

  Serial.print("Roll rate [deg/s]: ");
  Serial.print(RateRoll);
  Serial.print(" | Roll pitch [deg/s]: ");
  Serial.print(RatePitch);
  Serial.print(" | Roll yaw [deg/s]: ");
  Serial.println(RateYaw);
  delay(50);
}










