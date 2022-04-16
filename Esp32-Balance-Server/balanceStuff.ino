#include "Adafruit_MPU6050.h"
#include "Adafruit_Sensor.h"


#include <Wire.h>

Adafruit_MPU6050 mpu;

#define I2C_SDA 15
#define I2C_SCL 13

TwoWire I2CMPU = TwoWire(0);
double lastError = 0;
double targetAngle = -7;
double angle = 0;

void initMPU(){
  I2CMPU.begin(I2C_SDA, I2C_SCL, 100000);

  if (!mpu.begin(MPU6050_I2CADDR_DEFAULT, &I2CMPU)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void MPULoop(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp); 

      /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  double K = 1.0;  // Overall torque gain factor
  double Kp = 8.0;
  //double Kp2 = 16.0;
  double Ki = 3.0;
  double Kd = 0.5;
  double pTerm, iTerm, dTerm, output, angleError;
  
  double accelYAngle = -atan2(a.acceleration.x, a.acceleration.y);

  

  angle = 0.98 * (angle + .03 * g.gyro.z) + 0.02 * accelYAngle;
  
  double degAngle = angle*57.29578;
  Serial.print(", angle ");
  Serial.print(degAngle);
  degAngle = ((degAngle+180)>180? degAngle-360: degAngle)+180;

  angleError = (targetAngle)-degAngle; // subtract angle by offset
  pTerm = angleError*Kp;

  dTerm = Kd * (angleError - lastError);
  lastError = angleError;

  output = K*(pTerm + dTerm);
  
  
  Serial.print(", angle error ");
  Serial.println(output);

  if (degAngle > targetAngle-10 && degAngle < targetAngle+10){
    controllMotors(0, output);
  }else{
    controllMotors(0, 0);
  }
    
  
}
