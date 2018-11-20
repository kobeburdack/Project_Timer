

#include <Arduino.h>

int L1G = A0;
int L1R = A1;
int L2G = A2;
int L2O = A3;
int L2R = A4;

int TimeSeparate(long t)
{
  static int array[3];
  array[0] = t / 60000;
  array[1] = (t % (array[0] * 60000)) / 1000;
  array[2] = array[0] % (array[1] * 1000);
  return array;
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

  /* void addPentaltyTime()  //Pentaly time gets added to current timer
  {
    if (stateCounter1 == true)
    {
      startTime1 = startTime1 + penaltyTimeLenght;
    }
    if (stateCounter2 == true)
    {
      startTime2 = startTime2 + penaltyTimeLenght;
    }
  }
*/
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
  long getTime1() { return time1; }
  long getTime2() { return time2; }
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
  while (T1.getTimeCountDown() > 0)
  {
    T1.displayTimeCountDown();
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
  delay(1000);
  D1.stopCounter2();
  D1.displayTime();
  Serial.println(D1.getTotalTime());
  TimeSeparate(getTotalTime());
  Serial.println(TimeSeparate[0]);
  Serial.println(TimeSeparate[1]);
  Serial.println(TimeSeparate[2]);
}

void loop() {}
