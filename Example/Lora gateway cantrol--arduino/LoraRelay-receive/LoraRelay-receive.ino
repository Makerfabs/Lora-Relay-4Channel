// Arduino9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Arduino9x_TX
 
#include <RadioLib.h>

const int DIO0 = 2;
const int DIO1 = 6;
const int DIO2 = 7;
const int DIO5 = 8;

const int LORA_RST = 9;
const int LORA_CS = 10;

const int SPI_MOSI = 11;
const int SPI_MISO = 12;
const int SPI_SCK = 13;

#define FREQUENCY 434.0
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9
#define CODING_RATE 7
#define OUTPUT_POWER 10
#define PREAMBLE_LEN 8
#define GAIN 0

SX1278 radio = new Module(LORA_CS, DIO0, LORA_RST, DIO1);

#define LED 5



String RS1="0";
String RS2="0";
String RS3="0";
String RS4="0";


void setup() 
{
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);

  digitalWrite(5, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(3, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A2, LOW);
 
  //while (!Serial);
  Serial.begin(115200);
  delay(100);
 

 
  int state = radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, SX127X_SYNC_WORD, OUTPUT_POWER, PREAMBLE_LEN, GAIN);
    //int state = radio.begin();
    if (state == ERR_NONE)
    {
        Serial.println(F("success!"));
    }
    else
    {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
            ;
    }

    radio.setDio0Action(setFlag);

  // start listening for LoRa packets
  Serial.print(F("[SX1278] Starting to listen ... "));
  state = radio.startReceive();
  if (state == ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

volatile bool receivedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // check if the interrupt is enabled
  if(!enableInterrupt) {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
}
 
void loop()
{   
  if (receivedFlag)
  {
    digitalWrite(LED, LOW);
    enableInterrupt = false;

    // reset flag
    receivedFlag = false;

    // you can read received data as an Arduino String
    String str;
    int state = radio.readData(str);

    // you can also read received data as byte array
    /*
      byte byteArr[8];
      int state = radio.readData(byteArr, 8);
    */

    if (state == ERR_NONE) {
      // packet was successfully received
      Serial.println(F("[SX1278] Received packet!"));
        if (str.indexOf("R1ON") != -1)
        {
          digitalWrite(4, HIGH);    //RELAY 1
          RS1 = "1";

        }

        else if (str.indexOf("R1OFF") != -1)
        {
            digitalWrite(4, LOW);
            RS1 = "0";

        }
        else if (str.indexOf("R2ON") != -1)
        {
            digitalWrite(3, HIGH);    //RELAY 2
            RS2 = "1";

        }

        else if (str.indexOf("R2OFF") != -1)
        {
            digitalWrite(3, LOW);
            RS2 = "0";

        }
        else if (str.indexOf("R3ON") != -1)
        {
            digitalWrite(A3, HIGH);   //RELAY 3
            RS3 = "1";

        }

        else if (str.indexOf("R3OFF") != -1)
        {
            digitalWrite(A3, LOW);
            RS3 = "0";
        }
        else if (str.indexOf("R4ON") != -1)
        {
            digitalWrite(A2, HIGH);   //RELAY 4
            RS4 = "1";
        }

        else if (str.indexOf("R4OFF") != -1)
        {
            digitalWrite(A2, LOW);
            RS4 = "0";
        }

        Serial.println(str);
        String message;
        message = RS1+RS2+RS3+RS4;
        Serial.println(message);          
    }
    digitalWrite(LED, HIGH);
    radio.startReceive();

    // we're ready to receive more packets,
    // enable interrupt service routine
    enableInterrupt = true;
    }   

}

 
