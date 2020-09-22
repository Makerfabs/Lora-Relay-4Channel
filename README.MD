 Lora-Relay-4Channel
 ==
Introduce
-
4-Channel Lora Relay-10A based on the Arduino, users can program it with Arduino IDE, which is very easy especially suit for the none-programmers. There are also guide for users to learn how to create the first IoT project with this board, with which the starters can learn the hardware and programming skill quickly. <br>
[Makerfabs Wiki](https://makerfabs.com/wiki/index.php?title=Main_Page) <br>
![](https://github.com/Makerfabs/Lora-Relay-4Channel/blob/master/md_pic/4-Channel%20Lora%20Relay-10A_1.jpg)

Feature
-
* ATMEL Atmega328P: High Performance, Low Power Atmel®AVR® 8-Bit Microcontroller
* Speed Grade:20Mhz
* Flash:32Kbytes
* RAM: 2KBytes
* EEPROM: 1Kbytes
* Relay type: General Purpose 
* Rated current of relay contact: 10A
* Coil type of relay: Non Latching
* Coil voltage of relay: 5V
* Switching voltage of relay: (277VAC , 28VDC) Max
* DC12V or 5V input

Usage
-
 ### Hardware connection
The USB TO UART TOOL is needed to connect the module and PC. <br>
The pins connections is: 
|4-Channel Lora Relay-10A |USB-to-UART Tool |
|---|---|
|3V3 |3V3 |
|GND |GND |
|RX |TXD |
|TX |RXD |

![](https://github.com/Makerfabs/Lora-Relay-4Channel/blob/master/md_pic/4-Channel%20Lora%20Relay-10A_3.jpg)

 ### Software setup
Open the Arduino, select the tools menu and set the parameter, as the figure:<br>
![](https://github.com/Makerfabs/Lora-Relay-4Channel/blob/master/md_pic/4-Channel%20Lora%20Relay-10A_4.png)

 ### Lora Transmitter
Prepare a Lora receiver and put it aside.<br>
Open the code "LoraTransmitter/LoraTransmitter.ino" and modify the frequency macro according to your board is 433Mhz or 868Mhz or 915Mhz. <br>
```
#define RF95_FREQ 868.0  
```
Verify and upload it. <br>
![](https://github.com/Makerfabs/Lora-Relay-4Channel/blob/master/md_pic/4-Channel%20Lora%20Relay-10A_8.png)  <br>
Open the “Tools—Serial Monitor” and the information of Lora Transmitter will be shown in the monitor, as the picture. 
![](https://github.com/Makerfabs/Lora-Relay-4Channel/blob/master/md_pic/4-Channel%20Lora%20Relay-10A_5.png) 

 ### Lora receiver
Open the "LoraReceiver/LoraReceiver.ino" and modify the frequency macro according to your board is 433Mhz or 868Mhz or 915Mhz. 
```
#define RF95_FREQ 868.0 
```
Verify and upload it. After finish, open the “Tools—Serial Monitor”, If have a transmitter next to it, the receive will receive data sent by the Transmitter and the monitor will show the received information in the window, as the picture.
![](https://github.com/Makerfabs/Lora-Relay-4Channel/blob/master/md_pic/4-Channel%20Lora%20Relay-10A_6.png)  <br>
If the receiver has received it, the relay will be turned on or turned off. Light on when COM connect to NO, light off when COM connect to NC and Normally light off.<br>
![](https://github.com/Makerfabs/Lora-Relay-4Channel/blob/master/md_pic/4-Channel%20Lora%20Relay-10A_7.gif) <br>
Control relay pins: <br>
|Atmega328P	|Relay |
|---|---|
|D4	|K1 |
|D3	|K2 |
|A3	|K3 |
|A2	|K4 |


The following table lists all of the relay's pins and their functionality.<br>
|Pin	|Description | 
|---|---|
|NC	Normally |Closed | 
|NO	Normally |Opened | 
|COM	|Switch Common | 





