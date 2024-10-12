#include "PenguinBotLib.h"

/** 
 * Constructor
 */
Penguin::Penguin() {

    for (int i = 0; i < N_SERVOS; i++) {
      oldPosition[i] = CENTRE;
    }

    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(INDICATOR_LED_PIN, OUTPUT);
    pinMode(VOLTAGE_MEASURE_PIN, INPUT);
    analogWrite(INDICATOR_LED_PIN, HIGH);

}

 

/**
 * a delay that could be interrupted by bluetooth control
 */
bool Penguin::delays(unsigned long ms)
{
    for (unsigned long i = 0; i < ms; i++)
    {
        if (serial_flag)
        {
            return true;
        }
        delay(1);
    }
    return false;
}


/**
 * set serial flag, if true there is communication from serial (bluetooth) and
 * so delays and movement are interrupted
 */
void Penguin::setSerialFlag(boolean flag) {
  serial_flag = flag;
}

/**
 * get serial flag value stored
 */
bool Penguin::getSerialFlag() {
  return serial_flag;
}

void Penguin::setBluetoothValue(char v) {
  btValue = v;
}

char Penguin::getBluetoothValue() {
  return btValue;
}

/* Control command acquisition implementation*/
void Penguin::getCommand()
{
    if (Serial.available())
    {
        btValue = Serial.read();
        if (btValue && btValue != '\0')
        {
            // Serial.print("new data: ");
            // Serial.println(irValue);
            serial_flag = true;
        }
        else
        {
            // Serial.print("error data: ");
            // Serial.println(irValue);
            btValue = '\0';
        }
    }
     Test_voltageMeasure();// Realization of Voltage Detection
}

/**
 * Servo initialization attachment
 */
void Penguin::servoAttach()
{
    //
    servo[0].SetTrim(trim_rr);
    servo[1].SetTrim(trim_rl);
    servo[2].SetTrim(trim_yr);
    servo[3].SetTrim(trim_yl);
   
    servo[0].attach(RR_PIN);
    servo[1].attach(RL_PIN);
    servo[2].attach(YR_PIN);
    servo[3].attach(YL_PIN);
}

/**
 * Servo detachment
 */
void Penguin::servoDetach()
{
    servo[0].detach();
    servo[1].detach();
    servo[2].detach();
    servo[3].detach();
}

/**
 * Servo trim reset write 255 to EEPROM data
 */
void Penguin::resetTrim () {
    EEPROM.write(addr_trim_rr, 255);
    EEPROM.write(addr_trim_rl, 255);
    EEPROM.write(addr_trim_yr, 255);
    EEPROM.write(addr_trim_yl, 255);
}

/**
 * Servo initialization, use values in EEPROM to update trim values
 * trim_rr, trim_rl, trim_yr, trim_yl are used in servoAttach
 */
void Penguin::servoInit()
{
    
    if (EEPROM.read(addr_trim_rr) != 255)
    {
        trim_rr = EEPROM.read(addr_trim_rr) - 90; 
    }
    else
    {
        trim_rr = 0;
    }

    if (EEPROM.read(addr_trim_rl) != 255)
    {
        trim_rl = EEPROM.read(addr_trim_rl) - 90; 
    }
    else
    {
        trim_rl = 0;
    }

    if (EEPROM.read(addr_trim_yr) != 255)
    {
        trim_yr = EEPROM.read(addr_trim_yr) - 90; 
    }
    else
    {
        trim_yr = 0;
    }

    if (EEPROM.read(addr_trim_yl) != 255)
    {
        trim_yl = EEPROM.read(addr_trim_yl) - 90; 
    }
    else
    {
        trim_yl = 0;
    }
}

/**
 * Update trim servo from button pressed in the application 
 */
