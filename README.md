# PenguinBotLib remake

![image](https://github.com/giuliopons/PenguinBot/assets/1871627/f9962769-78e2-47cd-b678-49f4350c4cf7)

### Disclaimer
Hi. I rewrited the original files of the Elegoo Penguin bot 2.0 kit because I have to teach a robotic course to young beginner students and I need a simple way to start play with the Elegoo kit: for example, I need to make the robot walk, turn, measure distance, etc. with a few commands and the original code is too complex for my students, so I made this Arduino lib.

Please, consider that this code is free and comes as it is and it's only for educational purposes, if any of the libraries reused for this repo violates some rights, please contact me to remove.

The original library for Elegoo Penguin Bot 2.0 is available at this address:
https://www.elegoo.com/blogs/arduino-projects/elegoo-samrt-robot-penguin-bot-tutorial


Some of the things I've changed:

* separated mp3 driver class to `MY1690_16S.cpp` and `MY1690_16S.h`
* removed not used code (functions, variables, defines)
* renamed functions with more correct names (ex: ir is infrared, but in Penguin v.2 there is bluetooth not ir)
* separated most of the code from main `PeguinBot.ino` file to `penguin.cpp` and `penguin.h` which now is a class
* made t (time unit) optional from functions, to simplify usage
* also changed volume functionalities
* pack everything into an Arduino IDE library, with examples
* added methods to access sensor values (ir, distance and battery)
* added method to turn on and off the light
* the original `PeguinBot.ino` now is an example file and it has all the original functionalities of the original software
* added more examples
* added more sounds

## Library files:

* `Penguin` (.h, .cpp) : main library to access the basic movements of the robot and the sensors
* `Oscillator` (.j, .cpp) : Juan Gonzalez-Gomez oscillator library to move servos in sync
* `MY1690_16S` (.h, .cpp) : library to use the mp3/audio functions throught software serial communication
* `NeoSWSerial` (.h, .cpp) : library to perform software serial communication via specific pins, used for audio
* `MsTimer2` (.h, .cpp) : library to handle a timer function to monitor Serial communication between BT and robot throught pins 0,1

## Examples:

* `SensorsCheck.ino` : show alues of sensors
* `Followmode.ino` : follow a near target
* `ObstacleMode.ino` : avoid an obstacle
* `ObstacleModeWithSounds.ino` : avoid an obstacle
* `Walkround.ino` : walk around if there are no obstacles
* `PenguinBot.ino`: refactor of the original code with all functionalities
* `WithoutLibs.ino` : an example of accessing servos and sensors without using libs

## Mp3 files note:

The first 7 files are the original mp3 files from Elegoo Kit. The other mp3 come around from the web from r2d2 library (https://www.101soundboards.com/) and some other sound effects from free libraries.

