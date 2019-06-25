void setVariableTopics() {
//  This function run once to set all the variuable topics for the after the main topic was set 
 
sprintf(outTopicV ,"%s%s%s",stat,maintopicV,"/state");
sprintf(inTopicV ,"%s%s%s",cmd,maintopicV,Powera);
sprintf(outRelayTopicV ,"%s%s%s",stat,maintopicV,Power);
sprintf(outPinTopicV ,"%s%s%s",stat,maintopicV,Switch);
sprintf(LWTV ,"%s%s%s",stat,maintopicV,"/LWT");
sprintf(chTopicV ,"%s%s%s",cmd,maintopicV,cnhgeT); 
sprintf(lkTopic ,"%s%s%s",cmd,maintopicV,linkT); 
sprintf(lkTopic1 ,"%s%s%s",cmd,maintopicV,linkT1);
sprintf(outLinkRw ,"%s%s%s%s",stat,maintopicV,link,"#");
Serial.print("Variable Topic Set:");
Serial.println(outLinkRw);
Serial.println(inTopicV);
 
  
}
