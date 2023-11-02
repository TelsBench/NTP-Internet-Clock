#include <Arduino.h>
#include "SoftAP.h"
#include <WebServer.h>
#include <WebSocketsServer.h>

WebServer webServer(80);
WebSocketsServer webSocketServer = WebSocketsServer(81); 
//These two global vars are requied to handle the loop 
bool client0Connected=false;
bool sentDefaultsAlready= false;
bool requestReboot = false;


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
      Serial.println("Client " + String(num) + " disconnected");
      break;
    case WStype_CONNECTED:
      Serial.println("Client " + String(num) + " connected");
      client0Connected=true;
      break;
    case WStype_TEXT:   
      Serial.println("Im Receiving Text");
      for (int i=0; i<length; i++) {                  // print received data from client
        Serial.print((char)payload[i]);
      }
      //This is where we need to store the Received config. Then we need to issue a reboot requested.
      //requestReboot=true;

      Serial.println("");
      break;

      case WStype_ERROR :
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

      default:
      Serial.println("UNKNONW ERROR");
      break;
  }

}

void SoftAP::StartServer()
{
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");

  WiFi.softAP("ESP32-Clock-Server","12345678");
  IPAddress local_ip(192,168,4,1);
  IPAddress gateway(192,168,4,1);
  IPAddress subnet(255,255,255,0);
  WebServer webserver(80);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  DebugMessage("\nAP IP address: ");
  DebugMessage(WiFi.softAPIP().toString(),true);
  webServer.on("/",[](){
    webServer.send(200,"text/html",rootSettingsPage);
  });
  
  webServer.begin();
  webSocketServer.begin();
  webSocketServer.onEvent(webSocketEvent);

  DebugMessage("WEB Server Started",true);
  int count;
  String str = String (testJsonConfig);

   
  while(!requestReboot)
  {
    webServer.handleClient();
    webSocketServer.loop();

    if(sentDefaultsAlready==false && client0Connected)
    {
      String str = String(testJsonConfig);  
      int str_len = str.length() + 1;               
      char char_array[str_len];
      str.toCharArray(char_array, str_len);  
      webSocketServer.broadcastTXT(char_array); 
      sentDefaultsAlready=true;
      Serial.println("SentDefaultsAlready : " + String(sentDefaultsAlready));
    }

  }

};