#include <Wire.h>

float RateRoll, RatePitch, RateYaw;

float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;

float LoopTimer;

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
  Wire.write(0x10); // setting sensitivity to +-8g
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
}

void loop(){
  gyro_signals();
  Serial.print("AccX [g]:  ");
  Serial.print(AccX);
  Serial.print(" | AccY [g]:  ");
  Serial.print(AccY);
  Serial.print(" | AccZ [g]:  ");
  Serial.println(AccZ);
  // delay(50);

  Serial.print("Roll angle: ");
  Serial.print(AngleRoll);
  Serial.print(" | Pitch angle: ");
  Serial.println(AnglePitch);
  delay(50);

}


