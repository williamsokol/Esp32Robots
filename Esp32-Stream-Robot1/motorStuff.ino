const int MotPin0 = 14;  
const int MotPin1 = 12;  
const int MotPin2 = 2;  
//const int MotPin3 = 2;  

void initMotors() 
{
  pinMode(MotPin0, OUTPUT);
  pinMode(MotPin1, OUTPUT);
  pinMode(MotPin2, OUTPUT);
  //pinMode(MotPin3, OUTPUT);
  ledcSetup(3, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(4, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(5, 2000, 8); // 2000 hz PWM, 8-bit resolution
  //ledcSetup(6, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcAttachPin(MotPin0, 3); //left 
  ledcAttachPin(MotPin1, 4); //left
  ledcAttachPin(MotPin2, 5); //right
  //ledcAttachPin(MotPin3, 6); //right
}

void controllMotors(float x, float y)
{
  // y is power in the forward/backwards
  // x is how much of the power should go in each wheel
  int l,r,m;

  
  l = (y+x)*255*2;
  r = (y-x)*255*2;
  m = ((y+1)/2) * 255*2;
  
//  Serial.print(x);
//  Serial.print(" ");
//  Serial.print(y);
//  Serial.print(" ");
//  Serial.print(l);
//  Serial.print(" ");
//  Serial.println(r);

  constrain(l, 0,255);
  constrain(r, 0,255);
  constrain(m, 0,255);

//
//  if (l < 0){
//      ledcWrite(3,l);
//      ledcWrite(4,0);
//  }else{
//      ledcWrite(3,0);
//      ledcWrite(4,abs(l));
//  }
//  if (r < 0){
//      ledcWrite(5,r);
//      ledcWrite(6,0);
//  }else{
//      ledcWrite(5,0);
//      ledcWrite(6,abs(r));
//  }
  if(y < 0){
    //ledcWrite(3,0);
    ledcWrite(4,m);
    ledcWrite(5,0);
    //ledcWrite(6,0);
  }else{
    //ledcWrite(3,m);
    ledcWrite(4,0);
    ledcWrite(5,m);
    //ledcWrite(6,r);
  }

}
