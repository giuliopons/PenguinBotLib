# PenguinBotLib refactor

Refactoring of the original Elegoo code for the PenguinBot version 2.

The original library is available at this address:
https://www.elegoo.com/blogs/arduino-projects/elegoo-samrt-robot-penguin-bot-tutorial

I decided to rewrite the original files because I have to teach a course to beginner students. I need a simpler way to make the robot walk, turn, measure distance, etc..

![image](https://github.com/giuliopons/PenguinBot/assets/1871627/f9962769-78e2-47cd-b678-49f4350c4cf7)

What I've changed:
* separated mp3 driver class to `MY1690_16S.cpp` and `MY1690_16S.h`
* removed not used code (functions, variables, defines)
* renamed functions with more correct names (ex: ir is infrared, but in Penguin v.2 there is bluetooth not ir)
* separated most of the code from main `PeguinBot.ino` file to `penguin.cpp` and `penguin.h` which now is a class
* made t (time unit) optional from functions, to simplify usage
* also changed volume functionalities and added a battery method to simplify usage
* pack everything into an Arduino IDE library, with examples
* the `PeguinBot.ino` now is an example and has the original functionalities of the original software
* added more examples and recreated the examples with new library
