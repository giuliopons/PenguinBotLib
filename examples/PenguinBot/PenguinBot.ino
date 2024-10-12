// Full code
//

#include "MsTimer2.h"       // timer lib used for bluetooth interaction
#include "MY1690_16S.h"     // mp3 chip lib

#include "Penguin.h"

/* Serial Bluetooth Communication Control Command Data Frame*/

// APP COMMANDS FOR MOVEMENT
#define BTN_UP    'f'
#define BTN_DOWN  'b'
#define BTN_LEFT  'l'
#define BTN_RIGHT 'i'
#define BTN_IDLE  's'

// APP OTHER COMMANDS
#define BTN_MUSIC    '1'
#define BTN_DANCE    '2'
#define BTN_OBSTACLE '3'
#define BTN_VOL_ADD  '4'
#define BTN_VOL_SUB  '5'
#define BTN_FOLLOW   '6'

// APP SERVO TUNING COMMANDS
#define BTN_RR_ADD   '7'
#define BTN_RL_ADD   '8'
#define BTN_YR_ADD   '9'
#define BTN_YL_ADD   '0'
#define BTN_RR_SUB   'a'
#define BTN_RL_SUB   'c'
#define BTN_YR_SUB   'd'
#define BTN_YL_SUB   'e'

// NEW CUSTOM COMMANDS
#define BTN_TRACK   'r'             // Play a random track
#define BTN_PROG    'p'              // execute the prog() function

/*
                    ---------------
                   |     O   O     |
                   |---------------|
  right leg [2]<== |               | ==> left leg [3]
                    ---------------
                       ||     ||
                       ||     ||
  right foot [0]<==  -----   ------  ==> left foot [1]
                    |-----   ------|
           
*/



enum MODE
{
  IDLE,
  BLUETOOTH,
  OBSTACLE,
  FOLLOW,
  MUSIC,
  DANCE,
  VOLUME,
  TRACK,
  PROG
} mode = IDLE; // Right Domain Key of Hand-Tour APP Control Interface

enum BTMODE
{
  FORWARD,
  BACKWAED,
  TURNRIGHT,
  TURNLIFT,
  STOP,
} BTmode = STOP; // Hand Tour APP Control Interface Left Domain Key


int musicIndex = 2;
int trackIndex = 8;
int musicNumber = 4;
int danceIndex = 2;
bool danceFlag = false;
bool progFlag = false;
unsigned long preMp3Millis;

unsigned long preMp3MillisStop_OBSTACLE;
unsigned long preMp3MillisStop_FOLLOW;

/* Hardware interface mapping mp3 */
#define SOFTWARE_RXD A2 //Software implementation of serial interface (audio module driver interface)
#define SOFTWARE_TXD A3
NeoSWSerial mp3Serial(SOFTWARE_RXD, SOFTWARE_TXD);
MY1690_16S mp3(mp3Serial);

// the new penguin object!
// Penguin robot(mp3);
Penguin robot;

/*
   Custom example program
*/
void prog() {

  robot.servoAttach();
  robot.home();

robot.run(15);
  
  // mp3.playSong(9);
  //robot.walk(10, 1);
  //robot.home();
  //robot.sitdown();
  robot.home();
  robot.servoDetach();

  delay(1000);
}

void getCommand() {
  robot.getCommand();
}

void setup()
{
  Serial.begin(9600);

  /* This means that every 50 milliseconds, the getCommand function is called, allowing the robot to check if new commands have been received through serial communication. */
  MsTimer2::set(50, getCommand);
  MsTimer2::start();

  mp3Serial.begin(9600);
  mp3.init();
 
  delay(1000);

  mp3.stopPlay();
  delay(10);

  robot.servoInit();
  robot.servoAttach();
  robot.homes(200);

  // start animation
  mp3.playSong(10);
  robot.lateral_fuerte(1);
  robot.lateral_fuerte(0);
  robot.goingUp();
  robot.servoDetach();

}


