const int MotPin0 = 12;  
const int MotPin1 = 13;  
const int MotPin2 = 14;  
const int MotPin3 = 15;  

void initMotors() 
{
  ledcSetup(3, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(4, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(5, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(6, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcAttachPin(MotPin0, 3); //left 
  ledcAttachPin(MotPin1, 4); //left
  ledcAttachPin(MotPin2, 5); //right
  ledcAttachPin(MotPin3, 6); //right
}

void controllMotors(float x, float y)
{
  // y is power in the forward/backwards
  // x is how much of the power should go in each wheel
  
  

  int l,r;

  
  l = (y+x)*255*2;
  r = (y-x)*255*2;

  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(l);
  Serial.print(" ");
  Serial.println(r);

  constrain(l, 0,255);
  constrain(r, 0,255);


  if (l < 0){
      ledcWrite(3,l);
      ledcWrite(4,0);
  }else{
      ledcWrite(3,0);
      ledcWrite(4,abs(l));
  }
  if (r < 0){
      ledcWrite(5,r);
      ledcWrite(6,0);
  }else{
      ledcWrite(5,0);
      ledcWrite(6,abs(r));
  }
//  if(y < 0){
//    ledcWrite(3,l);
//    ledcWrite(4,0);
//    ledcWrite(5,r);
//    ledcWrite(6,0);
//  }else{
//    ledcWrite(3,0);
//    ledcWrite(4,l);
//    ledcWrite(5,0);
//    ledcWrite(6,r);
//  }

}