void Penguin::trimServo(char btn) {

  if(btn == BTN_RR_ADD) {
    trim_rr++;
    trim_rr = constrain(trim_rr, -90, 90);
    EEPROM.write(addr_trim_rr, trim_rr + 90);
  }

  if(btn == BTN_RL_ADD) {
    trim_rl++;
    trim_rl = constrain(trim_rl, -90, 90);
    EEPROM.write(addr_trim_rl, trim_rl + 90);
  }

  if(btn==BTN_YR_ADD){
      trim_yr++;
      trim_yr = constrain(trim_yr, -90, 90);
      EEPROM.write(addr_trim_yr, trim_yr + 90);
  }
   if(btn==BTN_YL_ADD) {
      trim_yl++;
      trim_yl = constrain(trim_yl, -90, 90);
      EEPROM.write(addr_trim_yl, trim_yl + 90);
   }
   if(btn==BTN_RR_SUB){
    trim_rr--;
    trim_rr = constrain(trim_rr, -90, 90);
    EEPROM.write(addr_trim_rr, trim_rr + 90);
   }
   if(btn==BTN_RL_SUB) {
      trim_rl--;
      trim_rl = constrain(trim_rl, -90, 90);
      EEPROM.write(addr_trim_rl, trim_rl + 90);
   }
   if(btn==BTN_YR_SUB){
      trim_yr--;
      trim_yr = constrain(trim_yr, -90, 90);
      EEPROM.write(addr_trim_yr, trim_yr + 90);
   }
   if(btn==BTN_YL_SUB) {
      trim_yl--;
      trim_yl = constrain(trim_yl, -90, 90);
      EEPROM.write(addr_trim_yl, trim_yl + 90);
    }
    servoAttach();
    homes(100);
    servoDetach();
}

/**
 * Set time unit, 200 fast! 1000 slow. 495 default
 */
void Penguin::setTimeUnit(int t) {
    Penguin::t = t;
}

/**
 * Get time unit
 */
int Penguin::getTimeUnit() {
    return Penguin::t;
}

/*
 Walking control realization:
*/
bool Penguin::walk(int steps, int dir, int T = -1)
{
    
    if (T == -1) T = t * 3;

    int move1[] = {90, 90 + 35, 90 + 15, 90 + 15};
    int move2[] = {90 + 25, 90 + 30, 90 + 15, 90 + 15};
    int move3[] = {90 + 20, 90 + 20, 90 - 15, 90 - 15};
    int move4[] = {90 - 35, 90, 90 - 15, 90 - 15};
    int move5[] = {90 - 40, 90 - 30, 90 - 15, 90 - 15};
    int move6[] = {90 - 20, 90 - 20, 90 + 15, 90 + 15};

    int move21[] = {90, 90 + 35, 90 - 15, 90 - 15};
    int move22[] = {90 + 25, 90 + 30, 90 - 15, 90 - 15};
    int move23[] = {90 + 20, 90 + 20, 90 + 15, 90 + 15};
    int move24[] = {90 - 35, 90, 90 + 15, 90 + 15};
    int move25[] = {90 - 40, 90 - 30, 90 + 15, 90 + 15};
    int move26[] = {90 - 20, 90 - 20, 90 - 15, 90 - 15};

    if (dir == 1) //Walking forward
    {
        for (int i = 0; i < steps; i++)
            if (
                moveNServos(T * 0.2, move1) ||
                delays(t / 10) ||
                moveNServos(T * 0.2, move2) ||
                delays(t / 10) ||
                moveNServos(T * 0.2, move3) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move4) ||
                delays(t / 2) ||
                moveNServos(T * 0.2, move5) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move6) ||
                delays(t / 5))
                return true;
    }
    else //Walking backward
    {
        for (int i = 0; i < steps; i++)
            if (
                moveNServos(T * 0.2, move21) ||
                delays(t / 10) ||
                moveNServos(T * 0.2, move22) ||
                delays(t / 10) ||
                moveNServos(T * 0.2, move23) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move24) ||
                delays(t / 2) ||
                moveNServos(T * 0.2, move25) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move26))
                return true;
    }

    return false;
}

/**
 * moveNServos in a sequence of positions
 */
bool Penguin::moveNServos(int time, int newPosition[])
{
    for (int i = 0; i < N_SERVOS; i++)
    {
        increment[i] = ((newPosition[i]) - oldPosition[i]) / (time / INTERVALTIME);
    }
    final_time = millis() + time;
    iteration = 1;
    while (millis() < final_time)
    {
        interval_time = millis() + INTERVALTIME;
        oneTime = 0;
        while (millis() < interval_time)
        {
            if (oneTime < 1)
            {
                for (int i = 0; i < N_SERVOS; i++)
                {
                    servo[i].SetPosition(oldPosition[i] + (iteration * increment[i]));
                }
                iteration++;
                oneTime++;
            }
            if (serial_flag)
                return true;
        }
    }

    for (int i = 0; i < N_SERVOS; i++)
    {
        oldPosition[i] = newPosition[i];
    }
    return false;
}

