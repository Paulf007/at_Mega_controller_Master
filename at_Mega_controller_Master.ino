/*
Inicial MQTT Code BY  By Jeroen Schaeken
  Derived from http://www.esp8266.com/viewtopic.php?f=29&t=8746
  It connects to an MQTT server then:
  - on 0 switches off relay
  - on 1 switches on relay
  - on 2 switches the state of the relay

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.


The Embedded Network Setup Webpage was Made by JO3RI check http://www.jo3ri.be/arduino
* Add option to set button switch to relay so the the module can work independant 
* Add fixed links that can be activated and de activated - this will then be noted in the manual so that a fixed value connnection can be used. 58%
* Add DHT11 Sensor
* Save static info to program 49
* 1.8 aDD Analog Sensor to test
* 1.9 Add input buttons to web page as well as software vertion to info topic Mem 66%
* 2.0 add OTA update via eathener  73% - Boot loader needs to be changed so will wait to test later
* 21 - optimse webpage space for better resalts in glaobal variables 59%
* 22 - add memory link sothat relay links to switch can be set manual via mqtt 59%
* 23 - add Swicthes to enable or disable the manual switch link
* 24 - add Switch to add manual overide as per OEM
* 25 - add random client name to mqtt connect
* 26 - add check to se if all input pins is linked to the internal memory 
* 27 - change how the relay topic is parced
* UNtested Code must stille be checked 
* 29 - change format of relay topic
* 30 - add test to check if relay topic is used or not
* 31 - Build Report on Custom Rely Links , Remove Relay and Input status on Web Page so that it uses less memory
* 32 - Add json Building Sequince to send payload - add info to main webpage for all the links
* 33 - Add Eathernet Check to network adaptor 
* 
*/

#include <SPI.h>
#include <Ethernet.h>
#include <TextFinder.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h> //  ADD of Wtachdog to reset the board on submit
#include <ArduinoJson.h>
//#include <MemoryFree.h>
int ver = 32 ;
// ######## Custom Link Code ############
int customLink; // Enable or disable - possibly over MQTT
int overide; //= 0 ;
int cmnd;


// ######## Analog Sensor Code ############
int analogSensor = A12;
int analogVar = 0 ;
int analogSw;
int analogTrig = 1024 ; // setup sothat the trigger can be changed in eeprom with mqtt or hard coded. 


// ######## DHT 11 Code ############
#include <dht.h>
dht DHT;
#define DHT11_PIN 5
  // Set Timer
  unsigned long interval=30000; // the time we need to wait
  unsigned long previousMillis=0; // millis() returns an unsigned long.
  int temp;
  int hum;


// ######## RELAY CODE ############
#include <Bounce2.h>
#include <AsyncDelay.h>
long lastMsg = 0;
char msg[50];
int value = 0;
AsyncDelay delay_400ms;
AsyncDelay delay_30s;
//MutuallyExclude the 'on' state between pairs of relays (for shutter to prevent up and down at the same time)
//ask R1 to turn on while R2 = on -> turns off R2 first (and broadcasts 'domogik/in/relay/r2 0')
//ask R2 to turn on while R1 = on -> turns off R1 first (and broadcasts 'domogik/in/relay/r1 0')
// Very IMPORTANT THE MAINTOPIC MUST UNIQUE
bool isMutuallyExclude = false;
// These values will be used when the maintoic was not declred in the EEORom
const char* outTopic = "stat/atmegarelay/maintopic/state";
const char* inTopic = "cmd/atmegarelay/maintopic/#";
const char* outRelayTopic = "stat/atmegarelay/maintopic/POWER";
const char* outPinTopic = "stat/atmegarelay/maintopic/switch/";
const char* LWT = "stat/atmegarelay/maintopic/LWT"; // Last Will and Testament
const char* chTopic = "cmd/atmegarelay/maintopic/changetopic";
// set the Maindefault topic when the Ketch runs for the first time --------------- NEDD TO DO

