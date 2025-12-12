/*

	how to use hook delay
	to add some light behaviour
	during the "delays" function
	used during the movements, in this way
	we can do something like turning on
	and off a light while walking.

	in this example we have added a led 
	to the pin 2 of the Arduino Nano
	which is free


*/

#include "PenguinBotLib.h"  // lib for robot

// Bob is our robot
Penguin bob;

void lightsTask() {
  static unsigned long last = 0;
  if (millis() - last > 200) {
    digitalWrite(2, !digitalRead(2));
    last = millis();
  }
}

void setup()
{
  // serial communication for debug
  Serial.begin(9600);     
  delay(1000);

  // a led attached on free pin n.2
  pinMode(2,OUTPUT);

  // start servos
  bob.servoInit();

  // home position for servos
  bob.servoAttach();
  bob.homes(200);
  bob.servoDetach();
  
  delay(2000);
  randomSeed(analogRead(0));

  bob.setDelayHook(lightsTask);
}    


void loop()
{   
     if ( random(1,200) == 8) {

        // sometimes make some funny movement
        // with lights!
        Serial.println("funny move");
        delay(500);
        bob.servoAttach();
        bob.drunk();
        bob.walk(4,1);
        if(random(1,3)==1) bob.lateral_fuerte(1); else bob.lateral_fuerte(0);
        bob.homes();
        bob.servoDetach();
        delay(500);

    }

}