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
  ledcAttachPin(MotPin0, 3); 
  ledcAttachPin(MotPin1, 4); 
  ledcAttachPin(MotPin2, 5); 
  ledcAttachPin(MotPin3, 6); 
}

void controllMotors(int x, int y)
{
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);

  if(x > 0){
    ledcWrite(3,x);
    
  }

}
