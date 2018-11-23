

#include <Arduino.h>

int L1G = A0;
int L1R = A1;
int L2G = A2;
int L2O = A3;
int L2R = A4;

int switches[]{8, A5, 0, 1, 5, 3, 4, 2, 6, 7}; //defining switches and butons

int C1;
int C2;
int C3;

static int timeSeparate[3];

void TimeSeparate(long t)
{
    timeSeparate[0] = t / 60000;
    timeSeparate[1] = (t % (timeSeparate[0] * 60000)) / 1000;
    timeSeparate[2] = t - (timeSeparate[0] * 60000) - (timeSeparate[1] * 1000);
    if (timeSeparate[0] <= 9)
    {
        Serial.print("0");
    }
    Serial.print(timeSeparate[0]);
    Serial.print(":");
    if (timeSeparate[1] <= 9)
    {
        Serial.print("0");
    }
    Serial.print(timeSeparate[1]);
    Serial.print(":");
    if (timeSeparate[2] <= 9)
    {
        Serial.print("00");
    }
    else if (timeSeparate[2] <= 99)
    {
        Serial.print("0");
    }
    Serial.println(timeSeparate[2]);
}

class Data
{
  private:
    void TimeSeparate(long t)
    {
        timeSeparate[0] = t / 60000;
        timeSeparate[1] = (t % (timeSeparate[0] * 60000)) / 1000;
        timeSeparate[2] = t - (timeSeparate[0] * 60000) - (timeSeparate[1] * 1000);
        if (timeSeparate[0] <= 9)
        {
            Serial.print("0");
        }
        Serial.print(timeSeparate[0]);
        Serial.print(":");
        if (timeSeparate[1] <= 9)
        {
            Serial.print("0");
        }
        Serial.print(timeSeparate[1]);
        Serial.print(":");
        if (timeSeparate[2] <= 9)
        {
            Serial.print("00");
        }
        else if (timeSeparate[2] <= 99)
        {
            Serial.print("0");
        }
        Serial.println(timeSeparate[2]);
    }

    long penaltyTimeLenght = 6000; //spenalty time when paused
    long startTime1 = 0;
    long startTime2 = 0;
    long time1 = 0;
    long time2 = 0;
    long pauseTime = 0;
    long totalTime = 0;
    bool stateCounter1 = false;
    bool stateCounter2 = false;
    bool statePause = false;
    int pausetime = 0;

  public:
    String name;
    int number = 0;

    Data(String na, int nu) //give a name and number to the timer
    {
        name = na;
        number = nu;
    }

    void startCounter1() //set te starttime of counter 1
    {

        startTime1 = millis();
        stateCounter1 = true;
    }
    void stopCounter1() //stop counter 1 and save the data
    {

        if (statePause == false)
        {
            time1 = millis() - startTime1;
            stateCounter1 = false;
            totalTime = time1;
        }
    }

    void startCounter2() //set te starttime of counter 2
    {
        startTime2 = millis();
        stateCounter2 = true;
    }
    void stopCounter2() //stop counter 2 and save the data
    {
        if (statePause == false)
        {
            time2 = millis() - startTime2;
            stateCounter2 = false;
            totalTime = totalTime + time2;
        }
    }
    void pauseTimer() //wwen the timer gets paused, save the time until
    {
        statePause = true;
        if (stateCounter1 == true)
        {
            pauseTime = millis() - startTime1;
        }
        if (stateCounter2 == true)
        {
            pauseTime = millis() - startTime2;
        }
    }
    void restartTimer() //when the timer gets restarted, reset the starttime that's counted up with the pentaltytime
    {
        if (stateCounter1 == true)
        {
            startTime1 = millis() - pauseTime - penaltyTimeLenght;
        }
        if (stateCounter2 == true)
        {
            startTime2 = millis() - pauseTime - penaltyTimeLenght;
        }
        //addPentaltyTime();
        statePause = false;
    }

    void displayTime() //displays the times on the serial monitor
    {
        Serial.print("tijd 1: ");
        TimeSeparate(time1);
        Serial.print("tijd 2: ");
        TimeSeparate(time2);
        Serial.print("totale tijd: ");
        TimeSeparate(totalTime);
    }

