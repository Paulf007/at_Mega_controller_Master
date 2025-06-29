void write_data (int addr , int info ){ // update the custom pin link

    int temp_comp ;  
    int temp_val = info ;
    EEPROM.get(addr,temp_comp);
    //Serial.print("Memory Value : ");
    //Serial.println(temp_comp);
    if (temp_comp!=temp_val){
     //Serial.println("Write new Data");
     EEPROM.write(addr,info);
     //Serial.println("Read EEPROM after write:");
    // EEPROM.get(addr,temp_comp);
    // int s = ((addr+97)/2)-100 ;     
    
     /*  
          String  payload1 = "{";
          payload1 += "\"Relay Link Updated\":{"; 
          payload1 += "\"Switch\":"; 
          payload1 += s; 
          payload1 += ",";
          payload1 += "\"Relay\":"; payload1 += temp_comp;
          payload1 += "}";
          payload1 += "}";
          char payLd[100] ;
          payload1.toCharArray( payLd, 100 ); 
     client.publish(outTopicV,payLd);
     */
     readLinks ();
     //update_sw (201); 
     }
   }

void readEEOROMip (){
  valuerom = EEPROM.read(address);
  Serial.print(address);
  Serial.print("\t");
  Serial.print(valuerom, DEC);
  Serial.println();
  address = address + 1;
  if (address == EEPROM.length()) {
    address = 0;
  }
  delay(500);
 }
// Get the maintpoic value from the EEOROM and set for later use
 void readEEOROM (char add){
  Serial.print("Read Main Topic");
  int i;
  char data[200]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<1000)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
    data[len]='\0';
  //return String(data);
  strcpy(maintopicV, data);
  //const char* emaintopic = data;
  Serial.print("Main Topic is:");
  Serial.println(data);
  mt_len = strlen(data); // get the lenth of the main topic so that it can be used in the MQTT recieved payload
  // Set the variable funtions
  setVariableTopics();
  }
