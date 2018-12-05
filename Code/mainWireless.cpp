// program communication transmitter

#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
const byte addresses[][6] = {"00001", "00002", "00003", "00004", "00005", "00006"};

class Communication {
  private:
    int adress;
    bool state = false;

    void sendCommunicationCheck() {
      radio.begin();
      for (int i = 0 ; i < 3 ; i++) {
        radio.openWritingPipe(addresses[i]);
        radio.write("1");
        delay(5);
      }
    };
    void receiveCommunicationCheck() {
      radio.startListening();
      radio.available();
      int a = radio.read();

      if (a == 1) {
        // kobe moet led nog aan zetten!
        Serial.println("signal ok");
        return true;

      }
      else {
        serial.println("communication error");
        return false;

      }
    };

  public:
    bool checkCom() {
      sendCommunicationCheck();
      receiveCommunicationCheck();
    }

    void startCom() {
      radio.begin();
      radio.openWritingPipe(addresses[adress]);
      radio.write("2");
      delay(5);
    };
    void stopCom() {
      radio.begin();
      radio.openWritingPipe(addresses[adress]);
      radio.write("3");
      delay(5);
    };
};
bool getState() {
  radio.startListening();
  if (radio.read() == 5) {
    return true;
  }
  return false;
};

void setup() {
  radio.begin();
  Serial.begin(9600);
}
void loop() {}


