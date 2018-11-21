/*
#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
}

*/
/*#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
}*/

#include <Arduino.h>

int L1G = A0;
int L1R = A1;
int L2G = A2;
int L2O = A3;
int L2R = A4;
static int timeSeparate[3];

void TimeSeparate(long t)
{
  timeSeparate[0] = t / 60000;
  timeSeparate[1] = (t % (timeSeparate[0] * 60000)) / 1000;
  timeSeparate[2] = t - (timeSeparate[0] * 60000) - (timeSeparate[1] * 1000);
  Serial.print(timeSeparate[0]);
  Serial.print(":");
  Serial.print(timeSeparate[1]);
  Serial.print(":");
  Serial.println(timeSeparate[2]);
}

class Data
{
private:
  long penaltyTimeLenght = 6000; //spenalty time when paused
  long startTime1;
  long startTime2;
  long time1;
  long time2;
  long pauseTime;
  long totalTime;
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
    Serial.println(time1);
    Serial.print("tijd 2: ");
    Serial.println(time2);
    Serial.print("totale tijd: ");
    Serial.println(totalTime);
  }

  void checkLedState()
  {
    if (stateCounter1 == true || stateCounter2 == true)
    {
      digitalWrite(L2G, HIGH);
      digitalWrite(L2O, LOW);
      digitalWrite(L2R, LOW);
    }
    if (statePause == true)
    {
      digitalWrite(L2G, LOW);
      digitalWrite(L2O, HIGH);
      digitalWrite(L2R, LOW);
    }
    if (stateCounter1 == false || stateCounter2 == false && statePause == false)
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
    if (stateCounter1 == true)
    {
      return millis() - startTime2;
    }
    return time;
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

Data D1("bob", 1);
CountDown T1;

void setup()
{
  Serial.begin(9600);

  T1.startCountdown();

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
  Serial.println(D1.getTotalTime());
  TimeSeparate(D1.getTotalTime());
}

void loop() {}
