#include "Penguin.h"

Penguin robot;

int THRESHOLD_IR = 400;


void setup()
{
  Serial.begin(9600);

  //EEPROM.write(0, 255);

  robot.servoInit();
  robot.servoAttach();
  robot.homes(200);
  robot.servoDetach();
  delay(5000);
  robot.servoAttach();
  

}


/* Realization of Obstacle Avoidance Mode*/
void loop() {


    int distance_value = robot.getDistance();
    bool turnFlag = true;
    
    robot.servoAttach();
    if (distance_value >= 1 && distance_value <= 250)
    {
        int st188Val_L = robot.irLeft();
        int st188Val_R = robot.irRight();
        if (st188Val_L >= THRESHOLD_IR && st188Val_R >= THRESHOLD_IR)
        {
            // Obstacle everywhere go back 3 steps
            robot.walk(3, -1);
            if (turnFlag)
            {
                // Go on the right (1), turn 3 steps right
                robot.turn(3, 1);
            }
            else
            {
                // Go on the left (2), turn 3 steps left
                robot.turn(3, -1);
            }
        }
        else if (st188Val_L >= THRESHOLD_IR && st188Val_R < THRESHOLD_IR)
        {
            // Obstacle on the left, turn 3 steps right
            turnFlag = true;
            robot.turn(3, 1);
        }
        else if (st188Val_L < THRESHOLD_IR && st188Val_R >= THRESHOLD_IR)
        {
            // Obstacle on the right, turn 3 steps left
            turnFlag = false;
            robot.turn(3, -1);
        }
        else if (st188Val_L < THRESHOLD_IR && st188Val_R < THRESHOLD_IR)
        {
            if (distance_value < 5)
            {
                // Obstacle in front, go back 3 steps
                robot.walk(3, -1);
                if (turnFlag)
                {
                    // Go on the right, turn 3 steps right
                    robot.turn(3, 1);
                }
                else
                {
                    // Go on the left, turn 3 steps left
                    robot.turn(3, -1);
                }
            }
            else if (distance_value >= 5 && distance_value <= 20)
            {
                // Obstacle in front 5<x<20");
                if (turnFlag)
                {
                    // Go on the right, turn 1 step right
                    robot.turn(1, 1);
                }
                else
                {
                    // Go on the left, turn1 steps left
                    robot.turn(1, -1);
                }
            }
            else
            {
                // Go on, free way, walk 1 step
                robot.walk(1, 1);
            }
        }
    }
    else
    {
        // Stop");
        robot.home();
    }

    robot.servoDetach();

}


