
// Read all the input pins and check if they are linked to any relays:
void readLinks (){
//int i;
    for (int i = 0; i < 22; i++)      //declaring analog inputs
  {
  //int s = buttPins[i];
  int GetAdd = ((i+100)*2)-97 ; 
  int sw = EEPROM.read(GetAdd);
 // Serial.print("Switch: ");
 // Serial.print(i);
 // Serial.print(" Relay :"); 
 // Serial.print (sw);
 // Serial.println ("");
          String  payload1 = "{";
          //payload1 += "\"Relay Link Updated\":{"; 
          payload1 += "\"Relay\"";
          //payload1 +=i ;
          payload1 +=":"; 
          payload1 += sw; 
         // payload1 += ",";
         // payload1 += "\"Relay\":"; payload1 += sw;
         // payload1 += "}";
          payload1 += "}";
          char payLd[100] ;
          //Serial.print(" Link Topic : ");
          sprintf(outLinkR ,"%s%s%s%d",stat,maintopicV,link,i);
          //Serial.println (outLinkR);
          payload1.toCharArray( payLd, 100 ); 
          client.publish(outLinkR,payLd);
  
  }
}





void int_link(int addr){
  int switchState;
  // first check if int link as active or not.
  //Serial.print("Link in Mem Pin:");
  //Serial.println(addr); 
  if (customLink == 1){
  int relay;
  int GetAdd = ((addr+100)*2)-97 ; // this is how the pin was saved in the memory so that two places was added for each link.  
  //Serial.print("Adress to search:");
  //Serial.println(addr);
  // read the pin in the memory
  // Serial.print("Get Relay Pin Info from EEPROM");
  EEPROM.get(GetAdd,relay);
  int Relaycheck = EEPROM.read(0);
  if (relay!= 0){
  //Serial.print("Link in Mem Pin:");
  //Serial.println(addr);
  //Serial.print("Relay Pin:");
  //Serial.println(relay);
  //check_relay(relay);
  switchState = check_relay(relay-1);
  switchRelay(switchState,relay-1);
    }else{
    //Serial.print("No Link for Pin:"); 
    //Serial.println(addr);     
    }
  }
}




void update_sw (int val){
     int i ;
     EEPROM.get(201,i);
     if (val == 201 && i == 1){
      customLink = 1;
      overide = 0 ;
     }else{
      customLink = 0;
      overide = 1;      
     }
     //customLink = i;      
     Serial.print("Custom Link is: ");
     Serial.println(customLink);
     
}