// Delaire base topic for variable topics these are fixed and will not change
const char* msSwitchOff = "{\"SWITCH\":\"OFF\"}" ;
const char* msSwitchOn = "{\"SWITCH\":\"ON\"}" ;
const char* msPowerOff = "{\"POWER\":\"OFF\"}" ;
const char* msPowerOn = "{\"POWER\":\"ON\"}" ;
const char* cmd = "cmd/atmegarelay/";
const char* stat = "stat/atmegarelay/";
const char* cnhgeT = "/changetopic";
const char* linkT = "/relayLink/#"; // the topic end will be the relay number of the of the or this option can be used
const char* linkT1 = "/relayLink";
const char* link = "/LINK";
const char* Switch = "/SWITCH";
const char* Power = "/POWER";
const char* Powera = "/#";
const char* vOnline ="Online";
const char* vOffLine ="Offline";
//char chTopic[]="cmd/atmegarelay/maintopic/changetopic";



// Setup the full variable topics that will be changed at startup for the rest of the sketch
char maintopicV[20] = "";    // Toipc gets updtated by the readEEOrom Funtion 
char outTopicV[35] = "";
char inTopicV[35] = "";
char outRelayTopicV[35] = ""; 
char outPinTopicV[35] = ""; 
char LWTV[35] = ""; 
char chTopicV[40] = "";   
char lkTopic[40] = "";
char lkTopic1[40] = ""; 
char topic1[40] = ""; 
char mqttStat[15] ="";
char outLinkR[40] ="";
char outLinkRw[50] ="";
// For Setting Up MQTT MAIN TOPIC
//void writeString(char add,String data);
//String read_String(char add);
// Switch Overide 


//seting up the EthernetShield
//change the defaults the match your own network
byte mac[] = {0x30, 0x30, 0xAB, 0xF0, 0x67, 0xE2};
byte ip[] = {192,168,8,2};
byte subnet[] = {255,255,255,0};
byte gateway[] = {192,168,8,1};
byte dnsserver[] = {192,168,8,1};
byte mqttserver[] = {192,168,8,3};
int address = 0;
byte valuerom;
char ipadr[100];
String ipmq = "";
char buffer[200]; // HTML CODE SETUP PAGE
//int address = 0;
//byte valuerom;
EthernetServer server(80);
EthernetClient ethClient;
PubSubClient client(ethClient);
long lastReconnectAttempt = 0;


#define NUM_BUTTONS 35
int buttPins[] = {0,1,2, 3, 6, 7, 8, 9,11,12,14, 15 ,16 ,17 ,18 ,19 , 20 ,21 ,46 ,47 ,48 ,49};
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {0,1,2, 3, 6, 7, 8, 9,11,12,14, 15 ,16 ,17 ,18 ,19 , 20 ,21 ,46 ,47 ,48 ,49 ,A0 ,A1 ,A2 ,A3 ,A4 ,A5 ,A6 ,A7 ,A8 ,A9 ,A10 ,A11,A15};//,68,69};
Bounce * buttons = new Bounce[NUM_BUTTONS];
int ledState = LOW ;
#define LED_PIN 13
bool relayStates[] = {
  HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH
};
int relayPins[] = {
22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 ,45
};       // an array of pin numbers to which LEDs are attached
int pinCount = 24;
boolean doSwitch = false;
boolean doAllOff = false;




// ##################### MQTT END
// This is our buffer through which we will will "flow" our HTML code.
// It has to be as big as the longest character chain +1 including the "


// This is the HTML code all chopped up. The best way to do this is, is by typing
// your HTML code in an editor, counting your characters and divide them by 8.
// you can chop your HTML on every place, but not on the \" parts. So remember,
// you have to use \" instead of simple " within the HTML, or it will not work.

