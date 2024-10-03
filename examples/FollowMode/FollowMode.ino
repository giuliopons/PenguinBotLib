#include "Penguin.h"

Penguin robot;

int THRESHOLD_IR = 400;

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
  delay(5000);
  robot.servoAttach();
  

}



/* Follow-up mode implementation*/
void loop()
{
    int distance_value = robot.getDistance();

    int st188Val_L = robot.irLeft();
    int st188Val_R = robot.irRight();
      
    robot.servoAttach();

    if ( ( distance_value >= 5 && distance_value <= 40 ) || (st188Val_L >= THRESHOLD_IR || st188Val_R >= THRESHOLD_IR)  )
    {


        if (st188Val_L >= THRESHOLD_IR && st188Val_R >= THRESHOLD_IR) // it's near
        {
            robot.home();
            delay(500);
            
        }
        else if (st188Val_L >= THRESHOLD_IR && st188Val_R < THRESHOLD_IR)// it's on the left
        {
            robot.turn(1, -1);
            
        }
        else if (st188Val_L < THRESHOLD_IR && st188Val_R >= THRESHOLD_IR) // it's on the right
        {
            robot.turn(1, 1);
            
        }
        else if (distance_value > 20) // too far, stop
            {
                robot.home();
                delay(500);
            }
            else // it's near, follow
            {
                robot.walk(1, 1);
            }
        
    }
    else
    {
        robot.home();
        delay(500);
    }
    robot.servoDetach();
}



