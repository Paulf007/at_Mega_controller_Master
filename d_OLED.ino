void oledstart() {
  Serial.print("Start OLED !");
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_7x14_1x2_f);
  u8x8.setCursor(0, 2);
  u8x8.print(F("Current Ip"));
  u8x8.setCursor(0, 4);
  u8x8.print(Ethernet.localIP());
  oled_active = 1 ;
  }


void show_mqtt_status (int stat) {
if (oled_active == 1){  
  u8x8.setFont(u8x8_font_victoriamedium8_r);
  u8x8.setCursor(0,0);
  if (stat == 1){
   u8x8.clearLine(0); 
   u8x8.print(F("MQTT: "));
   u8x8.println(F("Online")); 
  }else{
   u8x8.clearLine(0); 
   u8x8.print(F("MQTT:")); 
   u8x8.println(F("Connecting")); 
  }
 }
}

void show_tempR4 (int t,int h){
if (oled_active == 1){    
  u8x8.setFont(u8x8_font_7x14_1x2_f);
  u8x8.setCursor(0, 6);
  u8x8.clearLine(6);
  u8x8.print(F("Temp:"));
  u8x8.println(t);
  u8x8.setCursor(8, 6);
  u8x8.print(F("Hum:"));
  u8x8.println(h);
}
}

void show_statR2 (int i , int pin){
  if (oled_active == 1){  
        char link[10];
        const char* pl;
        u8x8.setFont(u8x8_font_7x14_1x2_f);
        sprintf(link ,"%s%d","Switch ",pin);
        if (i == 1){
        pl = OnA ;
        }else if (i==0) {
        pl = OffA ;          
        }       
        u8x8.setCursor(0, 2);
        u8x8.clearLine(2); 
        u8x8.print(link);
        //u8x8.setCursor(0, 4);
        u8x8.println(pl);
  }
}

void show_statR3 (int i,int pin){
  if (oled_active == 1){  
        char link[10];
        const char* pl;
        u8x8.setFont(u8x8_font_7x14_1x2_f);
        sprintf(link ,"%s%d","Relay ",pin);
        if (i == 1){
        pl = OnA ;
        }else if (i==0) {
        pl = OffA ;          
        }
        u8x8.setCursor(0, 4);
        u8x8.clearLine(4);
        u8x8.print(link);
        //u8x8.setCursor(0, 4);
        u8x8.println(pl);
  }
}
