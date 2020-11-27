// Arduino9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Arduino9x_TX
 
#include <SPI.h>
#include <RH_RF95.h>
 
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
 
// Change to 868.0 or other frequency, must match RX's freq!
#define RF95_FREQ 433.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
// Blinky on receipt
#define LED 5


int count=0;
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
   
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  //while (!Serial);
  Serial.begin(115200);
  delay(100);
 
  Serial.println("Arduino LoRa RX Test!");
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}
 
void loop()
{   
  //digitalWrite(LED, LOW);
  if (rf95.available())
  {
    
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      
      digitalWrite(LED, LOW);
      RH_RF95::printBuffer("Received: ", buf, len);
      String msg = String((char *)buf);
      Serial.println(msg);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      Serial.println();
      if (msg.indexOf("RELAY") == 0)
      {
        String message ;
        if (msg.indexOf("R1ON") != -1)
        {
          digitalWrite(4, HIGH);    //RELAY 1
          RS1 = "1";

        }

        else if (msg.indexOf("R1OFF") != -1)
        {
            digitalWrite(4, LOW);
            RS1 = "0";

        }
        else if (msg.indexOf("R2ON") != -1)
        {
            digitalWrite(3, HIGH);    //RELAY 2
            RS2 = "1";

        }

        else if (msg.indexOf("R2OFF") != -1)
        {
            digitalWrite(3, LOW);
            RS2 = "0";

        }
        else if (msg.indexOf("R3ON") != -1)
        {
            digitalWrite(A3, HIGH);   //RELAY 3
            RS3 = "1";

        }

        else if (msg.indexOf("R3OFF") != -1)
        {
            digitalWrite(A3, LOW);
            RS3 = "0";
        }
        else if (msg.indexOf("R4ON") != -1)
        {
            digitalWrite(A2, HIGH);   //RELAY 4
            RS4 = "1";
        }

        else if (msg.indexOf("R4OFF") != -1)
        {
            digitalWrite(A2, LOW);
            RS4 = "0";
        }

        Serial.println(message);
 
          message = RS1+RS2+RS3+RS4;
          Serial.println(message);          
          uint8_t radioPacket[message.length() + 1];
          message.toCharArray(radioPacket, message.length() + 1);
          radioPacket[message.length() + 1] = '\0';
          Serial.println("Sending...");
          delay(10);
          rf95.send((uint8_t *)radioPacket, message.length() + 1);
      
        /*uint8_t data[] = {R1S,R2S,R3S,R4S};
        for (int i = 0; i < 4; i++)  Serial.print(data[i]);
        rf95.send(data, sizeof(data));
        rf95.waitPacketSent();*/
        digitalWrite(LED, HIGH);
    }
    delay(1000);
    }   
  }
}
