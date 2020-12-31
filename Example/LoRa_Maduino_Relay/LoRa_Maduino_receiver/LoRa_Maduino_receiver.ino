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
#define RELAY1 4
#define RELAY2 3
#define RELAY3 A3
#define RELAY4 A2


bool RS1 = LOW;
bool RS2 = LOW;
bool RS3 = LOW;
bool RS4 = LOW;


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
    digitalWrite(LED,LOW);
    enableInterrupt = false;
    receivedFlag = false;


    // you can read received data as an Arduino String
    String str;
    int state = radio.readData(str);



    if (state == ERR_NONE) {
      // packet was successfully received
      Serial.println(F("[SX1278] Received packet!"));
      Serial.println(str);
      if (str.indexOf("RELAY01") != -1)  
      {
        RS1 = !RS1;
        switch (RS1) {
        case LOW: {digitalWrite(RELAY1,LOW); } break;
        case HIGH: {digitalWrite(RELAY1,HIGH);} break;
        }
      }
      else if (str.indexOf("RELAY02") != -1)  
      {
        RS2 = !RS2;
        switch (RS2) {
        case LOW: {digitalWrite(RELAY2,LOW); } break;
        case HIGH: {digitalWrite(RELAY2,HIGH); } break;
        }
      }
      else if (str.indexOf("RELAY03") != -1)  
      {
        RS3 = !RS3;
        switch (RS3) {
        case LOW: {digitalWrite(RELAY3,LOW);} break;
        case HIGH: {digitalWrite(RELAY3,HIGH); } break;
        }
      }
      else if (str.indexOf("RELAY04") != -1)  
      {
        RS4 = !RS4;
        switch (RS4) {
        case LOW: {digitalWrite(RELAY4,LOW); } break;
        case HIGH: {digitalWrite(RELAY4,HIGH); } break;
        }
      }
      
    }
    digitalWrite(LED,HIGH);
    radio.startReceive();
    enableInterrupt = true;
    }   

}

 