/**
 * use the oscillate library to move servos
 */
bool Penguin::oscillate(int A[N_SERVOS], int O[N_SERVOS], int T, double phase_diff[N_SERVOS])
{
    for (int i = 0; i < 4; i++)
    {
        servo[i].SetO(O[i]);
        servo[i].SetA(A[i]);
        servo[i].SetT(T);
        servo[i].SetPh(phase_diff[i]);
    }
    double ref = millis();
    for (double x = ref; x < T + ref; x = millis())
    {
        for (int i = 0; i < 4; i++)
        {
            servo[i].refresh();
        }
        if (serial_flag)
        {
            return true;
        }
        //if (irValue)
        //    return true;
    }
    return false;
}


/*
 * note:
 * all the methods that could be interrupted (by the bluetooth command) return false;
 * 
 * a chain of actions that could be interrupted is placed in expressions like the following. So if there isn't interruption
 * the result will complete all the chain evaluating false each block.
 * if interruption happens, a called method return true and stop the evaluation of expression returning true;
 * 
 * 
 *     bool Penguin:actionNew() {
 *     if (
                moveNServos(T * 0.2, move1) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move2) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move3) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move4) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move5) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move6) ||
                delays(t / 5) )
                 returm true;
          return false;
       }          
 * 
 * interruption happens with serial_flag true. It's triggered by a timer (MsTimer2) that is called very often
 * 
 */


/**
 * setting the 90-degree position of the steering gear to make the penguin stand on its feet
 * (there are two ways of doing this, both use oscillator)
 */
bool Penguin::home()
{
    int move1[] = {90, 90, 90, 90};
    if (moveNServos(t, move1) || delays(t)) return true;
    return false;
}

/*
    Setting the 90-degree position of the steering gear to make the penguin stand on its feet
    (there are two ways of doing this, both use oscillator)
*/
void Penguin::homes(int T = -1)
{
    if (T==-1) T = t;
    servo[0].SetPosition(90);
    servo[1].SetPosition(90);
    servo[2].SetPosition(90);
    servo[3].SetPosition(90);
    delay(T);
}



/**
 * Turn right and left 
 */
bool Penguin::turn(int steps, int dir, int T = -1)
{
    if (T==-1) T = t * 4;
  
    int move1[] = {90 - 55, 90 - 20, 90 + 20, 90 + 20};
    int move2[] = {90 - 20, 90 - 20, 90 + 20, 90 - 20};
    int move3[] = {90 + 20, 90 + 55, 90 + 20, 90 - 20};
    int move4[] = {90 + 20, 90 + 20, 90 - 20, 90 + 20};
    int move5[] = {90 - 55, 90 - 20, 90 - 20, 90 + 20};
    int move6[] = {90 - 20, 90 - 20, 90 + 20, 90 - 20};
    int move7[] = {90 + 20, 90 + 55, 90 + 20, 90 - 20};
    int move8[] = {90 + 20, 90 + 20, 90 - 20, 90 + 20};
  
    int move21[] = {90 + 20, 90 + 55, 90 + 20, 90 + 20};
    int move22[] = {90 + 20, 90 + 20, 90 + 20, 90 - 20};
    int move23[] = {90 - 55, 90 - 20, 90 + 20, 90 - 20};
    int move24[] = {90 - 20, 90 - 20, 90 - 20, 90 - 20};
    int move25[] = {90 + 20, 90 + 55, 90 - 20, 90 + 20};
    int move26[] = {90 + 20, 90 + 20, 90 + 20, 90 - 20};
    int move27[] = {90 - 55, 90 - 20, 90 + 20, 90 - 20};
    int move28[] = {90 - 20, 90 - 20, 90 - 20, 90 - 20};

    if (dir == 1)
    {
        for (int i = 0; i < steps; i++)
            if (
                moveNServos(T * 0.2, move1) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move2) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move3) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move4) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move5) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move6) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move7) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move8) ||
                delays(t / 5))
                return true;
    }
    else
    {
        for (int i = 0; i < steps; i++)
            if (
                moveNServos(T * 0.2, move21) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move22) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move23) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move24) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move25) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move26) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move27) ||
                delays(t / 5) ||
                moveNServos(T * 0.2, move28) ||
                delays(t / 5))
                return true;
    }

    return false;
}

