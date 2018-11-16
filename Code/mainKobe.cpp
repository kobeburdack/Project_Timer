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

class Data
{
private:
  long penaltyTimeLenght = 6000;
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

  void addPentaltyTime()
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

public:
  String name;
  int number = 0;

  Data(String na, int nu)
  {
    name = na;
    number = nu;
  }

  void startCounter1()
  {

    startTime1 = millis();
    stateCounter1 = true;
  }
  void stopCounter1()
  {

    if (statePause == false)
    {
      time1 = millis() - startTime1;
      stateCounter1 = false;
      totalTime = time1;
    }
  }

  void startCounter2()
  {
    startTime2 = millis();
    stateCounter2 = true;
  }
  void stopCounter2()
  {
    if (statePause == false)
    {
      time2 = millis() - startTime2;
      stateCounter2 = false;
      totalTime = totalTime + time2;
    }
  }
  void pauseTimer()
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
  void restartTimer()
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

  void displayTime()
  {
    Serial.printf("tijd 1: %l", time1);
    Serial.print("tijd 2: ");
    Serial.println(time2);
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

Data D1("bob", 1);

void setup()
{
  delay(20);
  Serial.begin(9600);
  D1.displayTime();
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
}

void loop() {}
