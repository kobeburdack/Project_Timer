#include <Arduino.h>


class Data
{
private:

int penaltyTimeLenght = 6000;
int startTime1;
int startTime2;
int time2;
int time2;
int pauseTijd;
int totalTime;
bool stateCounter1 = false;
bool stateCounter2 = false;
bool statePause = false;


public:

String name;
int number = 0;

startCounter1(){
  startTime1 = millis();
  stateCounter1 = true;
}
stopCounter1(){
  time2 = millis() - startTime1;
  stateCounter1 = false;
}

startCounter2(){
  startTime1 = millis();
  stateCounter2 = true;
}
stopCounter2(){
  time2 = millis() - startTime1;
  stateCounter2 = false;
}
pauzeTimer(){
  if(startTime1 == true){
    int time = millis() - startTime1;
    
  }
}

}




void setup() {
  Serial.begin(9600);
}



void loop() {
  // put your main code here, to run repeatedly:
}