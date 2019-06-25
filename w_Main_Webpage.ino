// Full Code to Display the Main Webpage and Write data to EEPROM
void webpage(){

  
  EthernetClient client = server.available();
  if (client) {
    TextFinder  finder(client );
    while (client.connected()) {      
      if (client.available()) {
        //This part does all the text searching
        if( finder.find("GET /") ) {
          // if you find the word "setup" continue looking for more
          // if you don't find that word, stop looking and go further
          // This way you can put your own webpage later on in the sketch
                  
          if (finder.findUntil("setup", "\n\r")){
            // if you find the word "SBM" continue looking for more
            // if you don't find that word, stop looking and go further
            // it means the SUBMIT button hasn't been pressed an nothing has
            // been submitted. Just go to the place where the setup page is
            // been build and show it in the client's browser.
          if (finder.findUntil("SBM", "\n\r")){
            byte SET = finder.getValue();
            // Now while you are looking for the letters "DT", you'll have to remember
            // every number behind "DT" and put them in "val" and while doing so, check
            // for the according values and put those in mac, ip, subnet, gateway and server.
              while(finder.findUntil("DT", "\n\r")){
                int val = finder.getValue();
                // if val from "DT" is between 1 and 6 the according value must be a MAC value.
                if(val >= 1 && val <= 6) {
                  mac[val - 1] = finder.getValue();
                }
                // if val from "DT" is between 7 and 10 the according value must be a IP value.
                if(val >= 7 && val <= 10) {
                  ip[val - 7] = finder.getValue();
                }
                // if val from "DT" is between 11 and 14 the according value must be a MASK value.
                if(val >= 11 && val <= 14) {
                  subnet[val - 11] = finder.getValue();                                               //################
                }
                // if val from "DT" is between 15 and 18 the according value must be a GW value.
                if(val >= 15 && val <= 18) {
                  gateway[val - 15] = finder.getValue();
                }
    //###############
                // if val from "DT" is between 19 and 22 the according value must be a SR value.
                if(val >= 19 && val <= 22) {
                  mqttserver[val - 19] = finder.getValue();
                }
              }
//              
            // Now that we got all the data, we can save it to EEPROM
            for (int i = 0 ; i < 6; i++){
              EEPROM.write(i + 1,mac[i]);
            }
            for (int i = 0 ; i < 4; i++){
              EEPROM.write(i + 7, ip[i]);
            }
            for (int i = 0 ; i < 4; i++){
              EEPROM.write(i + 11, subnet[i]);
            }
            for (int i = 0 ; i < 4; i++){
              EEPROM.write(i + 15, gateway[i]);
            }
//####################
            for (int i = 0 ; i < 4; i++){
              EEPROM.write(i + 19, mqttserver[i]);
            }            
            // set ID to the known bit, so when you reset the Arduino is will use the EEPROM values
            EEPROM.write(0, 0x92);
            toggleLED ();
            
            wdt_enable(WDTO_4S); 
            // if al the data has been written to EEPROM we should reset the arduino
            // for now you'll have to use the hardware reset button
          }
          // and from this point on, we can start building our setup page
          // and show it in the client's browser.

  
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          //client.println("Content-Type: text/html");
///////////////////// Add WEB BANNER
/* 
 client.println();
          for (int i = 1; i < 15; i++) //each image (of the 8 in total) has been divided in an array of parts, this is a small loop putting the 8 parts together
            {
            strcpy_P(buffer, (char*)pgm_read_word(&(string_table7[i])));
            client.print( buffer );
            }
*/
//#########################          
          for (int i = 0; i < 4; i++)
            {
            strcpy_P(buffer, (char*)pgm_read_word(&(string_table0[i])));
            client.print( buffer );
            }
          //########### MAC 
          for (int i = 0; i < 3; i++)
            {
            strcpy_P(buffer, (char*)pgm_read_word(&(string_table1[i])));
            client.print( buffer );
            }
          client.print(mac[0],HEX);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table1[3])));
          client.print( buffer );
          client.print(mac[1],HEX);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table1[4])));
          client.print( buffer );
          client.print(mac[2],HEX);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table1[5])));
          client.print( buffer );
          client.print(mac[3],HEX);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table1[6])));
          client.print( buffer );
          client.print(mac[4],HEX);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table1[7])));
          client.print( buffer );
          client.print(mac[5],HEX);

          //########### IP
          for (int i = 0; i < 4; i++)
            {
            strcpy_P(buffer, (char*)pgm_read_word(&(string_table2[i])));
            client.print( buffer );
            }
          // ###########  
          client.print(ip[0],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table2[4])));
          client.print( buffer );
          client.print(ip[1],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table2[5])));
          client.print( buffer );
          client.print(ip[2],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table2[6])));
          client.print( buffer );
          client.print(ip[3],DEC);
          
          // ########### MASK
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table3[0])));
          client.print( buffer );
          client.print(subnet[0],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table3[1])));
          client.print( buffer );
          client.print(subnet[1],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table3[2])));
          client.print( buffer );
          client.print(subnet[2],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table3[3])));
          client.print( buffer );
          client.print(subnet[3],DEC);

          // ########### GAteWay
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table4[0])));
          client.print( buffer );
          client.print(gateway[0],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table4[1])));
          client.print( buffer );
          client.print(gateway[1],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table4[2])));
          client.print( buffer );
          client.print(gateway[2],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table4[3])));
          client.print( buffer );
          client.print(gateway[3],DEC);