/* Realization of Obstacle Avoidance Mode*/
void obstacleMode() {

    bool turnFlag = true;
    
    int distance_value = robot.getDistance();
    int THRESHOLD_IR = robot.getThresholdIr();

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



/* Follow-up mode implementation*/
void followMode()
{
    int distance_value = robot.getDistance();

    int THRESHOLD_IR = robot.getThresholdIr();
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



/* dance sequences */
bool primera_parte()
{
    int t= robot.getTimeUnit();
    int move2[4] = {90, 90, 90, 90};
    if (robot.lateral_fuerte(1, t) ||
        robot.moveNServos(t * 0.5, move2) ||
        robot.lateral_fuerte(0, t) ||
        robot.moveNServos(t * 0.5, move2) ||
        robot.lateral_fuerte(1, t) ||
        robot.moveNServos(t * 0.5, move2) ||
        robot.lateral_fuerte(0, t) ||
        robot.home())
        return true;
    return false;
}

bool segunda_parte()
{
    double pause = 0;
    int t = robot.getTimeUnit();
  
    int move1[4] = {90, 90, 80, 100};
    int move2[4] = {90, 90, 100, 80};
    for (int x = 0; x < 3; x++)
    {
        for (int i = 0; i < 3; i++)
        {
            pause = millis();
            if (robot.moveNServos(t * 0.15, move1) ||
                robot.moveNServos(t * 0.15, move2))
                return true;
            while (millis() < (pause + t))
            {
                //if (irValue)
                  //  return true;
                if (robot.getSerialFlag())
                  {
                      return true;
                  }
            }
        }
    }
    if (robot.home())
        return true;
    return false;
}




/*Dance action part*/

void dance()
{
    int t= robot.getTimeUnit();

    primera_parte();
    segunda_parte();
    robot.moonWalkLeft(4);
    robot.moonWalkRight(4);
    robot.moonWalkLeft(4);
    robot.moonWalkRight(4);
    primera_parte();

    for (int i = 0; i < 16; i++)
    {
        robot.flapping(1, t / 4);
        robot.delays(3 * t / 4);
    }

    robot.moonWalkRight(4);
    robot.moonWalkLeft(4);
    robot.moonWalkRight(4);
    robot.moonWalkLeft(4);

    robot.drunk(t * 4);
    robot.drunk(t * 4);
    robot.drunk(t * 4);
    robot.drunk(t * 4);
    robot.kickLeft(t);
    robot.kickRight(t);
    robot.drunk(t * 8);
    robot.drunk(t * 4);
    robot.drunk(t / 2);
    robot.delays(t * 4);

    robot.drunk(t / 2);

    robot.delays(t * 4);
    robot.walk(2, 1, t * 3);
    robot.home();
    robot.backyard(2, t * 2);
    robot.home();
    robot.goingUp(t * 2);
    robot.goingUp(t * 1);
    robot.noGravity(t);

    robot.delays(t);
    primera_parte();
    for (int i = 0; i < 32; i++)
    {
        robot.flapping(1, t / 2);
        robot.delays(t / 2);
    }

    // for (int i = 0; i < 4; i++)
    //     servo[i].SetPosition(90);

    robot.home();
}

void dance2()
{
    int t= robot.getTimeUnit();
    if (robot.lateral_fuerte(1, t) ||
        robot.lateral_fuerte(0, t) ||
        robot.drunk(t / 2) ||
        robot.drunk(t) ||
        robot.kickLeft(t) ||
        robot.kickRight(t) ||
        robot.walk(2, 1, t * 3) ||
        robot.home() ||
        robot.backyard(2, t * 4) ||
        robot.noGravity(t) ||
        robot.lateral_fuerte(1, t) ||
        robot.lateral_fuerte(0, t) ||
        segunda_parte() ||
        robot.upDown(5, 500))
        return;
}

void dance3()
{
    int t= robot.getTimeUnit();
    if (robot.sitdown() ||
        robot.legRaise(1, t) ||
        robot.swing(5, t) ||
        robot.legRaise1(1,t) ||
        robot.walk(2, 1, t * 3) ||
        robot.home() ||
        robot.noGravity(t) ||
        robot.kickRight(t) ||
        robot.goingUp(t) ||
        robot.kickLeft(t) ||
        robot.legRaise4(1, t) ||
        robot.backyard(2, t * 4) ||
        robot.drunk(t) ||
        robot.lateral_fuerte(1, 500) ||
        robot.lateral_fuerte(0, 500) ||
        robot.sitdown())
        return;
}

void dance4()
{
  int t= robot.getTimeUnit();
    if (robot.flapping(1, t) ||
        robot.drunk(t) ||
        robot.kickLeft(t) ||
        robot.walk(2, 1, t * 3) ||
        robot.home() ||
        robot.lateral_fuerte(0, t) ||
        robot.sitdown() ||
        robot.legRaise(1, t) ||
        robot.swing(5, t) ||
        robot.backyard(2, t * 4) ||
        robot.goingUp(t) ||
        robot.noGravity(t) ||
        robot.upDown(5, t) ||
        robot.legRaise1(1, t) ||
        robot.legRaise2(4, 0, t) ||
        robot.kickRight(t) ||
        robot.goingUp(t) ||
        robot.legRaise3(4, 1,t) ||
        robot.kickLeft(t) ||
        robot.legRaise4( 1, t) ||
        segunda_parte() ||
        robot.sitdown())
        return;
}






void loop()
{

  char btValue = robot.getBluetoothValue();
  
  if (btValue != '\0')// Bluetooth serial port data stream on app side (character acquisition is completed in timer 2)
  {
    robot.setSerialFlag(false);
    switch (btValue)
    {
      case BTN_UP:
        mp3.stopPlay();
        mode = BLUETOOTH;
        BTmode = FORWARD;
        break;
      case BTN_DOWN:
        mp3.stopPlay();
        mode = BLUETOOTH;
        BTmode = BACKWAED;
        break;
      case BTN_LEFT:
        mp3.stopPlay();
        mode = BLUETOOTH;
        BTmode = TURNLIFT;
        break;
      case BTN_RIGHT:
        mp3.stopPlay();
        mode = BLUETOOTH;
        BTmode = TURNRIGHT;
        break;
      case BTN_IDLE:
        mp3.stopPlay();
        mode = IDLE;
        robot.servoAttach();
        robot.homes(200);
        robot.servoDetach();
        break;
      case BTN_MUSIC:
        robot.servoDetach();
        mp3.stopPlay();
        mode = MUSIC;
        musicIndex++;
        if (musicIndex > musicNumber)
        {
          musicIndex = 2;
        }
        mp3.playSong(musicIndex);
        preMp3Millis = millis();
        break;
      case BTN_TRACK:
        robot.servoAttach();
        robot.homes(200);
        robot.servoDetach();
        mp3.stopPlay();
        mode = TRACK;
        trackIndex++;
        if (trackIndex > 30)
        {
          trackIndex = 8;
        }
        mp3.playSong(trackIndex);
        preMp3Millis = millis();
        break;
      case BTN_PROG:
        progFlag = true;
        mode = PROG;
        break;
      case BTN_DANCE:
        mode = DANCE;
        danceFlag = true;
        danceIndex++;
        if (danceIndex > 4)
        {
          danceIndex = 2;
        }
        mp3.playSong(danceIndex);
        break;
      case BTN_OBSTACLE:
        robot.delays(10);
        mp3.stopPlay();
        robot.delays(10);
        mp3.playSong(7);
        mode = OBSTACLE;
        //preMp3MillisStop_OBSTACLE = 0;
        preMp3MillisStop_OBSTACLE = millis();
        break;
      case BTN_VOL_ADD:
        mp3.volumePlus();
        break;
      case BTN_VOL_SUB:
        mp3.volumeDown();
        break;
      case BTN_FOLLOW:
        robot.delays(10);
        mp3.stopPlay();
        robot.delays(10);
        mp3.playSong(6);
        mode = FOLLOW;
        preMp3MillisStop_FOLLOW = 0;
        preMp3MillisStop_FOLLOW = millis();
        break;
      case BTN_RR_ADD:
        robot.trimServo(BTN_RR_ADD);
        break;
      case BTN_RL_ADD:
        robot.trimServo(BTN_RL_ADD);
        break;
      case BTN_YR_ADD:
        robot.trimServo(BTN_YR_ADD);
        break;
      case BTN_YL_ADD:
        robot.trimServo(BTN_YL_ADD);
        break;
      case BTN_RR_SUB:
        robot.trimServo(BTN_RR_SUB);
        break;
      case BTN_RL_SUB:
        robot.trimServo(BTN_RL_SUB);
        break;
      case BTN_YR_SUB:
        robot.trimServo(BTN_YR_SUB);
        break;
      case BTN_YL_SUB:
        robot.trimServo(BTN_YL_SUB);
        break;
      default:
        break;
    }
    if (robot.getSerialFlag() == false)
    {
      robot.setBluetoothValue('\0'); // Data Command Clearing Serial Cache
    }
  }







  switch (mode)
  {
    case IDLE:
      mp3.stopPlay();
      robot.servoDetach();
      break;
    case BLUETOOTH:
      switch (BTmode)
      {
        case FORWARD:
          robot.servoAttach();
          robot.walk(1, 1);
          robot.servoDetach();
          break;
        case BACKWAED:
          robot.servoAttach();
          robot.walk(1, -1);
          robot.servoDetach();
          break;
        case TURNRIGHT:
          robot.servoAttach();
          robot.turn(1, 1);
          robot.servoDetach();
          break;
        case TURNLIFT:
          robot.servoAttach();
          robot.turn(1, -1);
          robot.servoDetach();
          break;
        default:
          break;
      }
      break;

      
    case OBSTACLE:
      if (millis() - preMp3MillisStop_OBSTACLE > 1200) // Judgment timestamp: Turn off music playback (avoid random music broadcasting)
      {
        mp3.stopPlay();
      }

      // robot.servoAttach();
      obstacleMode();
      // robot.servoDetach();

      break;
    case FOLLOW:

      if (millis() - preMp3MillisStop_FOLLOW > 1200) // Judgment timestamp: Turn off music playback (avoid random music broadcasting)
      {
        mp3.stopPlay();
      }
      // robot.servoAttach();
      followMode();
      // robot.servoDetach();
      break;
    case MUSIC:
      if (millis() - preMp3Millis > 1000)
      {
        preMp3Millis = millis();
        if (mp3.getPlayStatus() == mp3.playStatus[0]) // STOPPED
        {
          musicIndex++;
          if (musicIndex > musicNumber)
          {
            musicIndex = 2;
          }
          mp3.playSong(musicIndex);
        }
      }
      break;
    case TRACK:
      if (trackIndex > 0) {
        if (millis() - preMp3Millis > 1000)
        {
          preMp3Millis = millis();
          if (mp3.getPlayStatus() == mp3.playStatus[0]) // STOPPED
          {
            trackIndex++;
            if (trackIndex > 30)
            {
              trackIndex = 8;
            }
            mp3.playSong(trackIndex);
          }
        }
      }
      break;
    case DANCE:
      if (danceFlag == true)
      {
        robot.servoDetach();
        robot.delays(10);
        mp3.stopPlay();
        robot.delays(10);
        mp3.playSong(danceIndex);
        robot.servoAttach();
        switch (danceIndex)
        {
          case 2:
            dance2();
            break;
          case 3:
            dance3();
            break;
          case 4:
            dance4();
            break;
          default:
            break;
        }
        robot.homes(200);
        robot.servoDetach();
        danceFlag = false;
        mp3.stopPlay();
        delay(10);
        mp3.stopPlay();
        delay(10);
        mp3.stopPlay();
        delay(10);
        
      }
      break;

    case PROG:
      if (progFlag == true)
      {
        robot.servoDetach();
        robot.delays(10);
        mp3.stopPlay();
        robot.delays(10);
        robot.servoAttach();
        prog(); // custom function
        robot.homes(200);
        robot.servoDetach();
        progFlag = false;
        mp3.stopPlay();
        delay(10);
        mp3.stopPlay();
        delay(10);
        mp3.stopPlay();
        delay(10);
        
      }
      break;

    default:
      break;
  }
}