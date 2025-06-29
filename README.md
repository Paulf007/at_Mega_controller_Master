# at_Mega_controller_Master
Relay and Input Controller via MQTT

DHT sensor is connected to PIN 5

Start up from scratch :
    1. Default Ip for config is 192.168.8.2
    2. Got to http://192.168.0.2/setup and set the nessasary network settings press submit 
    3. Your device will restart and connect to your broker. (Red LED will be on permanent if connected)
    4. Go to http://”new device ip” to get the config topic – Should add your own custom main topic 
	The default topic on startup is : cmd//changetopic 

	The Command topic will be :  cmd//changetopic and 	
	Payload 			 :  mainhouse

 This needs to change as the 	commands need identify your custom name – below is an example to the custom topic used 	“mainhouse”
Topics for MQTT Server 
General info Topic :
stat/mainhouse/state
Relay Command topic :
cmd/mainhouse/POWER/#
Where "/r1 = 1
 - ON
Where "/r1 = 0
 - OFF
Where "/r1 = 2
- Toggle
Status of relay is reported on :
stat/mainhouse/POWER
Status of Input Pins :
stat/mainhouse/SWITCH
Last will and testament :
stat/mainhouse/LWT
Main topic can be changed at :
cmd/atmegarelay/mainhouse/changetopic

NOTE THE MAIN TOPIC MUST BE UNIQUE ! 

Commands : 

Only Switch 0 – 21 can be used to link to relays

To link the Relay with the Switch and save it to the eeprom so that it is used in future -
Use Topic : cmd/atmegarelay/%yourTpopic%/relayLink/100 -  where switchpin number + 100
with the payload being the Relay Pin number so to link Switch 0 with Relay 2 the command will be:
cmnd/mainhouse/relayLink/100 	 Payload : 2
Note - the Board will be restarted after each command to ensure that he link is added 

To Change the Auto Command from ON or OFF use the following command 

cmnd/mainhouse/relayLink/201 	 Payload : 1 – ON or 2 – OFF 