//###########       MQTT SERVER   ###########
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table5[0])));
          client.print( buffer );
          client.print(mqttserver[0],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table5[1])));
          client.print( buffer );
          client.print(mqttserver[1],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table5[2])));
          client.print( buffer );
          client.print(mqttserver[2],DEC);
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table5[3])));
          client.print( buffer );
          client.print(mqttserver[3],DEC);
     // ## ON CLICK     
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table5[4])));
          client.print( buffer );
       //   client.print(mqttserver[4],DEC);

        
          for (int i = 0; i < 7; i++)
            {
            strcpy_P(buffer, (char*)pgm_read_word(&(string_table6[i])));
            client.print( buffer );
            }
          strcpy_P(buffer, (char*)pgm_read_word(&(string_table5[5])));
          client.print( buffer );  

          break;
        }
      }
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        //client.println("Connection: close"); 
        //client.println("Refresh: 5");
        client.println();
        sprintf(LWTV ,"%s%s%s",stat,maintopicV,"/LWT");

//#############################################################################################################
 client.println();
          for (int i = 1; i < 15; i++) //each image (of the 8 in total) has been divided in an array of parts, this is a small loop putting the 8 parts together
            {
            strcpy_P(buffer, (char*)pgm_read_word(&(string_table7[i])));
            client.print( buffer );
            }

client.println("<td width=120px><font color=\"white\" size=\"2\" face=\"Verdana\"></font></td><td width=120px>");
client.print("<a href=\"http://");
client.print(Ethernet.localIP());
client.print("/setup\"\"");


