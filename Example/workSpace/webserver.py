#webserver.py
import network
import webrepl
import time
from machine import Pin
try:
  import usocket as socket
except:
  import socket

AUTH_OPEN = 0
AUTH_WEP = 1
AUTH_WPA_PSK = 2
AUTH_WPA2_PSK = 3
AUTH_WPA_WPA2_PSK = 4

SSID = "Makerfabs"      #Modify here with SSID
PASSWORD = "20160704"   #Modify here with PWD
led = Pin(5, Pin.OUT)
ip = "ip get wrong"

def web_page(msg):
  html ="""
 <!DOCTYPE html><html><head>
 <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="refresh" content="600" />
<title>LED Button</title>
<style>
        .button1 {
            -webkit-transition-duration: 0.4s;
            transition-duration: 0.4s;
            padding: 16px 32px;
            text-align: center;
            background-color: white;
            color: black;
            border: 2px solid #4CAF50;
            border-radius:5px;
        }
    </style>
</head>
<body>
<img src='https://makerfabs.com/image/cache/logo11-190x63.png' />
      <h1>Lora Relay</h1>
      <h2>Lora Relay status:""" + str(msg) + """</h2>
<script>
function send1(){
  send('/1/');
  document.getElementById("btn1").style.color = "green"; 
  document.getElementById("btn2").style.color = "white";
}
function send2(){
  send('/2/');
  document.getElementById("btn1").style.color = "white"; 
  document.getElementById("btn2").style.color = "green";

}
function send3(){
  send('/3/');
  document.getElementById("btn3").style.color = "green"; 
  document.getElementById("btn4").style.color = "white";
}
function send4(){
  send('/4/');
  document.getElementById("btn3").style.color = "white"; 
  document.getElementById("btn4").style.color = "green";
}
function send5(){
  send('/5/');
  document.getElementById("btn5").style.color = "green"; 
  document.getElementById("btn6").style.color = "white";
}
function send6(){
  send('/6/');
  document.getElementById("btn5").style.color = "white"; 
  document.getElementById("btn6").style.color = "green";
}
function send7(){
  send('/7/');
  document.getElementById("btn7").style.color = "green"; 
  document.getElementById("btn8").style.color = "white";
}
function send8(){
  send('/8/');
  document.getElementById("btn7").style.color = "white"; 
  document.getElementById("btn8").style.color = "green";
}
function send(url){
  var xhr = new XMLHttpRequest();
  xhr.open('GET', url, true);
  xhr.send();
}
</script>
<br>
<div class='button1'>
<button id='btn1' onClick=send1()>R1 ON </button><br>
<button id='btn2' onClick=send2()>R1 OFF</button><br>
<button id='btn3' onClick=send3()>R2 ON </button><br>
<button id='btn4' onClick=send4()>R2 OFF</button><br>
<button id='btn5' onClick=send5()>R3 ON </button><br>
<button id='btn6' onClick=send6()>R3 OFF</button><br>

<button id='btn7' onClick=send7()>R4 ON </button><br>
<button id='btn8' onClick=send8()>R4 OFF</button><br>
</div>
</body></html>
  """
  return html

def do_connect(ssid,psw):
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    s = wlan.config("mac")
    mac = ('%02x:%02x:%02x:%02x:%02x:%02x').upper() %(s[0],s[1],s[2],s[3],s[4],s[5])
    print("Local MAC:"+mac) #get mac 
    wlan.connect(ssid, psw)
    if not wlan.isconnected():
        print('connecting to network...' + ssid)
        wlan.connect(ssid, psw)

    start = time.ticks_ms() # get millisecond counter
    while not wlan.isconnected():
        time.sleep(1) # sleep for 1 second
        if time.ticks_ms()-start > 20000:
            print("connect timeout!")
            break

    if wlan.isconnected():
        print('network config:', wlan.ifconfig())
        global ip
        ip = str(wlan.ifconfig())
    return wlan

def connect():
 do_connect(SSID,PASSWORD)
 global ip
 return ip



