

#include <Arduino.h>

int L1G = A0, L1R = A1, L2G = A2, L2O = A3, L2R = A4; //defining leds
int switches[]{8, 12, 0, 1, 5, 3, 4, 2, 6, 7}; //defining switches and butons
int sensors[]{A5, A4, A3};    //defining sensor pins, from sensor 1 to 3

static int timeSeparate[3]; //array for time

void TimeSeparate(long t)   //This function can sepparate time in ms to time in m:s:ms and prints it
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

class Data  //
{
  private:
    void TimeSeparate(long t)   //This function can sepparate time in ms to time in m:s:ms and prints it
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
    long pausetime = 0;

    bool stateCounter1 = false; //is counter 1 running?
    bool stateCounter2 = false; //is counter 2 running?
    bool statePause = false;    //is the timer paused?
    

  public:

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
    void pauseTimer() //when the timer gets paused, save the time until
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

    void checkLedState()    //checks the state of the counter counter and lights up the right led
    {
        if (statePause == true) //orange led when the timer is paused
        {
            digitalWrite(L2G, LOW);
            digitalWrite(L2O, HIGH);
            digitalWrite(L2R, LOW);
        }
        else if (stateCounter1 == true || stateCounter2 == true)    //green led when the timer is running
        {
            digitalWrite(L2G, HIGH);
            digitalWrite(L2O, LOW);
            digitalWrite(L2R, LOW);
        }
        else    //red led when the timer is not running
        {
            digitalWrite(L2G, LOW);
            digitalWrite(L2O, LOW);
            digitalWrite(L2R, HIGH);
        }
    }

    long getTotalTime() { return totalTime; }   //returns the total time
    long getTime1() //returns the time of counter 1
    {
        if (stateCounter1 == true)  //if the counter is still running, return the time he's running
        {
            return millis() - startTime1;
        }
        return time1;   //is the timer isn't running anymore, return the final time
    }
    long getTime2()
    {
        if (stateCounter2 == true)  //if the counter is still running, return the time he's running
        {
            return millis() - startTime2;
        }
        return time2;   //is the timer isn't running anymore, return the final time
    }
};

class CountDown //counter for the counting down before the start of the timing
{
  private:
    long startCounter;
    long countDownTime = 4500; //maximal time before start of timer
    long time;

  public:
    void editCountDownTime(long t)  //change the time that the counter starts of
    {
        countDownTime = t;
    }

    void startCountdown()   //start with counting down
    {
        startCounter = millis();
    }

    long getTimeCountDown() //returns the time the counter
    {
        time = countDownTime + startCounter - millis();
        return time;
    }

    void displayTimeCountDown() //shows the time of the counter
    {
        Serial.print("time: ");
        Serial.println(getTimeCountDown());
    }
};

class Communication //class used for the communication with the sensors
{
  private:
    int port;

  public:
    Communication(int adress) { port = adress; }    //insert the port the sensor is connected to
    void startCom() {}  //start the communication with the sensor
    void stopCom() {}   //stop the communication with the sensor
    void checkCom() {}  //check the communication with the sensor
    bool getState() //check the state of the sensor
    {
        if(analogRead(port) <= 700){
            return false;
        }
        return true;
    }
};

Data D1;    //timer
Communication R1(sensors[0]);   //sensor 1
Communication R2(sensors[1]);   //sensor 2
Communication R3(sensors[2]);   //sensor 3
CountDown T1;   //countdown timer

void pause()    //ISR function to pause the timer
{ 
    D1.pauseTimer();
    D1.checkLedState();
    while (digitalRead(switches[8]) == HIGH)    //wait to the counter is restarted
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
    attachInterrupt(digitalPinToInterrupt(switches[7]), pause, LOW);    //attach ISR on pin switches[7]
    pinMode(L1G, OUTPUT);
    pinMode(L1R, OUTPUT);
    pinMode(L2G, OUTPUT);
    pinMode(L2O, OUTPUT);
    pinMode(L2R, OUTPUT);

    Serial.begin(9600);
}

void loop()
{
    if (digitalRead(switches[2]) == LOW)    //check the communication to the sensors
    {
        D1.checkLedState();
        R1.checkCom();
        R2.checkCom();
        R3.checkCom();
    }

    if (digitalRead(switches[3]) == LOW)    
    {
        T1.startCountdown();    //start counting down
        R1.startCom();  //start the communication with the fisrt sensor

        while (T1.getTimeCountDown() > 0 && R1.getState() == HIGH)  //if the counter is stopped or the first sensor detected a activity
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
