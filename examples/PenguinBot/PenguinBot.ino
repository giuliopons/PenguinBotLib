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
  YR 2<== |               | <== YL 3
           ---------------
              ||     ||
              ||     ||
  RR 0==^   -----   ------  v== RL 1
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
Penguin robot(mp3);

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

  
  //prog();
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

      robot.servoAttach();
      robot.obstacleMode();
      robot.servoDetach();

      break;
    case FOLLOW:

      if (millis() - preMp3MillisStop_FOLLOW > 1200) // Judgment timestamp: Turn off music playback (avoid random music broadcasting)
      {
        mp3.stopPlay();
      }
      robot.servoAttach();
      robot.followMode();
      robot.servoDetach();
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
            robot.dance2();
            break;
          case 3:
            robot.dance3();
            break;
          case 4:
            robot.dance4();
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
