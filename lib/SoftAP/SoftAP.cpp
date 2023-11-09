#include <Arduino.h>
#include "SoftAP.h"
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "MyConfig.h"
#include "MyDisplay.h"

WebServer webServer(80);
WebSocketsServer webSocketServer = WebSocketsServer(81); 
MyConfig apconfig;

//These global vars are requied to handle the loop 
bool client0Connected=false;
bool sentDefaultsAlready= false;
bool requestReboot = false;

//Constructor : the myDisplay is passed into the class for use.
SoftAP::SoftAP(MyDisplay &mydisplay)
{
    myScreen= mydisplay;
    mydisplay.tftSetup();
}

void SoftAP::DebugMessage( String msg, bool newline)
{  
    Serial.println(msg);
    if(newline) Serial.println("");
};

void  SoftAP::SendSocketEvent(String event)
{
    webSocketServer.broadcastTXT(event);
    sentDefaultsAlready=false;
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {   
  switch (type) 
  { 

  case WStype_DISCONNECTED:
    break;

  case WStype_CONNECTED:
    client0Connected=true;
    break;

  case WStype_TEXT:   
    apconfig.SetConfig(payload,length);
    apconfig.DisplaySettings();
    apconfig.DumpEEPROM(500);
    requestReboot=true;
    break;

  case WStype_ERROR:
    Serial.println("ERROR");
    break;

  case WStype_BIN:
    Serial.println("BIN");
    break;

  case WStype_PING:
  case WStype_PONG:
  case WStype_FRAGMENT:
  case WStype_FRAGMENT_FIN:
  case WStype_FRAGMENT_BIN_START:
  case WStype_FRAGMENT_TEXT_START:
    break;

  default:
    break;

  }
}

void SoftAP::StartServer()
{ 
  // Connect to Wi-Fi network with SSID and password
  WiFi.softAP(SOFT_ROUTER_ID,SOFT_ROUTER_PASSWORD);
  IPAddress local_ip(192,168,4,1);
  IPAddress gateway(192,168,4,1);
  IPAddress subnet(255,255,255,0);
  WebServer webserver(80);

  WiFi.softAPConfig(local_ip, gateway, subnet);
  myScreen.displayRemoteConfig(local_ip.toString());

  DebugMessage("\nAP IP address: ");
  DebugMessage(WiFi.softAPIP().toString(),true);
  webServer.on("/",[](){
    sentDefaultsAlready=false;
    webServer.send(200,"text/html",rootSettingsPage);
  });
  
  webServer.begin();
  webSocketServer.begin();
  webSocketServer.onEvent(webSocketEvent);

  DebugMessage("WEB Server Started",true);
  int count;
  apconfig.DisplaySettings();
  apconfig.DumpEEPROM(500);
   
  //This Loop is dependent in its operation on the condition of the client connection
  //and when it receives and stored the data from the front end. Only when that is
  //complete, can it provide the user with some instructions to reboot the clock.
  //This is prefereable to a simple reboot where confusion may result otherwise.
  while(!requestReboot)
  {
    webServer.handleClient();
    webSocketServer.loop();

    if(sentDefaultsAlready==false && client0Connected)
    {
      String str = apconfig.ReadConfigFromEEPROM(); 
      Serial.println("RETREIVED CONFIG :" + str); 
      int str_len = str.length() + 1;               
      char char_array[str_len];
      str.toCharArray(char_array, str_len);  
      webSocketServer.broadcastTXT(char_array); 
      sentDefaultsAlready=true;
      Serial.println("SentDefaultsAlready : " + String(sentDefaultsAlready));
    }

  }

  //Display reboot instructions then Wait intil user has read the message on screen 
  //and pressed the Config Button to retart the clock.
  //
  //** Note a Pullup Resistor on GPIO_NUM_15 May be required even if INPUT_PULLUP is used
  //when using a AZ-Devkit ESP32 for Mode Setting of the pin, otherwise It appears to float 
  //causing unpredicatble behaviour.
  myScreen.displayRebootInfo();
  while(digitalRead(GPIO_NUM_15)!=LOW){};
  esp_restart();
  while(1){};

};