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

void loop() {
    radio.openReadingPipe(1, addresses[0]);
    radio.startListening();
    while(radio.available());
    a = radio.read();
    radio.openWritingPipe(addresses[1]);
    if(a = 1){
    radio.write("1");
    delay(5);
    }
    if(a = 2){

    }
}