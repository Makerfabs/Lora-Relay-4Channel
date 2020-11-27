import sx127x
import config_lora 
import lora_node
import webserver

    

def main():
    ip = webserver.connect()
    
    controller = config_lora.Controller()                
    lora = controller.add_transceiver(sx127x.SX127x(name = 'LoRa'),
                                      pin_id_ss = config_lora.Controller.PIN_ID_FOR_LORA_SS,
                                      pin_id_RxDone = config_lora.Controller.PIN_ID_FOR_LORA_DIO0)
    print('lora', lora)
    print("ready to create node")
    
    #node = lora_node.Lora_Node("xbw","001",1)
    #gate = lora_node.Lora_Gate("Master1", lora_node.MODE_RECEIVER, lora, node)

    #node = lora_node.Lora_Node("xbw","001",1)
    #gate = lora_node.Lora_Gate("Master1", lora_node.MODE_TRANSFER, lora, node)

    gate = lora_node.Lora_Gate("Master1", lora_node.MODE_GATE, lora, ip)
    
    gate.working()
    print("Error ,program over")

    
if __name__ == '__main__':
    main()














