// include the library
#include <RadioLib.h>

//328p

#define DIO0 2
#define DIO1 6
#define DIO2 7
#define DIO5 8

#define LORA_RST 9
#define LORA_CS 10

#define SPI_MOSI 11
#define SPI_MISO 12
#define SPI_SCK 13

#define FREQUENCY 434.0
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9
#define CODING_RATE 7
#define OUTPUT_POWER 10
#define PREAMBLE_LEN 8
#define GAIN 0

SX1278 radio = new Module(LORA_CS, DIO0, LORA_RST, DIO1);

#define R1 A0
#define R2 A1
#define R3 A2
#define R4 A3


void setup()
{
    pinMode(R1,INPUT_PULLUP);
    pinMode(R2,INPUT_PULLUP);
    pinMode(R3,INPUT_PULLUP);
    pinMode(R4,INPUT_PULLUP);
    
    Serial.begin(115200);

 
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

   
}



void loop()
{           
    int S1 = digitalRead(R1);
    int S2 = digitalRead(R2);
    int S3 = digitalRead(R3);
    int S4 = digitalRead(R4);
    int transtatus = 0;
    String str = "";
    if (S1 == 0)
    {
        str = "RELAY01";
        transtatus = 1;
    }
    else if (S2 == 0)
    {
        str = "RELAY02";
        transtatus = 1;
    }
    else if (S3 == 0)
    {
        str = "RELAY03";
        transtatus = 1;
    }
    else if (S4 == 0)
    {
        str = "RELAY04";
        transtatus = 1;
    }

    if (transtatus)
    {
        Serial.println(str);
        int state = radio.transmit(str);

        if (state == ERR_NONE)
        {
        // the packet was successfully transmitted
            Serial.println(F(" success!"));
        // print measured data rate
            Serial.print(F("[SX1278] Datarate:\t"));
            Serial.print(radio.getDataRate());
            Serial.println(F(" bps"));
        }
        else if (state == ERR_PACKET_TOO_LONG)
        {
        // the supplied packet was longer than 256 bytes
            Serial.println(F("too long!"));
        }
        else if (state == ERR_TX_TIMEOUT)
        {
        // timeout occurred while transmitting packet
            Serial.println(F("timeout!"));
        }
        else
        {
        // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(state);
        }

    // wait for a second before transmitting again
        transtatus = 0;
        delay(200);
    }        
}
