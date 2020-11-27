
from machine import Pin
import time
import config_lora
import random
import webserver
try:
  import usocket as socket
except:
  import socket


#网关角色
MODE_TRANSFER   = 0
MODE_RECEIVER   = 1
MODE_GATE       = 2

#节点类型
NODE_SENSOR     = 0
NODE_RELAY      = 1
 
class Lora_Gate:

    def __init__(self, name, mode, lora, ip):
        self.name = name
        self.lora = lora
        self.mode = mode
        self.sensor_adc = 0
        self.relay_status = "UNKNOWN"
        self.flag = 0
        self.buff = ""
        self.ip = ip

    #模式配置
    def working(self):
        str_ip = self.ip
        self.lora.show_text_wrap(str_ip)
        print("MODE_GATE")
        self.set_callback(self.on_gate_receiver)
        self.lora.receive()
        self.gate_working()
        pass

    #设置回调任务
    def set_callback(self, callback_task):
        self.lora.onReceive(callback_task)
    
    #发送数据
    def sendMessage(self, message):
        self.lora.println(message)
        #print("Sending message:\n{}\n".format(message))
        print("Sending message:")
        print(message)


    #网关模式
    def gate_working(self):
        self.sendMessage("empty")
        onoff = "OFF"
        print("prepare create socket")
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("already create socket")
        s.bind(('', 80))
        print("bind")
        s.listen(5)
        print("listen")
        
        while True:
            #阻塞，等待webserver请求
            conn, addr = s.accept()
            print('Got a connection from %s' % str(addr))
            request = conn.recv(1024)
            request = str(request)
            print('Content = %s' % request)

            #解析请求并发送控制继电器指令
            R1_ON = request.find('/?R1=on')
            R1_OFF = request.find('/?R1=off')
            R2_ON = request.find('/?R2=on')
            R2_OFF = request.find('/?R2=off')
            R3_ON = request.find('/?R3=on')
            R3_OFF = request.find('/?R3=off')
            R4_ON = request.find('/?R4=on')
            R4_OFF = request.find('/?R4=off')
            if R1_ON == 6:
                print('R1 ON')
                onoff = 'R1ON'
            if R1_OFF == 6:
                print('R1 OFF')
                onoff = 'R1OFF'
            if R1_ON == 6:
                print('R2 ON')
                onoff = 'R2ON'
            if R2_OFF == 6:
                print('R2 OFF')
                onoff = 'R2OFF'
            if R3_ON == 6:
                print('R3 ON')
                onoff = 'R3ON'
            if R3_OFF == 6:
                print('R3 OFF')
                onoff = 'R3OFF'
            if R4_ON == 6:
                print('R4 ON')
                onoff = 'R4ON'
            if R4_OFF == 6:
                print('R4 OFF')
                onoff = 'R4OFF'

            temp_msg = "RELAY" + onoff
            self.sendMessage(temp_msg)
            
            #等待lora响应
            self.lora.receive()
            print("wait lora callback")

            #阻塞等待lora回调完毕
            #解析在回调中进行，结果放到对象的字符串里

            now = config_lora.millisecond()
            while(self.flag == 0):
                if config_lora.millisecond() - now > 3000:
                    print("Callback time out.")
                    break
                pass

            self.flag = 0
            print(self.buff)

            #没找到关闭回调的命令，手动发送个无意义的进入发送模式屏蔽回调
            self.sendMessage("empty")

            #响应web请求
            response = webserver.web_page(self.relay_status)
            conn.send('HTTP/1.1 200 OK\n')
            conn.send('Content-Type: text/html\n')
            conn.send('Connection: close\n\n')
            conn.sendall(response)
            conn.close()

            #显示到屏幕上
            

            #准备下一个循环


    #网关模式回调
    def on_gate_receiver(self, payload):    
        print("On gate receive") 
        rssi = self.lora.packetRssi()
        
        try:
            #print payload可知报文前4字节和后一字节无意义
            if int(payload[0]) == 255:
                length=len(payload)-1   
                payload_string = str((payload[4:length]),'utf-8')
            else:
                payload_string = str(payload,'utf-8')        
            print("Received message:\n{}".format(payload_string))
            self.buff = payload_string
            self.relay_status = str(self.buff)
            self.flag = 1

        except Exception as e:
            print(e)
        print("with RSSI {}\n".format(rssi))

