// Input Pins are wathched and if triggered it will send the state to the MQTT server in JSON FROMAT - the J-Sonlibary should be uitlised in order to optimase the 
// Sketch

void read_input_pins (){
// Read Inputs and Report States
  for (int i = 0; i < NUM_BUTTONS; i++)  {
    // Update the Bounce instance :
    buttons[i].update();
    // If it fell, flag the need to toggle the LED
    if ( buttons[i].fell() ) {
      char stopic[40];
      sprintf(stopic ,"%s%d",outPinTopicV,i);
      client.publish(stopic, msSwitchOn);
      int_link(i); // sent to c_sw_relay_link
      connect_relay(i);
      
      // Set Overide ON or OFF
      if (i == 34){
        overide = 1 ;
        customLink = 0; // Disable Custom Link so that the Standard Liks can Operate 
      }
      
  //   needToToggleLed = true;
    }else if ( buttons[i].rose() ){
      char stopic[40];
      sprintf(stopic ,"%s%d",outPinTopicV,i);
      client.publish(stopic, msSwitchOff);
      int_link(i); // sent to c_sw_relay_link
      connect_relay(i);
   Serial.println(i);
   if (i == 34){
        overide = 0 ;
       
      }

    }
  }

}

// Add Funtion to link Relay to button toggle - will add 10 relays to 10 inputs
void connect_relay (int swpin) {                  // void switchRelay(char* switchState, int pos)
  Serial.print("Input Pin :");
  Serial.println(swpin);
  Serial.print("Overide :");
  Serial.println(overide);
  int switchState;
if (overide == 1){   
     if (swpin == 0){ switchState = check_relay(0);
                      switchRelay(switchState, 0);} // (relaycmd,relaypin) relay pin that is controlled -1 
else if (swpin == 1){ switchState = check_relay(1);
                      switchRelay(switchState, 1);} 
else if (swpin == 2){ switchState = check_relay(2);
                      switchRelay(switchState, 2);}                       
else if (swpin == 3){ switchState = check_relay(3);
                      switchRelay(switchState, 3);} 
else if (swpin == 4){ switchState = check_relay(4);
                      switchRelay(switchState, 4);} 
else if (swpin == 5){ switchState = check_relay(5);
                      switchRelay(switchState, 5);}  
else if (swpin == 6){ switchState = check_relay(6);
                      switchRelay(switchState, 6);}                                            
else if (swpin == 7){ switchState = check_relay(7);
                      switchRelay(switchState, 7);} 
 }
}

int check_relay (int rpin){
  if (relayStates[rpin] == HIGH) { // check state of relay 2
      return '1';
    }
    else {
      return '0';
    }
}

void publishPinState(int pinNbr, char* state) {
  char outputTopicBuff[100];
  strcpy(outputTopicBuff, outPinTopicV);
  char relaybuffer[5];
  sprintf(relaybuffer, "%d", pinNbr);
  strcat(outputTopicBuff, relaybuffer);
  client.publish(outputTopicBuff, state);
}
