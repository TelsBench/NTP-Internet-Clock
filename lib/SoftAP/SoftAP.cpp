#include <Arduino.h>
#include "SoftAP.h"
#include <WebServer.h>

WebServer webServer;


void SoftAP::DebugMessage( String msg, bool newline)
{  
   Serial.println(msg);
   if(newline) Serial.println("");
};


void SoftAP::StartServer()
{
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP("ESP32-Clock-Server","12345678");
  IPAddress local_ip(192,168,1,1);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  WebServer webserver(80);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  DebugMessage("\nAP IP address: ");
  DebugMessage(WiFi.softAPIP().toString(),true);
  webServer.on("/",[](){
    webServer.send(200,"html",rootSettingsPage);
  });

  webServer.begin();
  DebugMessage("WEB Server Started",true);
  while(1)
  {
   webServer.handleClient();
  }
 
};

void SoftAP::handle_NotFound(){
  webServer.send(404, "text/plain", "Not found");
};

void handle_OnRoot(const char* page) {
 webServer.send(200, "text/plain", page);
}

void handle_OnConnect() {
 webServer.send(200, "text/plain", "I'm Here!");
}

void SoftAP::EndServer()
{
};


void SoftAP::RequestListener()
{



};