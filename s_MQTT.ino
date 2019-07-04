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
/* - use to confirm if MQTT recieve command
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
*/
String msgIN = "";
for (int i=0;i<length;i++)
{
msgIN += (char)payload[i];
}
val = msgIN; // this is the payload from MQTT


  //Serial.println();
  String topicString = String(topic); // convert topic to string 
  int ch_len = (strlen(topic)-mt_len);
 // Serial.print("Lenght of the topic is :");
 //  Serial.println(ch_len); // Print the lenth of the topic so it could be used to determine what should be done with it
  int cmdNr ;
if (ch_len > 22 && ch_len < 25 ){
 // Serial.println("Relay Command");
    //String s = String(inTopicV);
  int len = (strlen(inTopicV)+4);
  int len2 = len+2;
  int relayNumber = topicString.substring(len,len2).toInt(); // the topic gets trimmed so that the relay number is left
  int len3 = strlen(lkTopic);
  String s = topicString.substring(0,(len3-2));
  cmdNr = topicString.substring((len3-1), len3+2).toInt();
  strcpy(topic1, s.c_str());
  int posInArray = relayNumber - 1; // 
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
    
}else if (ch_len == 28){
 // Serial.println("Change Topic Command");
  stop_publish = 1 ;
     delay(1000);
    // String value = String((char*)payload);
     Serial.println("Writning to EEPROM");
    // Serial.println(value);
   //  Serial.print("Val is :");
     Serial.println(val);
     writeString(55,val);
     EEPROM.write(0, 0x92);
     Serial.println("Writning to EEPROM Done");
     wdt_enable(WDTO_4S); 
     delay(1500);
     //String recivedData ;
    // recivedData = read_String(55);
   //  Serial.println(recivedData);

  
}else if (ch_len == 30){

  int len3 = strlen(lkTopic);
  String s = topicString.substring(0,(len3-2));
  int cmdNr = topicString.substring((len3-1), len3+2).toInt();

  
//  Serial.println("Custom Link Command");
  stop_publish = 1 ;
          Serial.println("Update EEPROM with relay link :");
          String value = String((char*)payload);
          Serial.print("MQTT Pay Load: ");
          Serial.println (val);
          if (cmdNr >= 100 && cmdNr <= 200 ){ // Cmd - is for linking the relay with the switch
            int RelayPl = (cmdNr*2)-97 ;// +2 is to make sure that two places is used fo the int
            int SwitchPl = atol(val.c_str()) ; // Payload of the MQTT Measage
            write_data(RelayPl,SwitchPl);
              }else if (cmdNr >= 200 && cmdNr <= 221 ){
                        if (cmdNr == 201){ // The Code to activate or deactivate the Auto Switch
                            cmnd = atol(val.c_str());
                            if (cmnd == 1 or cmnd == 0){ // Verify that the correct command was sent. 
                            write_data(201,cmnd);
                            update_sw (201); 
                            sprintf(outLinkR ,"%s%s%s%d",stat,maintopicV,link,201);
                            //cmnd.toCharArray( payLd,2 ); 
                            client.publish(outLinkR,String(cmnd).c_str()); 
                           }  
                      }else if (cmdNr == 202) {  // Switch to activate manual overide 
                               write_data(202,cmnd);
                              update_sw (202);                    
                        }
                   }
     stop_publish = 0 ;         
              }    
  
}
  
// #############################################################################################################


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
      show_mqtt_status(1);
      update_sw (201); // Update the Custom Link because it was set to 1 when MQTT was discanected
      //customLink = 1;
      //overide = 0 ;
  }
  return client.connected();
  //sprintf(mqttStat ,"%s %s","MQTT is:",vOffLine); 
}
