# PenguinBot

Refactoring of the original Elegoo code for the PenguinBot version 2.

![image](https://github.com/giuliopons/PenguinBot/assets/1871627/f9962769-78e2-47cd-b678-49f4350c4cf7)

What I've changed:
* separate mp3 driver class to `MY1690_16S.cpp` and `MY1690_16S.h`
* separate most of the code from main peguinbot.ino file to `penguin.cpp` and `penguin.h`
* removed not used code (functions, variables, defines)
* added a `prog` function to handle a new simple class for experiments
* renamed functions with more correct names (ex: ir is infrared, but in Penguin v.2 there is bluetooth not ir)
* bluetooth behaviour with original app is still the same
* made t (time unit) optional from function, to simplify usage
* simplify usage (volume, battery)
* pack it into an Arduino IDE library
* made examples
