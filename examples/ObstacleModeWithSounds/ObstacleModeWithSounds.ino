#include "MY1690_16S.h"     // mp3 chip lib

#include "PenguinBotLib.h"

/* Hardware interface mapping mp3 */
#define SOFTWARE_RXD A2 //Software implementation of serial interface (audio module driver interface)
#define SOFTWARE_TXD A3
NeoSWSerial mp3Serial(SOFTWARE_RXD, SOFTWARE_TXD);
MY1690_16S mp3(mp3Serial);

Penguin robot;

void setup()
{
  Serial.begin(9600);
  delay(1000);
  mp3Serial.begin(9600);
  mp3.init();
 
  delay(1000);

  mp3.stopPlay();
  delay(10);

  robot.servoInit();
  robot.servoAttach();
  robot.homes(200);
  robot.servoDetach();
  mp3.playSong(21);
  delay(5000);
  robot.servoAttach();
  robot.setTimeUnit(490);       // robot speed
  randomSeed(analogRead(A0));   // randomize
  

}

int threshold_ir = 200;
int distance = 250;
int ir_L;
int ir_R;
bool flag;  // true on obstacle near, seen with ir
       
void obstacleMode2() {

  // start a cycle



  // walk until an obstacle is found
  flag = false;
  while(distance > 10 && flag == false) {

    mp3.playSong(34);
    // make a step
    robot.walk(1,1);

    // see with sensors
    distance = robot.getDistance();
    ir_L = robot.irLeft();
    ir_R = robot.irRight();

    if( (ir_L >= threshold_ir || ir_R  >= threshold_ir) ) {
          // obstacle
          flag = true;
        } else {
          // free around
          flag = false;
        } 
    
  }


  // turn left or right with ir, until freeway
  int dir = 0;
  if (flag) {
    // obstacle is near, seen with ir
    
    if ( ir_R >= threshold_ir ) {
      // obstacle on the right
      dir = -1;
    } else if ( ir_L >= threshold_ir ) {
      // obstacle on the left
      dir = 1;
    }
    
  } else {
    // obstacle is "far" at 8-9 cm
    
    // choose a random turn direction (1 or -1)
    dir = random(1,3);
    if(dir == 2) dir = -1;
    
  }



  while (distance <=10 || flag == true) {
    mp3.playSong(17);

    // turn around to see a free way
    robot.turn(1,dir);
    
    distance = robot.getDistance();
    ir_L = robot.irLeft();
    ir_R = robot.irRight();
    if( (ir_L >= threshold_ir || ir_R  >= threshold_ir) ) {
      flag = true;
    } else {
      flag = false;
    }
    
  }

  // restart...
  
}


void loop()
{

  obstacleMode2();
  
}