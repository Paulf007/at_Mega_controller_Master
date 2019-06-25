void subscribe_intopic (){
  client.subscribe(inTopicV);
  client.subscribe("inTopic/test2");
  client.subscribe(chTopicV);
  client.subscribe(lkTopic);
 
  int idcheck = EEPROM.read(0);
     if (idcheck != ID){  } // Check if there is a value
     if (idcheck == ID){
       String recivedData;
       recivedData = read_String(55);
       String intopic = "cmd/"+recivedData+"loop" ;
       const char* mtopic = intopic.c_str();
       client.subscribe(mtopic);
    }
}


String val;
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

String msgIN = "";
for (int i=0;i<length;i++)
{
msgIN += (char)payload[i];
}
val = msgIN;

  
  //Serial.println();
  //Comes in as domogik/in/relay/r1 ... domogik/in/relay/r16
  String topicString = String(topic);
  //String s = String(inTopicV);
   int len = (strlen(inTopicV)+4);
   int len2 = len+2;

  int relayNumber = topicString.substring(len,len2).toInt(); // This is the reason why the main topic can only be 9 caractors
  //Serial.print("Relay Number is: ");
  //Serial.println(relayNumber);

  int len3 = strlen(lkTopic);
  //int len4 = len3 +2;
  String s = topicString.substring(0,(len3-2));
  int cmdNr = topicString.substring((len3-1), len3+2).toInt();
   //Serial.print("Len3 is : ");
   //Serial.println(len3);
   //Serial.print("Len4 is : ");
   //Serial.println(len4);
   //Serial.println(s);
   //Serial.print("Command Nr: ");
   //Serial.println(cmdNr);
   strcpy(topic1, s.c_str());
  
  //sprintf(lkTopic1 ,"%s",topicStr1);
  
  //String topic1 = topicString.substring(0, 30);
  int posInArray = relayNumber - 1; // relay number are set so the "r" is remved so when the switch is done it is just the number
 // Serial.println(posInArray);
  char* switchState = (char)payload[0];

  if (switchState == '2') {
    if (relayStates[posInArray] == HIGH) {
      switchState = '1';
    }
    else {
      switchState = '0';
    }
  }

  if (isMutuallyExclude) {
    mutuallyExcludePair(switchState, posInArray);
  }

  switchRelay(switchState, posInArray);
  if (switchState == '1') {
    doSwitch = true;
  }
  delayTimers();

// Check for Config topic and if it is the one write the payload to EEPROM
// set Config topic to monitor
      //char Chtopic[50];

for (int i=0;i<length;i++) {
    //Serial.print((char)payload[i]);
    }
     payload[length] = '\0';
     if (strstr(topic, chTopicV)){
    String value = String((char*)payload);
    
    Serial.println("Writning to EEPROM");
     writeString(55,value);
     EEPROM.write(0, 0x92);
     Serial.println("Writning to EEPROM Done");
     wdt_enable(WDTO_4S); 
     delay(1500);
     String recivedData ;
     recivedData = read_String(55);
     Serial.println(recivedData);
     }else if (strstr(topic1, lkTopic1)) {
        Serial.println("Update EEPROM with relay link :");
        String value = String((char*)payload);
        Serial.print("MQTT Pay Load: ");
        Serial.println (val);
          if (cmdNr >= 100 && cmdNr <= 200 ){ // Cmd - is for linking the relay with the switch
            int RelayPl = (cmdNr*2)-97 ;// +2 is to make sure that two places is used fo the int
            int SwitchPl = atol(val.c_str()) ; // Payload of the MQTT Measage
           // Serial.print("Switch Before send: ");   
           // Serial.println(RelayPl);
           // Serial.print("Relay Before send: "); 
           // Serial.println(SwitchPl);
            write_data(RelayPl,SwitchPl);
              }else if (cmdNr >= 200 && cmdNr <= 221 ){
                if (cmdNr == 201){ // The Code to activate or deactivate the Auto Switch
                cmnd = atol(val.c_str());
                  if (cmnd == 1 or cmnd == 0){ // Verify that the correct command was sent. 
                  write_data(201,cmnd);
            //      Serial.println("Auto Update set"); 
                  update_sw (201);  
                                    }  
                  }else if (cmdNr == 202) {  // Switch to activate manual overide 
                  write_data(202,cmnd);
           //       Serial.println("Auto Update set"); 
                  update_sw (202);                    
                        }
                    }
            }
     }     


// Connect to MQTT Serer
boolean reconnect() {
 String clientId = "Arduino-";
 clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), LWTV , 1, true, "Offline")) {
      Serial.println("connected");
      //client.publish(outTopicV, "Online");
      client.publish(LWTV, "Online");
      readLinks ();
      sprintf(mqttStat ,"%s %s","MQTT is:",vOnline); 
     // mqttIpadr ();
      digitalWrite(13, HIGH);
      publishStates();
      subscribe_intopic (); // Call to subscribe to topics
      overide = 0 ;
  }
  return client.connected();
  //sprintf(mqttStat ,"%s %s","MQTT is:",vOffLine); 
}