client.println("<style=\"text-align: center; font color=\"white\" size=\"2\" face=\"Verdana\">Network setup</font></a></td><td></td></tr></table></body></html>");
        // put your own html from here on
        client.print("<h2 style=\"text-align: center;\"><em><strong>Current Ip Adress : ");
        client.println(Ethernet.localIP());
        client.println("</strong></em></h2>");
       client.println("<p style=\"text-align: center;\"><em><strong>Temprature: ");
       client.println(temp);
       client.println(" &degC , Humidity");
       client.println(hum);

       client.println();
          for (int i = 0; i < 5; i++) 
            {
            strcpy_P(buffer, (char*)pgm_read_word(&(string_table8[i])));
            client.print( buffer );
            }
        /*
       client.println(" %</strong></em></p> ");
       client.println("<p style=\"text-align: center;\"><em><strong>Topics for MQTT Server</strong></em></p> ");
       client.println("<table style=\"height: 210px; margin-left: auto; margin-right: auto; width: 490px;\"><tbody><tr> ");
       client.println("<td style=\"width: 251.5px; text-align: right;\"><em><strong>General info&nbsp;Topic :</strong></em></td>");
       client.println("<td style=\"width: 224.5px; text-align: left;\"><em><strong>");//stat/"+maintopic+"/state</strong></em></td></tr><tr>");
       */
       client.println(outTopicV);
       client.println("</strong></em></td></tr><tr>");
       client.println("<td style=\"width: 251.5px; text-align: right;\"><em><strong>Relay Command topic :</strong></em></td> ");
       client.println("<td style=\"text-align: left;\"><em><strong>");
       client.println(inTopicV);

              client.println();
          for (int i = 0; i < 9; i++) 
            {
            strcpy_P(buffer, (char*)pgm_read_word(&(string_table9[i])));
            client.print( buffer );
            }
  /*            
       client.println("</strong></em></td></tr><tr>");   
       client.println("<td style=\"width: 251.5px; text-align: right;\"><em><strong>Where \"/r1 =&nbsp;1</strong></em></td> ");
       client.println("<td style=\"width: 224.5px; text-align: left;\"><em><strong>&nbsp;- ON</strong></em></td></tr><tr>");
       client.println("<td style=\"width: 251.5px; text-align: right;\"><em><strong>Where \"/r1 = 0</strong></em></td> ");
       client.println("<td style=\"width: 224.5px; text-align: left;\"><em><strong>&nbsp;- OFF</strong></em></td></tr><tr> "); // </tr><tr>
       client.println("<td style=\"width: 251.5px; text-align: right;\"><em><strong>Where \"/r1 = 2</strong></em></td> ");
       client.println("<td style=\"width: 224.5px; text-align: left;\"><em><strong>- Toggle</strong></em></td></tr><tr> ");
       client.println("<td style=\"width: 251.5px; text-align: right;\"><em><strong>Status of relay is reported on :</strong></em></td> ");
       client.println("<td style=\"width: 224.5px; text-align: left;\"><em><strong>");
  */
       client.println(outRelayTopicV);
                     client.println();
          for (int i = 0; i < 3; i++) 
            {strcpy_P(buffer, (char*)pgm_read_word(&(string_table10[i])));
            client.print( buffer );
            }
   /*    
       client.println("</strong></em></td></tr><tr>"); 
       client.println("<td style=\"width: 251.5px; text-align: right;\"><em><strong>Status of Input Pins :</strong></em></td> ");
       client.println("<td style=\"width: 224.5px; text-align: left;\"><em><strong>");
   */     
       client.println(outPinTopicV);
                            client.println();
          for (int i = 0; i < 3; i++) 
            {strcpy_P(buffer, (char*)pgm_read_word(&(string_table11[i])));
            client.print( buffer );
            }
   /*    
       client.println("</strong></em></td></tr><tr>"); 
       client.println("<td style=\"width: 251.5px; text-align: right;\"><em><strong>Last will and testament :</strong></em></td> ");
       client.println("<td style=\"width: 224.5px; text-align: left;\"><em><strong>");
  */    
       client.println(LWTV);
        client.println();
          for (int i = 0; i < 3; i++) 
            {strcpy_P(buffer, (char*)pgm_read_word(&(string_table12[i])));
            client.print( buffer );
            }
 
        client.println(chTopicV);
   //     client.println();
   //       for (int i = 0; i < 2; i++) 
   //         {strcpy_P(buffer, (char*)pgm_read_word(&(string_table13[i])));
   //         client.print( buffer );
   //         } 
 
       client.println();
          for (int i = 0; i < 3; i++) 
            {strcpy_P(buffer, (char*)pgm_read_word(&(string_table14[i])));
            client.print( buffer );
            }
   /*    
       client.println("</strong></em></td></tr><tr>"); 
       client.println("<td style=\"width: 251.5px; text-align: right;\"><em><strong>Last will and testament :</strong></em></td> ");
       client.println("<td style=\"width: 224.5px; text-align: left;\"><em><strong>");
  */    
       client.println(outLinkRw);
       //client.println("Test Info");
        client.println();
          for (int i = 0; i < 3; i++) 
            {strcpy_P(buffer, (char*)pgm_read_word(&(string_table15[i])));
            client.print( buffer );
            }
 
  
 /*      
       client.println("</strong></em></td></tr><tr>"); 
       client.println("<td style=\"width: 300.5px; text-align: right;\"><em><strong>Main topic can be changed at :</strong></em></td> ");
       client.println("<td style=\"width: 224.5px; text-align: left;\"><em><strong>");
 */      
       client.println(lkTopic);
       
        client.println();
          for (int i = 0; i < 2; i++) 
            {strcpy_P(buffer, (char*)pgm_read_word(&(string_table16[i])));
            client.print( buffer );
            }       
 /*      
       client.println("</strong></em></td></tr></tbody></table>"); 
       client.println("<p style=\"text-align: center;\"><strong><span style=\"color: #ff0000;\">NOTE THE MAIN TOPIC CAN ONLY BE 9 CHARACTERS!</span></strong></p>");
      // Relay Inout Tables 
      client.println("<p style=\"text-align: center;\"><strong><span style=\"color: #052303;\">Relay Status</span></strong></p>");
      client.println ("<table style=\" margin-left: auto; margin-right: auto\"><tbody>");
      client.println ("<tr><td>Relay 1:</td><td>");
 //     
      if (digitalRead(22)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 2:</td><td>");
      if (digitalRead(23)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 3:</td><td>");
      if (digitalRead(24)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 4:</td><td>");
      if (digitalRead(25)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td></tr>");
      client.println ("<tr><td>Relay 5:</td><td>");
      if (digitalRead(26)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 6:</td><td>");
      if (digitalRead(27)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 7:</td><td>");
      if (digitalRead(28)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 8:</td><td>");
      if (digitalRead(29)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //46%
      client.println ("</td></tr>");
      client.println ("<tr><td>Relay 9:</td><td>");
      if (digitalRead(30)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 10:</td><td>");
      if (digitalRead(31)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 11:</td><td>");
      if (digitalRead(32)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 12:</td><td>");
      if (digitalRead(33)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td></tr>");      
      client.println ("<tr><td>Relay 13:</td><td>");
      if (digitalRead(34)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 14:</td><td>");
      if (digitalRead(35)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 15:</td><td>");
      if (digitalRead(36)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 16:</td><td>");
      if (digitalRead(37)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td></tr>");    
      client.println ("<tr><td>Relay 17:</td><td>");
      if (digitalRead(38)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 18:</td><td>");
      if (digitalRead(39)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 19:</td><td>");
      if (digitalRead(40)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 20:</td><td>");
      if (digitalRead(41)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //46%
      client.println ("</td></tr>");    
      client.println ("<tr><td>Relay 21:</td><td>");
      if (digitalRead(42)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 22:</td><td>");
      if (digitalRead(43)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 23:</td><td>");
      if (digitalRead(44)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Relay 24:</td><td>");
      if (digitalRead(45)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //50%
      client.println ("</td></tr>");   
      client.println ("</tbody></table>");
      
      // Input Tables
      client.println("<p style=\"text-align: center;\"><strong><span style=\"color: #052303;\">Input Status</span></strong></p>");
      client.println ("<table style=\" margin-left: auto; margin-right: auto\"><tbody>");
      client.println ("<tr><td>Switch 1:</td><td>");
      if (digitalRead(0)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 2:</td><td>");
      if (digitalRead(1)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 3:</td><td>");
      if (digitalRead(2)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 4:</td><td>");
      if (digitalRead(3)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td></tr>");
      client.println ("<tr><td>Switch 5:</td><td>");
      if (digitalRead(6)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 6:</td><td>");
      if (digitalRead(7)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 7:</td><td>");
      if (digitalRead(8)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 8:</td><td>");
      if (digitalRead(9)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //46%
      client.println ("</td></tr>");
      client.println ("<tr><td>Switch 9:</td><td>");
      if (digitalRead(9)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 10:</td><td>");
      if (digitalRead(11)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 11:</td><td>");
      if (digitalRead(12)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 12:</td><td>");
      if (digitalRead(14)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td></tr>");      
      client.println ("<tr><td>Switch 13:</td><td>");
      if (digitalRead(15)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 14:</td><td>");
      if (digitalRead(16)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 15:</td><td>");
      if (digitalRead(17)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 16:</td><td>");
      if (digitalRead(18)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td></tr>");    
      client.println ("<tr><td>Switch 17:</td><td>");
      if (digitalRead(19)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 18:</td><td>");
      if (digitalRead(20)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 19:</td><td>");
      if (digitalRead(21)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 20:</td><td>");
      if (digitalRead(46)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //46%
      client.println ("</td></tr>");    
      client.println ("<tr><td>Switch 21:</td><td>");
      if (digitalRead(47)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 22:</td><td>");
      if (digitalRead(48)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 23:</td><td>");
      if (digitalRead(49)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 24:</td><td>");
      if (digitalRead(A0)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //50%
      client.println ("</td></tr>"); 

      client.println ("<tr><td>Switch 25:</td><td>");
      if (digitalRead(A1)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 26:</td><td>");
      if (digitalRead(A2)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 27:</td><td>");
      if (digitalRead(A3)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 27:</td><td>");
      if (digitalRead(A4)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //50%
      client.println ("</td></tr>");      

      client.println ("<tr><td>Switch 28:</td><td>");
      if (digitalRead(A5)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 29:</td><td>");
      if (digitalRead(A6)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 30:</td><td>");
      if (digitalRead(A7)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 31:</td><td>");
      if (digitalRead(A8)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //50%
      client.println ("</td></tr>");
      
      client.println ("<tr><td>Switch 32:</td><td>");
      if (digitalRead(A9)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 33:</td><td>");
      if (digitalRead(A10)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Switch 34:</td><td>");
      if (digitalRead(A11)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //42%
      client.println ("</td><td>Overide:</td><td>");
      if (digitalRead(A15)){client.println("<p style=\"text-align: center;\"><span style=\"color: #ff0000;\">OFF");}else{ client.println("<p style=\"text-align: center;\"><span style=\"color: #00ff04;\">ON");} //66%
      client.println ("</td></tr>");
     
      client.println ("</tbody></table>");
 */ 
     //   client.println(" ");       
        // put your own html until here 
        break;  
        }
      }
    //delay(1);
    client.stop();  
}
}
