
void  publishStates() {
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    char* state = relayStates[thisPin] == HIGH ? msPowerOff  : msPowerOn ;
    publishRelayState(thisPin + 1, state);
  }
}


void switchRelay(char* switchState, int pos) {
  Serial.println(pos);
  if (switchState == '0') {
    //Turning off is safe to be done immediately
    digitalWrite(relayPins[pos], HIGH);
    relayStates[pos] = HIGH;
    publishRelayState(pos +1,msPowerOff) ;
  } else if (switchState == '1') {
    //turning on is done in setStates
    digitalWrite(relayPins[pos], LOW);
    publishRelayState(pos +1,msPowerOn) ;
    publishRelayState ;
    //
    relayStates[pos] = LOW;
  }
}




void setStates() {
  if (doSwitch) {
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      digitalWrite(relayPins[thisPin], relayStates[thisPin]);
    }
    doSwitch = true;
    doAllOff = false;
    delay_30s.start(30000, AsyncDelay::MILLIS);
  }
}



void turnAllOff() {
  if (doAllOff) {
    doAllOff = false;
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      relayStates[thisPin] = HIGH;
      publishRelayState(thisPin + 1, "OFF");
      switchRelay('0', thisPin);
    }
  }
}




void publishRelayState(int relayNbr, char* state) {
  char outputTopicBuff[100];
  strcpy(outputTopicBuff, outRelayTopicV);
  char relaybuffer[5];
  sprintf(relaybuffer, "%d", relayNbr);
  strcat(outputTopicBuff, relaybuffer);
  client.publish(outputTopicBuff, state);
}



void mutuallyExcludePair(char* switchState, int pos) {
  if (pos % 2 == 0 && switchState == '1') //even
  {
    if (pinCount > (pos + 1) && relayStates[pos + 1] == LOW) {
      switchRelay('0', pos + 1);
      publishRelayState(pos + 2, "OFF");
    }
  }
  else if (pos % 2 == 1 && switchState == '1') //odd
  {
    if ((pos - 1) >= 0 && relayStates[pos - 1] == LOW) {
      switchRelay('0', pos - 1);
      publishRelayState(pos, "OFF");
    }
  }
}
