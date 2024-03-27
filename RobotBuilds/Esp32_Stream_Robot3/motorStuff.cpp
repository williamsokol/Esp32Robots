#include "motorStuff.h"

const int MotPin0 = 14;  
const int MotPin1 = 2;  
const int MotPin2 = 12;  
//const int MotPin3 = 2;  

void initMotors() 
{
  pinMode(MotPin0, OUTPUT);
  pinMode(MotPin1, OUTPUT);
  pinMode(MotPin2, OUTPUT);
  //pinMode(MotPin3, OUTPUT);
  ledcSetup(6, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(4, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(5, 2000, 8); // 2000 hz PWM, 8-bit resolution
  //ledcSetup(6, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcAttachPin(MotPin0, 6); //Both(backwards pin)
  ledcAttachPin(MotPin1, 4); //left
  ledcAttachPin(MotPin2, 5); //right
  //ledcAttachPin(MotPin3, 6); //right
}

void controllMotors(float x, float y)
{
  // y is power in the forward/backwards
  // x is how much of the power should go in each wheel
  int l;
  int r;
  int m;
  float normX;
  float normY;
  float adjustX;
  float adjustY;
  float sum;
  

  sum = x+y;
  normX = x/sum;
  normY = y/sum;
  adjustX = x*255;
  adjustY = y*255;

  l = (adjustY + constrain(adjustX,-255,0 ));
  r = (adjustY - constrain(adjustX,0,255 ));
  m = abs(adjustY);
// Serial.print("adjusted left and right Values: ");
//  Serial.print(x);
//  Serial.print(" ");
//  Serial.print(y);
//  Serial.print(" ");
//  Serial.print(m);
//  Serial.print(" ");
//  Serial.print(lr);
//  Serial.print(" ");
//  Serial.print(l);
//  Serial.print(" ");
//  Serial.println(r);
 
  
  if(y < 0){
    ledcWrite(4,0);
    ledcWrite(6,m);
    ledcWrite(5,0 );
    //ledcWrite(6,0);
  }else{
    ledcWrite(4,l);
    ledcWrite(6,0);
    ledcWrite(5,r);
    
    //ledcWrite(6,r);
  }

}