/**
 * Moonwalk to the right
 */
bool Penguin::moonWalkRight(int steps, int T = -1)
{
    if(T==-1) T = t*2;
    int A[4] = {25, 25, 0, 0};
    int O[4] = {-15, 15, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180 + 120), DEG2RAD(90), DEG2RAD(90)};

    for (int i = 0; i < steps; i++)
        if (oscillate(A, O, T, phase_diff))
            return true;
    return false;
}

/**
 * Moonwalk to the left
 */
bool Penguin::moonWalkLeft(int steps, int T = -1)
{
    if(T==-1) T = t*2;
    int A[4] = {25, 25, 0, 0};
    int O[4] = {-15, 15, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180 - 120), DEG2RAD(90), DEG2RAD(90)};

    for (int i = 0; i < steps; i++)
        if (oscillate(A, O, T, phase_diff))
            return true;
    return false;
}

/**
 * crusaito
 */
bool Penguin::crusaito(int steps, int T = -1)
{
    if (T==-1) T = t * 2; // 2 decided by me, need to be tested
    int A[4] = {25, 25, 30, 30};
    int O[4] = {-15, 15, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180 + 120), DEG2RAD(90), DEG2RAD(90)};
    for (int i = 0; i < steps; i++)
        if (oscillate(A, O, T, phase_diff))
            return true;
    if (home())
        return true;
    return false;
}

/**
 * swing movement
 */
bool Penguin::swing(int steps, int T = -1)
{
    if (T==-1) T = t;
    int A[4] = {25, 25, 0, 0};
    int O[4] = {-15, 15, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};

    for (int i = 0; i < steps; i++)
        if (oscillate(A, O, T, phase_diff))
            return true;
    return false;
}

/**
 * up and down movement
 */
bool Penguin::upDown(int steps, int T = -1)
{
    if (T==-1) T = t;
    int A[4] = {25, 25, 0, 0};
    int O[4] = {-15, 15, 0, 0};
    double phase_diff[4] = {DEG2RAD(180), DEG2RAD(0), DEG2RAD(270), DEG2RAD(270)};
    for (int i = 0; i < steps; i++)
        if (oscillate(A, O, T, phase_diff))
            return true;
    if (home())
        return true;
    return false;
}

/**
 * flapping movement
 */
bool Penguin::flapping(int steps, int T = -1)
{
    if (T==-1) T = t;
    int A[4] = {15, 15, 8, 8};
    int O[4] = {-A[0], A[1], 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90), DEG2RAD(90)};

    for (int i = 0; i < steps; i++)
        if (oscillate(A, O, T, phase_diff))
            return true;
    return false;
}

/*
* run movement
*/
bool Penguin::run(int steps, int T = -1)
{
    if (T==-1) T = t;  // t decided by me need to be tested
    int A[4] = {10, 10, 10, 10};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};

    for (int i = 0; i < steps; i++)
        if (oscillate(A, O, T, phase_diff))
            return true;
    return false;
}

/**
 * backyard movement
 */
bool Penguin::backyard(int steps, int T = -1)
{
    if (T==-1) T = t * 4;
    int A[4] = {15, 15, 30, 30};
    int O[4] = {0, 0, 0, 0};
    double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(-90), DEG2RAD(-90)};

    for (int i = 0; i < steps; i++)
        if (oscillate(A, O, T, phase_diff))
            return true;
    return false;
}

/**
 * going up movement
 */
bool Penguin::goingUp(int T = -1)
{
    if (T==-1) T = t;
    int move1[] = {50, 130, 90, 90};
    if (moveNServos(T * HIGH_RATE, move1) ||
        delays(T / 2) ||
        home())
        return true;
    return false;
}

/**
 * going down movement
 */
bool Penguin::drunk(int T = -1)
{
    if (T==-1) T = t;
    int move1[] = {70, 70, 90, 90};
    int move2[] = {110, 110, 90, 90};
    int move3[] = {70, 70, 90, 90};
    int move4[] = {110, 110, 90, 90};
    if (moveNServos(T * MID_RATE, move1) ||
        moveNServos(T * MID_RATE, move2) ||
        moveNServos(T * MID_RATE, move3) ||
        moveNServos(T * MID_RATE, move4) ||
        home())
        return true;
    return false;
}

