#include <PenguinBotLib.h>

Penguin robot;

void setup()
{
  Serial.begin(9600);  
  double bat = robot.getBatteryLevel();
  Serial.print("Battery level is ");
  Serial.println(bat);
  
  robot.servoInit();
  robot.servoAttach();
  robot.homes(200);
  robot.servoDetach();

  randomSeed( analogRead(0) );


}

void loop()
{
  
  robot.servoAttach();

  // make a random number of steps
  int steps = random(3,10);
  Serial.print("Walk for steps ");
  Serial.println(steps);
  robot.walk( steps, 1);
  robot.home();

  // make a random turn left or right
  int dir = random(0,1);
  String direction = "";
  if(dir == 0) {
    direction = "left";
  } else {
    direction = "right";
  }

  steps = random(1,3);
  Serial.print("Turn " + direction + " for steps ");
  Serial.println(steps);
    
  robot.turn( steps, dir);
  robot.home();
  
  robot.servoDetach();

  Serial.print("Wait for seconds ");
  Serial.println(steps);
  delay( steps * 1000); 

}
