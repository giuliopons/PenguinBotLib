/*

    this is a shy robot... if nothing approaches, it makes a funny move.
    if it sees an obstacle it goes back.
    it turns on the other side when something is on the left or on the right.
    it plays funny sounds.


*/

#include "MY1690_16S.h"     // lib for mp3
#include "PenguinBotLib.h"  // lib for robot

// mp3 handler on pin A2 and A3
NeoSWSerial mp3Serial(A2, A3);
MY1690_16S mp3(mp3Serial);

// Bob is our robot
Penguin bob;

int distance;       // var to store distance

void setup()
{
  // serial communication for debug
  Serial.begin(9600);     
  delay(1000);
  
  // mp3 communication
  mp3Serial.begin(9600);  
  mp3.init();
  delay(1000);
  mp3.stopPlay();
  delay(10);

  // start servos
  bob.servoInit();

  // home position for servos
  bob.servoAttach();
  bob.homes(200);
  bob.servoDetach();
  
  delay(2000);
  randomSeed(analogRead(0));
}    


// play an mp3 and wait the end of the mp3
void playAndWaitTheEnd(int x) {
  Serial.println("mp3.playSong(" + (String)x + ")");
  String status = "000000";
  mp3.playSong(x);
  while( status.substring(0, 6) != "OK0000") {
    status = mp3.getPlayStatus();
    delay(100);
  }
}

void loop()
{

    // measure distance
    distance = bob.getDistance();
    delay(100); // wait the end of the distance measurement

    if (distance <=10) {

      Serial.println("everything is ok");

      // there is something near, play a random alert sount and walk backward
      int m = random(10,14);
      
      mp3.playSong(m);
      bob.servoAttach();
      bob.walk(3,0);
      bob.homes();
      bob.servoDetach();
      
      delay(100);
    
    } else {

      
      if ( random(1,200) == 8) {

        // sometimes make some funny movement
        Serial.println("funny move");
        delay(500);
        bob.servoAttach();
        bob.drunk();
        if(random(1,3)==1) bob.lateral_fuerte(1); else bob.lateral_fuerte(0);
        bob.homes();
        bob.servoDetach();
        delay(500);
      }

    }

    if (bob.irLeft() > 700) {
        
        Serial.println("something on the left, turn on the other side");
        mp3.playSong(16);
        bob.servoAttach();
        bob.turn(1,1);
        bob.homes();
        bob.servoDetach();
    }
    if (bob.irRight() > 700) {
        Serial.println("something on the right, turn on the other side");
        mp3.playSong(22);
        bob.servoAttach();
        bob.turn(1,0);
        bob.homes();
        bob.servoDetach();
    }
    
}