const char htmlx0[] PROGMEM = "<html><title>Arduino Ethernet Setup Page</title><body marginwidth=\"10\" marginheight=\"10\" ";
const char htmlx1[] PROGMEM = "leftmargin=\"1\" style=\"margin: 0; padding: 1;\"><table bgcolor=\"#17A1A5\" border";
const char htmlx2[] PROGMEM = "=\"0\" width=\"100%\" cellpadding=\"1\" style=\"font-family:Verdana;color:#fff";
const char htmlx3[] PROGMEM = "fff;font-size:24px;\"><tr><h2><td>&nbsp Arduino Ethernet Setup Page</td><h2></tr></table><br>";
PROGMEM const char * const string_table0[] = {htmlx0, htmlx1, htmlx2, htmlx3};


// Gets written to mac
const char htmla0[] PROGMEM = "<script>function hex2num (s_hex) {eval(\"var n_num=0X\" + s_hex);return n_num;}";
const char htmla1[] PROGMEM = "</script><table><form><input type=\"hidden\" name=\"SBM\" value=\"1\"><tr><td style=\"width: 64px;\">MAC:</td>"; 
// MAC
const char htmla2[] PROGMEM = "<td style=\"width: 350px;\"><input id=\"T1\" type=\"text\" size=\"2\" maxlength=\"2\" name=\"DT1\" value=\"";               //T1 ; DT1
const char htmla3[] PROGMEM = "\">.<input id=\"T3\" type=\"text\" size=\"2\" maxlength=\"2\" name=\"DT2\" value=\"";           //T3 ;  DT2
const char htmla4[] PROGMEM = "\">.<input id=\"T5\" type=\"text\" size=\"2\" maxlength=\"2\" name=\"DT3\" value=\"";           //T5 ; DT3
const char htmla5[] PROGMEM = "\">.<input id=\"T7\" type=\"text\" size=\"2\" maxlength=\"2\" name=\"DT4\" value=\"";           //T7 ; DT4
const char htmla6[] PROGMEM = "\">.<input id=\"T9\" type=\"text\" size=\"2\" maxlength=\"2\" name=\"DT5\" value=\"";           //T9 ; DT5
const char htmla7[] PROGMEM = "\">.<input id=\"T11\" type=\"text\" size=\"2\" maxlength=\"2\" name=\"DT6\" value=\"";          //T11 ; DT6
PROGMEM const char * const string_table1[] = {htmla0, htmla1, htmla2, htmla3, htmla4, htmla5, htmla6, htmla7};
// IP
const char htmlb0[] PROGMEM = "\"><input id=\"T2\" type=\"hidden\" name=\"DT1\"><input id=\"T4\" type=\"hidden\" name=\"DT2";
const char htmlb1[] PROGMEM = "\"><input id=\"T6\" type=\"hidden\" name=\"DT3\"><input id=\"T8\" type=\"hidden\" name=\"DT4";
const char htmlb2[] PROGMEM = "\"><input id=\"T10\" type=\"hidden\" name=\"DT5\"><input id=\"T12\" type=\"hidden\" name=\"D";
const char htmlb3[] PROGMEM = "T6\"></td></tr><tr><td style=\"width: 64px;\">IP ADR:</td><td><input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT7\" value=\"";
const char htmlb4[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT8\" value=\"";
const char htmlb5[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT9\" value=\"";
const char htmlb6[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT10\" value=\"";
PROGMEM const char * const string_table2[] = {htmlb0, htmlb1, htmlb2, htmlb3, htmlb4, htmlb5, htmlb6};
// SUBNET
const char htmlc0[] PROGMEM = "\"></td></tr><tr></tr><tr><td>SUBNET:</td><td><input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT11\" value=\""; //DT11
const char htmlc1[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT12\" value=\"";
const char htmlc2[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT13\" value=\"";
const char htmlc3[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT14\" value=\"";
PROGMEM const char * const string_table3[] = {htmlc0, htmlc1, htmlc2, htmlc3};
// Gateway
const char htmld0[] PROGMEM = "\"></td></tr><tr><td>GATEWAY:</td><td><input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT15\" value=\"";
const char htmld1[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT16\" value=\"";
const char htmld2[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT17\" value=\"";
const char htmld3[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT18\" value=\"";
PROGMEM const char * const string_table4[] = {htmld0, htmld1, htmld2, htmld3};
// MQTT Server
const char htmlf0[] PROGMEM = "\"></td></tr><tr><td>MQTT:</td><td><input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT19\" value=\"";
const char htmlf1[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT20\" value=\"";
const char htmlf2[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT21\" value=\"";
const char htmlf3[] PROGMEM = "\">.<input type=\"text\" size=\"2\" maxlength=\"3\" name=\"DT22\" value=\"";
const char htmlf4[] PROGMEM = "\"></td></tr><tr><td><br></td></tr><tr><td><input id=\"button1\"type=\"submit\" value=\"SUBMIT\" ";
const char htmlf5[] PROGMEM = "></td></tr></form></table></body></html>";
PROGMEM const char * const string_table5[] = {htmlf0, htmlf1, htmlf2, htmlf3, htmlf4, htmlf5};
//
// Compose all 

const char htmle0[] PROGMEM = "Onclick=\"document.getElementById('T2').value ";
const char htmle1[] PROGMEM = "= hex2num(document.getElementById('T1').value);";
const char htmle2[] PROGMEM = "document.getElementById('T4').value = hex2num(document.getElementById('T3').value);";
const char htmle3[] PROGMEM = "document.getElementById('T6').value = hex2num(document.getElementById('T5').value);";
const char htmle4[] PROGMEM = "document.getElementById('T8').value = hex2num(document.getElementById('T7').value);";
const char htmle5[] PROGMEM = "document.getElementById('T10').value = hex2num(document.getElementById('T9').value);";
const char htmle6[] PROGMEM = "document.getElementById('T12').value = hex2num(document.getElementById('T11').value);\"";

PROGMEM const char *const string_table6[] = {htmle0, htmle1, htmle2, htmle3, htmle4, htmle5, htmle6};

const char htmll1[] PROGMEM ="<html><body marginwidth\"0\" marginheight=\"0\" topmargin=\"0\" leftmargin=\"0\" style=\"margin: 0; padding: 0;\">";
const char htmll2[] PROGMEM ="<table bgcolor=\"#999999\" border=\"0\" width=\"100%\" cellpadding=\"1\">";
const char htmll3[] PROGMEM ="<tr><td><font color=\"white\" size=\"2\" face=\"Verdana\">&nbsp Arduino Ethernet Shield setup page</font></td></tr></table><br>";
const char htmll4[] PROGMEM ="<table border=\"0\" width=\"100%\"><tr><td width=110px>&nbsp</td><td width=200px><style>pre {font-size:8pt; letter-spacing:2px; line-height:8pt; font-weight:bold;}</style>";
const char htmll5[] PROGMEM ="<pre><font color=\"#00979d\">";
const char htmll6[] PROGMEM ="<br>    ###      ###   TM ";
const char htmll7[] PROGMEM ="<br>  ##   ##  ##   ##  ";
const char htmll8[] PROGMEM ="<br> ##     ####  #  ## ";
const char htmll9[] PROGMEM ="<br> #  ###  ##  ### ## ";
const char htmll10[] PROGMEM ="<br> ##     ####  #  ## ";
const char htmll11[] PROGMEM ="<br>  ##   ##  ##   ##  ";
const char htmll12[] PROGMEM ="<br>   #####    #####  ";
const char htmll13[] PROGMEM ="</font></pre></td><td>&nbsp</td></tr><tr><td width=110px>&nbsp</td>";
const char htmll14[] PROGMEM ="<td width=200px><font color=\"#00979d\" size=\"6\" face=\"Verdana\"><strong> ARDUINO</strong></font></td><td>&nbsp</td></tr></table><br>";
const char htmll15[] PROGMEM ="<table bgcolor=\"#00979d\" border=\"0\" width=\"100%\" cellpadding=\"3\"><tr><td width=105px></td>";

PROGMEM const char * const string_table7[] = {htmll1, htmll2, htmll3, htmll4, htmll5, htmll6, htmll7, htmll8, htmll9, htmll10, htmll11, htmll12, htmll13, htmll14, htmll15};

const char htmlm0[] PROGMEM ="</strong></em></td></tr><tr>";   
const char htmlm1[] PROGMEM ="<td style=\"width: 260.5px; text-align: right;\"><em><strong>Where /POWER1 =&nbsp;1</strong></em></td> ";
const char htmlm2[] PROGMEM ="<td style=\"width: 260.5px; text-align: left;\"><em><strong>&nbsp;  - ON</strong></em></td></tr><tr>";
const char htmlm3[] PROGMEM ="<td style=\"width: 251.5px; text-align: right;\"><em><strong>Where /POWER1 = 0</strong></em></td> ";
const char htmlm4[] PROGMEM ="<td style=\"width: 224.5px; text-align: left;\"><em><strong>&nbsp;  - OFF</strong></em></td></tr><tr> "; // </tr><tr>
const char htmlm5[] PROGMEM ="<td style=\"width: 251.5px; text-align: right;\"><em><strong>Where /POWER1 = 2</strong></em></td> ";
const char htmlm6[] PROGMEM ="<td style=\"width: 224.5px; text-align: left;\"><em><strong>  - Toggle</strong></em></td></tr><tr> ";
const char htmlm7[] PROGMEM ="<td style=\"width: 251.5px; text-align: right;\"><em><strong>Status of relay is reported on :</strong></em></td> ";
const char htmlm8[] PROGMEM ="<td style=\"width: 224.5px; text-align: left;\"><em><strong>";
PROGMEM const char * const string_table9[] = {htmlm0,htmlm1, htmlm2, htmlm3, htmlm4, htmlm5, htmlm6, htmlm7, htmlm8};

const char html_n0 [] PROGMEM =" %</strong></em></p> ";
const char html_n1 [] PROGMEM ="<p style=\"text-align: center;\"><em><strong>Topics for MQTT Server</strong></em></p> ";
const char html_n2 [] PROGMEM ="<table style=\"height: 210px; margin-left: auto; margin-right: auto; width: 490px;\"><tbody><tr> ";
const char html_n3 [] PROGMEM ="<td style=\"width: 251.5px; text-align: right;\"><em><strong>General info&nbsp;Topic :</strong></em></td>";
const char html_n4 [] PROGMEM ="<td style=\"width: 224.5px; text-align: left;\"><em><strong>";
PROGMEM const char * const string_table8[] = {html_n0,html_n1, html_n2, html_n3, html_n4};


const char html_a0 [] PROGMEM ="</strong></em></td></tr><tr>"; 
const char html_a1 [] PROGMEM ="<td style=\"width: 251.5px; text-align: right;\"><em><strong>Status of Input Pins :</strong></em></td> ";
const char html_a2 [] PROGMEM ="<td style=\"width: 224.5px; text-align: left;\"><em><strong>";
PROGMEM const char * const string_table10[] = {html_a0,html_a1, html_a2};

const char html_b0 [] PROGMEM ="</strong></em></td></tr><tr>"; 
const char html_b1 [] PROGMEM ="<td style=\"width: 251.5px; text-align: right;\"><em><strong>Last will and testament :</strong></em></td> ";
const char html_b2 [] PROGMEM ="<td style=\"width: 224.5px; text-align: left;\"><em><strong>";
PROGMEM const char * const string_table11[] = {html_b0,html_b1, html_b2};

const char html_c0 [] PROGMEM ="</strong></em></td></tr><tr>"; 
const char html_c1 [] PROGMEM ="<td style=\"width: 300.5px; text-align: right;\"><em><strong>Main topic can be changed at :</strong></em></td> ";
const char html_c2 [] PROGMEM ="<td style=\"width: 224.5px; text-align: left;\"><em><strong>";
PROGMEM const char * const string_table12[] = {html_c0,html_c1, html_c2};     

//const char html_d0 [] PROGMEM ="</strong></em></td></tr></tbody></table>"; 
//const char html_d1 [] PROGMEM ="<p style=\"text-align: center;\"><strong><span style=\"color: #ff0000;\">NOTE THE MAIN TOPIC MUST BE UNIQUE</span></strong></p>";
      // Relay Inout Tables 
//const char html_d2 [] PROGMEM ="<p style=\"text-align: center;\"><strong><span style=\"color: #052303;\">Relay Status</span></strong></p>";
//const char html_d3 [] PROGMEM ="<table style=\" margin-left: auto; margin-right: auto\"><tbody>";
//const char html_d4 [] PROGMEM ="<tr><td>Relay 1:</td><td>";  
//PROGMEM const char * const string_table13[] = {html_d0,html_d1};  //, html_d2, html_d3, html_d4}; 

const char html_e0 [] PROGMEM ="</strong></em></td></tr><tr>"; 
const char html_e1 [] PROGMEM ="<td style=\"width: 251.5px; text-align: right;\"><em><strong>Custom Links Reported at :</strong></em></td> ";
const char html_e2 [] PROGMEM ="<td style=\"width: 224.5px; text-align: left;\"><em><strong>";
PROGMEM const char * const string_table14[] = {html_e0,html_e1, html_e2};

const char html_f0 [] PROGMEM ="</strong></em></td></tr><tr>"; 
const char html_f1 [] PROGMEM ="<td style=\"width: 251.5px; text-align: right;\"><em><strong>Custom Links Commands at :</strong></em></td> ";
const char html_f2 [] PROGMEM ="<td style=\"width: 224.5px; text-align: left;\"><em><strong>";
PROGMEM const char * const string_table15[] = {html_f0,html_f1, html_f2};

const char html_g0 [] PROGMEM ="</strong></em></td></tr></tbody></table>"; 
const char html_g1 [] PROGMEM ="<p style=\"text-align: center;\"><strong><span style=\"color: #ff0000;\">Refer to Manual for full set of commands</span></strong></p>";
PROGMEM const char * const string_table16[] = {html_g0,html_g1};  //, html_d2, html_d3, html_d4}; 




const byte ID = 0x92; //used to identify if valid data in EEPROM the "know" bit, 
// if this is written in EEPROM the sketch has ran before
// We use this, so that the very first time you'll run this sketch it will use
// the values written above. 
// defining which EEPROM address we are using for what data

void setup(){
//#########

for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    digitalWrite(relayPins[thisPin], HIGH);
    pinMode(relayPins[thisPin], OUTPUT);
  }

for (int i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].attach( BUTTON_PINS[i] , INPUT_PULLUP  );       //setup the bounce instance for the current button
    buttons[i].interval(25);              // interval in ms
  }
 
  pinMode(13, OUTPUT);
  delayTimers();
  toggleLED2 ();
  //###########
  Serial.begin(57600);
  Serial.println("Read Main Topic Setup");
  readEEOROM (55);
  Serial.println("Start Ethernet .....");
  ShieldSetup (); //Setup the Ethernet shield
  server.begin(); //starting the server
  client.setServer(mqttserver, 1883);
  client.setCallback(callback);
  //delay(1500);
 // lastReconnectAttempt = 0; 
overide = 0 ;
analogSw = 0 ;
update_sw (201);  // Get the switch info 
showIPAddress();
readLinks (); // Chech to see if liks is shown

}



 
void loop(){
  // check for updates
//  ArduinoOTA.poll();  


 if (!client.connected()) {
  long now = millis();
  overide = 1 ;
    if (now - lastReconnectAttempt > 10000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      Serial.println("Conneting .....");
      toggleLED () ;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  }else {
  client.loop();
   // Publish web page
  }
  read_input_pins ();
  analog () ; //Analog sensor can be placed on pin A12 to measure souns levels , could be used as a general alarm or klap sensor. 
  dht11 () ;
  webpage ();
}