    void checkLedState()
    {
        if (statePause == true)
        {
            digitalWrite(L2G, LOW);
            digitalWrite(L2O, HIGH);
            digitalWrite(L2R, LOW);
        }
        else if (stateCounter1 == true || stateCounter2 == true)
        {
            digitalWrite(L2G, HIGH);
            digitalWrite(L2O, LOW);
            digitalWrite(L2R, LOW);
        }
        else
        {
            digitalWrite(L2G, LOW);
            digitalWrite(L2O, LOW);
            digitalWrite(L2R, HIGH);
        }
    }

    long getTotalTime() { return totalTime; }
    long getTime1()
    {
        if (stateCounter1 == true)
        {
            return millis() - startTime1;
        }
        return time1;
    }
    long getTime2()
    {
        if (stateCounter2 == true)
        {
            return millis() - startTime2;
        }
        return time2;
    }
};

class CountDown
{
  private:
    long startCounter;
    long countDownTime = 4500; //maximal time before start of timer
    long time;

  public:
    void editCountDownTime(long t)
    {
        countDownTime = t;
    }

    void startCountdown()
    {
        startCounter = millis();
    }

    long getTimeCountDown()
    {
        time = countDownTime + startCounter - millis();
        return time;
    }

    void displayTimeCountDown()
    {
        Serial.print("time: ");
        Serial.println(getTimeCountDown());
    }
};

class Communication
{
  private:
    int port;

  public:
    Communication(int adress) { port = adress; }
    void startCom() {}
    void stopCom() {}
    void checkCom() {}
    bool getState()
    {
        Serial.println(digitalRead(port));
        return digitalRead(port);
    }
};

Data D1("bob", 1);
Communication R1(5);
Communication R2(3);
Communication R3(4);
CountDown T1;

void pause()
{ //ISR function to pause the timer
    D1.pauseTimer();
    D1.checkLedState();
    while (digitalRead(switches[8]) == HIGH)
    {
    };
    D1.restartTimer();
    D1.checkLedState();
}

void setup()
{
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    for (int i = 0; i <= 9; i++)
    { //set switches on inputs with pullup
        pinMode(switches[i], INPUT_PULLUP);
    }
    attachInterrupt(digitalPinToInterrupt(switches[7]), pause, LOW);
    pinMode(L1G, OUTPUT);
    pinMode(L1R, OUTPUT);
    pinMode(L2G, OUTPUT);
    pinMode(L2O, OUTPUT);
    pinMode(L2R, OUTPUT);

    Serial.begin(9600);
    /*
  T1.startCountdown();
  while(T1.getTimeCountDown() >0){
    TimeSeparate(T1.getTimeCountDown());
    }
  delay(20);
  D1.startCounter1();
  delay(5000);
  D1.stopCounter1();
  D1.startCounter2();
  delay(3000);
  D1.pauseTimer();
  delay(100);
  D1.restartTimer();
  delay(1248);
  D1.stopCounter2();
  D1.displayTime();
  TimeSeparate(D1.getTotalTime());
  D1.checkLedState();*/
}

void loop()
{

    if (digitalRead(switches[2]) == LOW)
    {
        D1.checkLedState();
        R1.checkCom();
        R2.checkCom();
        R3.checkCom();
    }

    if (digitalRead(switches[3]) == LOW)
    {
        T1.startCountdown();
        R1.startCom();

        while (T1.getTimeCountDown() > 0 && R1.getState() == HIGH)
        {
            TimeSeparate(T1.getTimeCountDown());
            D1.checkLedState();
        }

        D1.startCounter1();
        R1.stopCom();
        R2.startCom();
        D1.checkLedState();

        while (R2.getState() == HIGH)
        {
            TimeSeparate(D1.getTime1());
        }
        D1.stopCounter1();
        D1.checkLedState();

        if (digitalRead(switches[1]) == LOW)
        {
            D1.startCounter2();
            R2.stopCom();
            R3.startCom();
            D1.checkLedState();
            while (R3.getState() == HIGH)
            {
                TimeSeparate(D1.getTime2());
            }
            D1.stopCounter2();
            D1.checkLedState();
            R3.stopCom();
        }
        else
        {
            R2.stopCom();
        }
        D1.displayTime();
        delay(5000);
    }
}
