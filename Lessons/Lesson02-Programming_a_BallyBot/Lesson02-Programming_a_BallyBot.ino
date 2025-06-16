void setup() {
  pinMode(14, OUTPUT); /* both wheel backward */
  pinMode(12, OUTPUT); /* right wheel forward */
  pinMode(2, OUTPUT);  /* left wheel forward */
}
void loop() {
  move(LOW,LOW,LOW, 3000);  /* stay off */
  move(HIGH,HIGH,LOW,1000); /* move forward */
  move(LOW,LOW,HIGH, 500);  /* jolt backward */
  move(HIGH,HIGH,LOW,2000); /* go forward again to land */
}
void move(bool l, bool r, bool b, int time){
  digitalWrite(2, l);   /* left motor forward */
  digitalWrite(12, r);  /* right motor forward */
  digitalWrite(14, b);  /* both motors backward */
  delay(time);        /* time till next step */
}