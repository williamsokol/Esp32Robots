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
  ledcSetup(6, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(4, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcSetup(5, 2000, 8); // 2000 hz PWM, 8-bit resolution
  //ledcSetup(6, 2000, 8); // 2000 hz PWM, 8-bit resolution
  ledcAttachPin(MotPin0, 6); //left 
  ledcAttachPin(MotPin1, 4); //left
  ledcAttachPin(MotPin2, 5); //right
  //ledcAttachPin(MotPin3, 6); //right
}

void controllMotors(float x, float y)
{
  // y is power in the forward/backwards
  // x is how much of the power should go in each wheel
  int m;
  float lr;

  

  m = abs(y) * 255;
  lr = (x+1)/2;

  lr = constrain(lr, 0,1);
//  constrain(m, 0,255);

  int l = (m*1.5)*(1-(lr));
  int r = (m*1.5)*(lr);

  l = constrain(l, 0,255);
  r = constrain(r, 0,255);

  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(m);
  Serial.print(" ");
  Serial.print(lr);
  Serial.print(" ");
  Serial.print(l);
  Serial.print(" ");
  Serial.println(r);
 
  
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
