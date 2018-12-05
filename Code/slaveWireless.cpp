// program communication slave

#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
const byte addresses[][6] = {"00001", "00002", "00003", "00004", "00005", "00006"};
int a, b, c;

void setup()
{
    Serial.begin(9600);
    radio.begin;
}
void loop()
{
    receiveCommunicationCheck();
    sendCommunicationCheck();
    startCom();
}
void receiveCommunicationCheck()
{
    radio.startListening();
    radio.available();
    a = radio.read();
}
void sendCommunicationCheck()
{
    radio.openWritingPipe(addresses[3]); // aanpassen met andere slave programma's
    radio.write(a);
    delay(5);
}

bool getState()
{
    if (digitalRead(sensor) == true)
    {
        radio.openWritingPipe(addresses[3]); // aanpassen met andere slave programma's
        radio.write(5);
    }

    void startCom()
    {
        radio.startListening();
        radio.available();
        b = radio.read();
        if (b == 2)
        {
            geState();
        }
    }

    void stopCom()
    {
        radio.startListening();
        radio.available();
        c = radio.read();
        if (c == 3)
        {
            stopArduino();
        }
        else
        {
            Serial.println("error: wrong singal");
        }
    }
