#include "Adafruit_MPU6050.h"
#include "Adafruit_Sensor.h"


#include <Wire.h>

Adafruit_MPU6050 mpu;

#define I2C_SDA 15
#define I2C_SCL 16

TwoWire I2CMPU = TwoWire(0);

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

  
}