/**
 * no gravity movement
 */
bool Penguin::noGravity(int T)
{
    if (T==-1) T = t;
    int move1[] = {120, 140, 90, 90};
    int move2[] = {120, 30, 90, 90};
    int move3[] = {120, 120, 90, 90};
    int move4[] = {120, 30, 120, 120};
    int move5[] = {120, 30, 60, 60};
    if (moveNServos(T * MID_RATE, move1) ||
        delays(T) ||
        moveNServos(T * MID_RATE, move2) ||
        moveNServos(T * MID_RATE, move3) ||
        moveNServos(T * MID_RATE, move2) ||
        delays(T) ||
        moveNServos(T * LOW_RATE, move4) ||
        delays(T) ||
        moveNServos(T * LOW_RATE, move5) ||
        delays(T) ||
        moveNServos(T * LOW_RATE, move4) ||
        delays(T) ||
        home())
        return true;
    return false;
}

/**
 * kick left movement
 */
bool Penguin::kickLeft(int T = -1)
{
   if(T==-1) T=t;
    int move1[] = {120, 140, 90, 90};
    int move2[] = {120, 90, 90, 90};
    int move3[] = {120, 120, 90, 90};
    int move4[] = {120, 90, 120, 120};
    int move5[] = {120, 120, 60, 60};
    if (moveNServos(T * MID_RATE, move1) ||
        delays(T) ||
        moveNServos(T * MID_RATE, move2) ||
        delays(T / 4) ||
        moveNServos(T * MID_RATE, move3) ||
        delays(T / 4) ||
        moveNServos(T * LOW_RATE, move4) ||
        delays(T / 4) ||
        moveNServos(T * LOW_RATE, move5) ||
        delays(T / 4) ||
        home())
        return true;
    return false;
}

/**
 * kick right movement
 */
bool Penguin::kickRight(int T=-1)
{
    if(T==-1) T=t;
    int move1[] = {40, 60, 90, 90};
    int move2[] = {90, 60, 90, 90};
    int move3[] = {60, 60, 90, 90};
    int move4[] = {90, 60, 120, 120};
    int move5[] = {60, 60, 60, 60};
    if (moveNServos(T * MID_RATE, move1) ||
        delays(T) ||
        moveNServos(T * MID_RATE, move2) ||
        delays(T / 4) ||
        moveNServos(T * MID_RATE, move3) ||
        delays(T / 4) ||
        moveNServos(T * LOW_RATE, move4) ||
        delays(T / 4) ||
        moveNServos(T * LOW_RATE, move5) ||
        delays(T / 4) ||
        home())
        return true;
    return false;
}

/**
 * leg raise
 */
bool Penguin::legRaise(int dir, int T=-1)
{
    if(T==-1) T = t;
    if (dir)
    {
        int move1[] = {70, 70, 60, 60};
        if (moveNServos(T * MID_RATE, move1) || delays(T))
            return true;
    }
    else
    {
        int move1[] = {110, 110, 120, 120};
        if (moveNServos(T * MID_RATE, move1) || delays(T))
            return true;
    }
    if (home())
        return true;
    return false;
}

/**
 * leg raise variation 1
 */
bool Penguin::legRaise1(int T, int dir)
{
    if(T==-1) T = t;
    if (dir)
    {
        int move1[] = {50, 60, 90, 90};
        int move2[] = {60, 60, 120, 90};
        int move3[] = {60, 60, 60, 90};
        if (moveNServos(T * MID_RATE, move1) ||
            delays(T) ||
            moveNServos(T * LOW_RATE, move2) ||
            delays(T / 4) ||
            moveNServos(T * LOW_RATE, move3) ||
            delays(T / 4) ||
            moveNServos(T * LOW_RATE, move2) ||
            delays(T / 4) ||
            moveNServos(T * LOW_RATE, move3) ||
            delays(T / 4))
            return true;
    }
    else
    {
        int move1[] = {120, 130, 90, 90};
        int move2[] = {120, 120, 90, 60};
        int move3[] = {120, 120, 90, 120};
        if (moveNServos(T, move1) ||
            delays(T) ||
            moveNServos(T * MID_RATE, move2) ||
            delays(T / 4) ||
            moveNServos(T * MID_RATE, move3) ||
            delays(T / 4) ||
            moveNServos(T * MID_RATE, move2) ||
            delays(T / 4) ||
            moveNServos(T * MID_RATE, move3) ||
            delays(T / 4))
            return true;
    }
    if (home())
        return true;
    return false;
}

