//
// can access penguin bot servos and sensors
// directly, without original libs
//
// note:
// place the robot upside down to test
// movements of servos
//

#include <Servo.h>

Servo lfoot; 
Servo lleg; 
Servo rfoot; 
Servo rleg; 

void setup() {  
  Serial.begin(9600);
  
  lfoot.attach(12);
  lleg.attach(10);
  rfoot.attach(6);
  rleg.attach(9);

  lfoot.write(90);
  delay(500);
  lleg.write(90);
  delay(1000);
  rfoot.write(90);
  delay(500);
  lleg.write(90);
  delay(1000);

  //sensore distanza
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
}

void loop() {  
  // IR sensors
  int irl = analogRead(A0);
  int irr = analogRead(A1);
  Serial.print(irl);
  Serial.print(" - ");
  Serial.println(irr);
  delay(100);

  // HCSR04 distance sensor 
  digitalWrite( 5, LOW );
  digitalWrite( 5, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( 5, LOW );
   
  long duration = pulseIn( 4, HIGH );   
  long distance = 0.034 * duration / 2;
   
  Serial.print("distance: ");
  Serial.println(distance);

  //
  // test movement
  lleg.write(60);
  delay(2000);
  lleg.write(90);
  delay(2000);
  
  rleg.write(60);
  delay(2000);
  rleg.write(90);
  delay(2000);

  lfoot.write(60);
  delay(2000);
  lfoot.write(90);
  delay(2000);

  rfoot.write(60);
  delay(2000);
  rfoot.write(90);
  delay(2000);
  
  
}