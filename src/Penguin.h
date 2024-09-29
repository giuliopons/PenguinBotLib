#ifndef PENGUIN_H
#define PENGUIN_H

#define N_SERVOS 4
#define INTERVALTIME 10.0
#define CENTRE 90
#define AMPLITUDE 30
#define ULTRA_HIGH_RATE 0.3
#define HIGH_RATE 0.5
#define MID_RATE 0.7
#define LOW_RATE 1.0
#define ULTRA_LOW_RATE 1.5

#define YL_PIN 10
#define YR_PIN 9
#define RL_PIN 12
#define RR_PIN 6

#define BTN_RR_ADD   '7'
#define BTN_RL_ADD   '8'
#define BTN_YR_ADD   '9'
#define BTN_YL_ADD   '0'

#define BTN_RR_SUB   'a'
#define BTN_RL_SUB   'c'
#define BTN_YR_SUB   'd'
#define BTN_YL_SUB   'e'

#define ECHO_PIN 4//Ultrasound interface
#define TRIG_PIN 5

#define ST188_R_PIN A1//Infrared Controller Interface
#define ST188_L_PIN A0

#define INDICATOR_LED_PIN A5 //LED Indicator Interface

#define VOLTAGE_MEASURE_PIN A4 //Voltage Detection Interface


#include <Arduino.h>
#include "Oscillator.h"
#include <EEPROM.h>         // for servo fine tunings values
#include "MY1690_16S.h"     // mp3 chip lib
#include "MsTimer2.h"       // used for bluetooth interaction


class Penguin {
  public:
    Penguin(MY1690_16S& mp3Instance);
    Penguin();

    /* movement methods */
    bool walk(int steps, int dir, int T = -1);
    bool moveNServos(int time, int newPosition[]);
    bool delays(unsigned long ms);
    void servoAttach();
    void servoDetach();
    void servoInit();
    bool home();
    void homes(int T = -1);
    bool oscillate(int A[N_SERVOS], int O[N_SERVOS], int T, double phase_diff[N_SERVOS]);
    bool turn(int steps, int dir, int T = -1);
    bool moonWalkRight(int steps, int T = -1);
    bool moonWalkLeft(int steps, int T = -1);
    bool crusaito(int steps, int T = -1);
    bool swing(int steps, int T = -1);
    bool upDown(int steps, int T = -1);
    bool flapping(int steps, int T = -1);
    bool run(int steps, int T = -1);
    bool backyard(int steps, int T = -1);
    bool goingUp(int T = -1);
    bool drunk(int T = -1);
    bool noGravity(int T =-1);
    bool kickLeft(int T=-1);
    bool kickRight(int T=-1);
    bool legRaise(int dir, int T=-1);
    bool legRaise1(int dir, int T=-1);
    bool legRaise2(int steps, int dir, int T=-1);
    bool legRaise3(int steps, int dir, int T=-1);
    bool legRaise4(int dir, int T = -1);
    bool sitdown();
    bool lateral_fuerte(boolean dir, int T=-1);
    bool primera_parte();
    bool segunda_parte();
    void dance();
    void dance2();
    void dance3();
    void dance4();

    /* bt serial command */
    void setSerialFlag(boolean flag);
    bool getSerialFlag();
    char getBluetoothValue();
    void setBluetoothValue(char v);
    void getCommand();

    /* settings */
    void trimServo(char btn);

    /* startup anim */
    void startAnimation();

    /* sensor methods */
    int getDistance();
    double batteryLevel();
    
    void obstacleMode();
    void followMode();
    void st188Adjust(int dis);
    void Test_voltageMeasure(void);
  


  private:
    /* fine-tuning temporary storage variables*/
    int trim_rr;
    int trim_rl;
    int trim_yr;
    int trim_yl;
    int addr_trim_rr = 0;
    int addr_trim_rl = 1;
    int addr_trim_yr = 2;
    int addr_trim_yl = 3;

    MY1690_16S& mp3;

    int t = 495;                    // time unit
    double pause = 0;
    char btValue = '\0';            // it's "null", so if('\0') è false
    bool serial_flag = false;       // becomes "true" when there is serial communication
    float increment[N_SERVOS];
    unsigned long final_time;
    unsigned long interval_time;
    int oneTime;
    int iteration;
    
    Oscillator servo[N_SERVOS];
    int oldPosition[N_SERVOS];

    double distance_value = 0;
    
    int st188Val_L;                 // left infra red
    int st188Val_R;                 // right infra red

    long int ST188Threshold;
    long int ST188RightDataMin;
    long int ST188LeftDataMin;

    unsigned long infraredMeasureTime;

    unsigned long voltageMeasureTime;

  
};

#endif // PENGUIN_H