/**
 * leg raise variation 2
 */
bool Penguin::legRaise2(int steps, int dir, int T=-1)
{
    if(T==-1) T = t;
    if (dir)
    {
        int move1[] = {20, 60, 90, 90};
        int move2[] = {20, 90, 120, 90};
        for (int i = 0; i < steps; i++)
        {
            if (moveNServos(T * 0.7, move1) ||
                delays(T / 4) ||
                moveNServos(T * 0.7, move2) ||
                delays(T / 4))
                return true;
        }
    }
    else
    {
        int move1[] = {120, 160, 90, 90};
        int move2[] = {90, 160, 90, 60};
        for (int i = 0; i < steps; i++)
        {
            if (moveNServos(T * 0.7, move1) ||
                delays(T / 4) ||
                moveNServos(T * 0.7, move2) ||
                delays(T / 4))
                return true;
        }
    }
    if (home())
        return true;
    return false;
}

/**
 * leg raise variation 3
 */
bool Penguin::legRaise3(int steps, int dir, int T=-1)
{
    if(T==-1) T = t;
    if (dir)
    {
        int move1[] = {20, 60, 90, 90};
        int move2[] = {20, 90, 90, 90};
        for (int i = 0; i < steps; i++)
        {
            if (moveNServos(T * 0.5, move1) ||
                delays(T / 4) ||
                moveNServos(T * 0.5, move2) ||
                delays(T / 4))
                return true;
        }
    }
    else
    {
        int move1[] = {120, 160, 90, 90};
        int move2[] = {90, 160, 90, 90};
        for (int i = 0; i < steps; i++)
        {
            if (moveNServos(T * 0.5, move1) ||
                delays(T / 4) ||
                moveNServos(T * 0.5, move2) ||
                delays(T / 4))
                return true;
        }
    }
    if (home())
        return true;
    return false;
}

/**
 * leg raise variation 4
 */
bool Penguin::legRaise4(int dir, int T = -1)
{
    if(T==-1) T = t;
    if (dir)
    {
        int move1[] = {20, 60, 90, 90};
        int move2[] = {20, 90, 90, 90};

        if (moveNServos(T * MID_RATE, move1) ||
            delays(T / 4) ||
            moveNServos(T * MID_RATE, move2) ||
            delays(T / 4))
            return true;
    }
    else
    {
        int move1[] = {120, 160, 90, 90};
        int move2[] = {90, 160, 90, 90};
        if (moveNServos(T * MID_RATE, move1) ||
            delays(T / 4) ||
            moveNServos(T * MID_RATE, move2) ||
            delays(T / 4))
            return true;
    }
    if (home())
        return true;
    return false;
}

/**
 * sit down movement
 */
bool Penguin::sitdown()
{
    int move1[] = {150, 90, 90, 90};
    int move2[] = {150, 30, 90, 90};
    if (moveNServos(t * ULTRA_LOW_RATE, move1) ||
        delays(t / 2) ||
        moveNServos(t * ULTRA_LOW_RATE, move2) ||
        delays(t / 2) ||
        home())
        return true;
    return false;
}

/**
 * lateral fuerte movement
 */
