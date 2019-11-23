void time_data () {
if (stop_publish == 0){  
 unsigned long currentMillis = millis(); // grab current time
  // check if "interval" time has passed (1000 milliseconds)
 if ((unsigned long)(currentMillis - previousMillis) >= interval) {
 dht11 ();
 publishStates(); // states of the relays will be sent every 5 min - 
    }
  } 
}

void dht11 () {
 
  int chk = DHT.read11(DHT11_PIN);
  temp = DHT.temperature;
  hum = DHT.humidity;
  show_tempR4 (temp,hum);// to OLED
  int anl = analog ();
  //Serial.print("Analog = ");
  //Serial.println(anl);
 //Serial.print("Humidity = ");
 //Serial.println(DHT.humidity);
 // Serial.println (hum);
       //convert ip Array into String
  ipmq = String (Ethernet.localIP()[0]);
  ipmq = ipmq + ".";
  ipmq = ipmq + String (Ethernet.localIP()[1]);
  ipmq = ipmq + ".";
  ipmq = ipmq + String (Ethernet.localIP()[2]);
  ipmq = ipmq + ".";
  ipmq = ipmq + String (Ethernet.localIP()[3]);
  //Serial.println(ip);
 // client.publish(outTopic, ipmq.c_str());
 

  String  payload1 = "{";
          payload1 += "\"Ver\":"; payload1 += ver; payload1 += ",";
          payload1 += "\"ip\":\""; payload1 += ipmq.c_str();  payload1 += "\",";
          payload1 += "\"Overide\":"; payload1 += customLink; payload1 += ",";
          payload1 += "\"Anl\":"; payload1 += anl; payload1 += ",";
          payload1 += "\"DHT11\":{"; 
          payload1 += "\"Temp\":"; 
          payload1 += DHT.temperature; 
          payload1 += ",";
          //payload1 += "\"P_m\":"; payload1 += temp_m; payload1 += ",";
          //payload1 += "\"P_a\":"; payload1 += temp_a; payload1 += ",";
          //payload1 += "\"ip\":"; payload1 += "\"192.168.1.11\""; payload1 += ",";
          payload1 += "\"Hum\":"; payload1 += DHT.humidity;
          payload1 += "}";
          payload1 += "}";
  char payLd[100] ;
  payload1.toCharArray( payLd, 100 );         
  client.publish(outTopicV,payLd);
 // Serial.println ();
 //Serial.print (F("MQTT Data Sent"));
 // Serial.println(freeMemory());
  previousMillis = millis();
//  }
// }  
}
// Needs to be activated in setup
int analog (){
  //if (analogSw == 1){
     long sum = 0;
    for(int i=0; i<100; i++)
    {
       sum += analogRead(analogSensor);
    }
 
    sum = sum/100;
    return sum ;
    //Serial.println(sum);
    //delay(10);
//  }
}
