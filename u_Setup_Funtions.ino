

void delayTimers() {
  delay_400ms.start(50, AsyncDelay::MILLIS);
  delay_30s.start(1, AsyncDelay::MILLIS);
}



void ShieldSetup(){
  int idcheck = EEPROM.read(0);
  if (idcheck != ID){
    //ifcheck id is not the value as const byte ID,
    //it means this sketch has NOT been used to setup the shield before
    //just use the values written in the beginning of the sketch
  }
  if (idcheck == ID){
    //if id is the same value as const byte ID,
    //it means this sketch has been used to setup the shield.
    //So we will read the values out of EERPOM and use them
    //to setup the shield.
    for (int i = 0; i < 6; i++){
      mac[i] = EEPROM.read(i+1);
    }
    for (int i = 0; i < 4; i++){
      ip[i] = EEPROM.read(i+7);
    }
    for (int i = 0; i < 4; i++){
      subnet[i] = EEPROM.read(i+11);
    }
    for (int i = 0; i < 4; i++){
      gateway[i] = EEPROM.read(i+15);
    }
   //#############
    for (int i = 0; i < 4; i++){         
      mqttserver[i] = EEPROM.read(i+19);
    }
  }
  Serial.println("Connect to Network  .....");
  
Ethernet.begin(mac, ip);
printIPAddress();
toggleLED2 ();
}