bool Penguin::lateral_fuerte(boolean dir, int T=-1)
{
   if(T==-1) T= t / 2;
    if (dir)
    {
        int move1[] = {CENTRE - 2 * AMPLITUDE, CENTRE - AMPLITUDE, CENTRE, CENTRE};
        int move2[] = {CENTRE + AMPLITUDE, CENTRE - AMPLITUDE, CENTRE, CENTRE};
        int move3[] = {CENTRE - 2 * AMPLITUDE, CENTRE - AMPLITUDE, CENTRE, CENTRE};
        if (moveNServos(T * LOW_RATE, move1) || delays(T * 2) ||
            moveNServos(T * ULTRA_HIGH_RATE, move2) || delays(T / 2) ||
            moveNServos(T * ULTRA_HIGH_RATE, move3) || delays(T))
            return true;
    }
    else
    {
        int move1[] = {CENTRE + AMPLITUDE, CENTRE + 2 * AMPLITUDE, CENTRE, CENTRE};
        int move2[] = {CENTRE + AMPLITUDE, CENTRE - AMPLITUDE, CENTRE, CENTRE};
        int move3[] = {CENTRE + AMPLITUDE, CENTRE + 2 * AMPLITUDE, CENTRE, CENTRE};
        if (moveNServos(T * LOW_RATE, move1) || delays(T * 2) ||
            moveNServos(T * ULTRA_HIGH_RATE, move2) || delays(T / 2) ||
            moveNServos(T * ULTRA_HIGH_RATE, move3) || delays(T))
            return true;
    }
    if (home())
        return true;
    return false;
}















/**
 * get distance with ultrasounds
 */
int Penguin::getDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    // limit to 32760 to avoid negative values
    int dist = (int)pulseIn(ECHO_PIN, HIGH, 32760) / 58;
    return dist > 0 && dist < 250 ? dist : 250;
}

/**
 * get infra red left
 */
int Penguin::irLeft() {
    st188Val_L = analogRead(ST188_L_PIN);
    return st188Val_L;

}

/**
 * get infra red right
 */
int Penguin::irRight() {
    st188Val_R = analogRead(ST188_R_PIN);
    return st188Val_R;
}

/** 
 * set infra red threshold
 */
void Penguin::setThresholdIr(int v) {
    Penguin::THRESHOLD_IR = v;
}

/**
 * get infra red threshold
 */
int Penguin::getThresholdIr() {
    return Penguin::THRESHOLD_IR;
}


void Penguin::indicatorOff() {
    digitalWrite(INDICATOR_LED_PIN, LOW);
}

void Penguin::indicatorOn() {
    digitalWrite(INDICATOR_LED_PIN, HIGH);
}

/*
read ir data and perform mean... never used?
*/
/*
void Penguin::st188Adjust(int dis)
{
    if (millis() - infraredMeasureTime > 1000 && dis > 20 && dis < 200 && analogRead(ST188_L_PIN) < 300 && analogRead(ST188_R_PIN) < 300)
    {
        unsigned long st188RightData = 0;
        unsigned long st188LeftData = 0;
        for (int n = 0; n < 10; n++)
        {
            st188LeftData += analogRead(ST188_L_PIN);
            st188RightData += analogRead(ST188_R_PIN);
        }
        ST188LeftDataMin = st188LeftData / 10;
        ST188RightDataMin = st188RightData / 10;
        ST188Threshold = ST188LeftDataMin - ST188RightDataMin;
        infraredMeasureTime = millis();
    }
}
*/

/**
 * read voltage level for the battery
 */
double Penguin::getBatteryLevel() {
  return analogRead(VOLTAGE_MEASURE_PIN) * 4.97 / 1023;
}

/**
 * Voltage detection implementation function:
 * Acquisition of battery voltage, preset abnormal voltage threshold, control LED flashing to remind users of charging
 */
void Penguin::Test_voltageMeasure(void) //Realization of Voltage Detection
{
   
    static boolean voltageMeasure_flag = true;
    static boolean is_flag = true;
    if (millis() - voltageMeasureTime > 10000)
    {
        double volMeasure = getBatteryLevel();
        //Serial.print("Battery voltage: ");
        //Serial.println(volMeasure);

        //if (volMeasure < 3.70 || volMeasure >= 4.97)//Detection of power supply voltage below or above the set value is regarded as an abnormal phenomenon
        if (volMeasure >= 3.70 )//Detection of power supply voltage below or above the set value is regarded as an abnormal phenomenon
        {
            // voltage is ok, no flash
            voltageMeasure_flag = true;
        }
        else
        {
            // need recharge, flash
            voltageMeasure_flag = false;
        }
        voltageMeasureTime = millis();
    }

    if (voltageMeasure_flag)
    {
        indicatorOff();
    }
    else
    {
        if (is_flag)
        {
            is_flag = false;
            indicatorOn();
        }
        else
        {
            is_flag = true;
            indicatorOff();
        }
    }
}
