#include "motorStuff.h"
#include <Arduino.h>
#include <driver/ledc.h>

const int MotPin0 = 14;  
const int MotPin1 = 2;  
const int MotPin2 = 12;  
//const int MotPin3 = 2;  

static int oldl = 0, oldr = 0, oldm = 0;
int l;
int r;
int m;

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
  m = abs(adjustY) ;



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
//    l = 0;
//    r = 0;
   
  }else{
    
    ledcWrite(4,l);
    ledcWrite(6,0);
    ledcWrite(5,r);
//    m = 0;
    
   
  }

}

int lowPassFilter(int current,int & old){
  float rate = .9;
  old = float(old)*rate + float(current)*(1-rate);
  return old;
}

void motorLoop(){
  static float oldTime = 0;
  static bool oldMoveBackwards = false;
  float DeltaTime = 0;
  float rate = .5;
  oldl = float(oldl)*rate + float(l)*(1-rate);
  oldr = float(oldr)*rate + float(r)*(1-rate);
  oldm = float(oldm)*rate + float(m)*(1-rate);

  ledcWrite(4,oldl * (oldl>oldm));
  ledcWrite(6,oldm * (oldm>oldl && oldm>oldr));
  ledcWrite(5,oldr * (oldr>oldm));
  
}
