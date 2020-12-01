#include <WiFi.h>
#include <WebServer.h> 
#include <SPI.h>
#include <Wire.h>
#include <RadioLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//SSD1306
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16


//esp32
#define DIO0 35
#define DIO1 39

#define LORA_RST 2
#define LORA_CS 25

#define SPI_MOSI 13
#define SPI_MISO 12
#define SPI_SCK 14

#define FREQUENCY 434.0
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9
#define CODING_RATE 7
#define OUTPUT_POWER 10
#define PREAMBLE_LEN 8
#define GAIN 0

SX1278 radio = new Module(LORA_CS, DIO0, LORA_RST, DIO1, SPI, SPISettings());

const char *ssid = "Makerfabs";
const char *password = "20160704";

WebServer server(80); 
String form =
" <!DOCTYPE html><html><head>\
 <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <meta http-equiv='refresh' content='600' />\
<title>LED Button</title>\
<style>\
        .button1 {\
            -webkit-transition-duration: 0.4s;\
            transition-duration: 0.4s;\
            padding: 16px 32px;\
            text-align: center;\
            background-color: white;\
            color: black;\
            border: 2px solid #4CAF50;\
            border-radius:5px;\
        }\
    </style>\
</head>\
<body>\
<img src='https://makerfabs.com/image/cache/logo11-190x63.png' />\
      <h1>Lora Relay</h1>\
<script>\
function send1(){\
  send('/1/');\
  document.getElementById('btn1').style.color = 'green';\ 
  document.getElementById('btn2').style.color = 'white';\
}\
function send2(){\
  send('/2/');\
  document.getElementById('btn1').style.color = 'white';\ 
  document.getElementById('btn2').style.color = 'green';\
}\
function send3(){\
  send('/3/');\
  document.getElementById('btn3').style.color = 'green';\ 
  document.getElementById('btn4').style.color = 'white';\
}\
function send4(){\
  send('/4/');\
  document.getElementById('btn3').style.color = 'white';\ 
  document.getElementById('btn4').style.color = 'green';\
}\
function send5(){\
  send('/5/');\
  document.getElementById('btn5').style.color = 'green';\
  document.getElementById('btn6').style.color = 'white';\
}\
function send6(){\
  send('/6/');\
  document.getElementById('btn5').style.color = 'white';\ 
  document.getElementById('btn6').style.color = 'green';\
}\
function send7(){\
  send('/7/');\
  document.getElementById('btn7').style.color = 'green';\ 
  document.getElementById('btn8').style.color = 'white';\
}\
function send8(){\
  send('/8/');\
  document.getElementById('btn7').style.color = 'white'; \
  document.getElementById('btn8').style.color = 'green';\
}\
function send(url){\
  var xhr = new XMLHttpRequest();\
  xhr.open('GET', url, true);\
  xhr.send();\
}\
</script>\
<br>\
<div class='button1'>\
<button id='btn1' onClick=send1()>R1 ON </button><br>\
<button id='btn2' onClick=send2()>R1 OFF</button><br>\
<button id='btn3' onClick=send3()>R2 ON </button><br>\
<button id='btn4' onClick=send4()>R2 OFF</button><br>\
<button id='btn5' onClick=send5()>R3 ON </button><br>\
<button id='btn6' onClick=send6()>R3 OFF</button><br>\
<button id='btn7' onClick=send7()>R4 ON </button><br>\
<button id='btn8' onClick=send8()>R4 OFF</button><br>\
</div>\
</body></html>\
";

void handleRoot(){
  Serial.println("Access");
  server.send(200, "text/html", form);
}


void handleP1() 
{
  Serial.println("1");
  radio.transmit("R1ON!");
  Serial.print(radio.getDataRate());
  Serial.println(F(" bps"));
}
void handleP2() 
{
  Serial.println("2");
  radio.transmit("R1OFF!");
  Serial.print(radio.getDataRate());
  Serial.println(F(" bps"));
}
void handleP3() 
{
  Serial.println("3");
  radio.transmit("R2ON!");
  Serial.print(radio.getDataRate());
  Serial.println(F(" bps"));
}
void handleP4() 
{
  Serial.println("4");
  radio.transmit("R2OFF!");
  Serial.print(radio.getDataRate());
  Serial.println(F(" bps"));
}
void handleP5() 
{
  Serial.println("5");
  radio.transmit("R3ON!");
  Serial.print(radio.getDataRate());
  Serial.println(F(" bps"));
}
void handleP6() 
{
  Serial.println("6");
  radio.transmit("R3OFF!");
  Serial.print(radio.getDataRate());
  Serial.println(F(" bps"));
}
void handleP7() 
{
  Serial.println("7");
  radio.transmit("R4ON!");
  Serial.print(radio.getDataRate());
  Serial.println(F(" bps"));
}
void handleP8() 
{
  Serial.println("8");
  radio.transmit("R4OFF!");
  Serial.print(radio.getDataRate());
  Serial.println(F(" bps"));
}

void setup()
{
  Serial.begin(115200);
  Serial.println();

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    //SPI.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(200); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

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

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0); // Start at top-left corner
  display.print(WiFi.localIP());
  display.display();
  
  server.on("/", handleRoot); 
  server.on("/1/", handleP1);
  server.on("/2/", handleP2);
  server.on("/3/", handleP3);
  server.on("/4/", handleP4);
  server.on("/5/", handleP5);
  server.on("/6/", handleP6);
  server.on("/7/", handleP7);
  server.on("/8/", handleP8); 


  server.begin(); 
  Serial.println("Web server started");

}

void loop()
{
  server.handleClient(); 
  
}
