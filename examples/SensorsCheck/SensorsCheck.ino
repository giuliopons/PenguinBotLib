#include <Penguin.h>

Penguin robot;

void setup()
{
  Serial.begin(9600);  
  robot.servoInit();
  robot.servoAttach();
  robot.homes(200);
  robot.servoDetach();  
}

void loop()
{
  
  int irL = robot.irLeft();
  int irR = robot.irRight();
  int distance = robot.getDistance();
  double bat = robot.getBatteryLevel();
  
  Serial.print("IR LEFT: ");
  Serial.print(irL);
  Serial.print(" IR RIGHT: ");
  Serial.print(irR);
  Serial.print(" DISTANCE: ");
  Serial.print(distance);
  Serial.print(" BATTERY: ");
  Serial.print(bat);
  Serial.println();

  delay(1000);
}