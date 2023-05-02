#include "Adafruit_MPU6050.h"
#include "Adafruit_Sensor.h"
#include <Wire.h>

Adafruit_MPU6050 mpu;

#define I2C_SDA 15
#define I2C_SCL 13

TwoWire I2CMPU = TwoWire(0);


float pwmMotors = 0;
double outputControlOutput = 0;


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
  static int frameCount = 0;
  frameCount += 1;
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp); 

      /* Print out the values */
//  if (frameCount % 10 == 0)
//  {    
//    Serial.print("Acceleration X: ");
//    Serial.print(a.acceleration.x);
//    Serial.print(", Y: ");
//    Serial.print(a.acceleration.y);
//    Serial.print(", Z: ");
//    Serial.print(a.acceleration.z);
//    Serial.println(" m/s^2");
//  }

  double pTerm, iTerm, dTerm, output, angleError;
  
  double accelYAngle = atan2(a.acceleration.x, a.acceleration.y);

  // calculate the angle so that:    0 = vertical    -90 = on its back    90 = on its face
  double degAccelAngle = accelYAngle*57.29578;
  degAccelAngle = ((degAccelAngle+180)>180? degAccelAngle-360: degAccelAngle)+180;
  double scaledZGyro = (g.gyro.z * 57.29578)/(800/deltaTime);
  
  static double angle = degAccelAngle; 
  angle = (0.98 * (angle + scaledZGyro)) + (0.02 * degAccelAngle);

  

  // get the error (difference between desired and current angle)
  static double adaptiveAngleBias = 0;
  
  

  if(balancing){
    float kpOutput = .03;    // kp = .07
    float kiOutput = .09; 
    angleError = (angle - (targetAngle + adaptiveAngleBias)); // subtract angle by offset
    
    // calculate the proportional component
    pTerm = angleError*Kp;
  
    // calculate the integral component (summation of past errors * i scalar)
    static double integral = 0;
    integral += angleError;
    iTerm = Ki * constrain(integral, -100,100);
  
    // calculate the derivative component
    static double lastError = 0;
    dTerm = Kd * (angleError - lastError);
    lastError = angleError;
  
    output = K*(pTerm + iTerm + dTerm);
    //adaptiveAngleBias += output * 1.5e-4;
    //adaptiveAngleBias = constrain(adaptiveAngleBias, -5,5);
    static int count =0;
    count ++;
    if(count >= 2){
      count = 0;
      // angle adjusting
      static double outputFilter = output;
      static double outputFilterOld, outputIntegral = 0;
      outputFilter = outputFilterOld * 0.7 + output * 0.3;
      outputFilterOld = outputFilter;
      outputIntegral += outputFilter;
      //outputIntegral += -setting_car_speed;
      outputIntegral = constrain(outputIntegral, -3000, 3000);
      
      outputControlOutput = -kpOutput * outputFilter - kiOutput * outputIntegral;
      //rotation_control_output = setting_turn_speed + kd_turn * scaledYGyro;
      
    }

    pwmMotors = output;
  }

  //float outmap = mapf(abs(output),0,1,.11,1);  //map out deadband
  //int outsign = (output>0) - (output< 0); // get sign of number
  //outmap  = outmap * outsign;
  
  // stop the motors if we're far from vertical since there is no chance of success
  if (angle > targetAngle-10 && angle < targetAngle+10){
    balancing = true;
    Motors(0, pwmMotors);
  }else{
    balancing = false;
    Motors(0, 0);
  }
  
  if (frameCount % 10 == 0)
  {
//    Serial.print(", output ");
//    Serial.print(output,5);
//    Serial.print(", angleBias ");
//    Serial.print(adaptiveAngleBias,5);
    Serial.print(" angle ");
    Serial.print(angle);
    Serial.print(", gyro ");
    Serial.print(scaledZGyro,5);
    Serial.println(" ");

  }
}
