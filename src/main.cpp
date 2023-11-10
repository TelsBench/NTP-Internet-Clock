/*
MIT License

Copyright (c) [2023] [TEL at @TELsBench channel on youtube]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <WiFi.h>       
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Myhelpers.h"
#include "MyDisplay.h"
#include "MyConfig.h"
#include "SoftAP.h"
#include <WebSockets.h>

//private lib declarations under the project directory hence the quote marks for a local search.
MyHelpers myHelpers;
MyDisplay myScreen;
MyConfig config;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#define DST_SWITCH  GPIO_NUM_19 // Daylight Saving Switch input GPIO Pin.
#define DO_CONFIG   GPIO_NUM_15 // Press before Power On/Reset, clock will go into SoftAP mode and allow config via a web browser.

// Main Loop Control
bool firstTime = true;

//Colon Only Appears every half second
bool showColon = true;

//LastKnownDSTSwitch
int  LastKnownDSTSWITCH=LOW;


bool GetConncted(){

  const int GETTING_CONNECTED_MAX_TRIES=30;
  int CONNECT_TRY_COUNT =0;
  bool isConnected = false;
  String hostname = "TELs ESP32 CLOCK";
  WiFi.setHostname(hostname.c_str());
  WiFi.begin(config.GetSSID(), config.GetSSIDPassword());  
   
  while(CONNECT_TRY_COUNT++<GETTING_CONNECTED_MAX_TRIES){
   int count=0;
   myScreen.displayConnectStatus(myHelpers.string2char("Trying to connect..."));
   delay (1000 ); 
   if( WiFi.status() == WL_CONNECTED) 
   { 
      myScreen.displayConnectStatus(myHelpers.string2char("You are Connected..."));
      delay (1000 ); 
      isConnected = true;
      break;
   }
  }

  return isConnected; 
}

void setup(void) {

   //External 10K pullup resistors may be required
   pinMode(DO_CONFIG,INPUT_PULLUP);
   pinMode(DST_SWITCH,INPUT_PULLUP); 

  //Enables debugging print statements
   Serial.begin(115200);
   EEPROM.begin(1024);
   
   //When power applied if config button depressed, clock will go into softAP mode for config from web browser
   //This is to prevent this going back into config at the end of a config session as wel check the button here.
   delay(1000);
   bool doRemoteConfig = !digitalRead(DO_CONFIG);
   if(doRemoteConfig || !config.ConfigExists() || !config.HasCredentials()){

      //Only clear the EEPROM if there is no config. or no credentials.
      if(doRemoteConfig==false)
      {
         Serial.println("** Creating A default config **");
         config.ClearEEPROM();
         config.CreateDefaultConfig();
         config.WriteConfigToEEPROM(); 
      }

      SoftAP softserver(myScreen);
      //This is important as we are passing the router credentials over the air.
      WiFi.setMinSecurity(WIFI_AUTH_WPA2_WPA3_PSK); 
      softserver.StartServer();
   }
   
   //Retreive Settinsg fomr EEPROM for use
   config.ReadConfigFromEEPROM();
   
   //Setup Screen.
   myScreen.tftSetup();
   myScreen.clearScreen();

   //Display the DST Switch Position
   int switchValue = digitalRead(DST_SWITCH);
   String message="";
   if(switchValue==HIGH)
   {
      message="DST SWITCH SET = ON.";
      myScreen.displayConnectStatus(myHelpers.string2char(message));
   }
   else
   {
      message = "DST SWITCH SET = OFF.";
      myScreen.displayConnectStatus(myHelpers.string2char(message));
   }

   //Attempt Connection.
   if(!GetConncted()) 
   {
      Serial.println("**FATAL ERROR** - COULD NOT GET CONNECTED ");
      myScreen.displayConnectStatus(myHelpers.string2char("Connect Failed"));
      //Loop forever.
      while(1){};
   }
   else
   {
      //Success - tell the user the good news , 'Connected'
      myScreen.displayConnectStatus(myHelpers.string2char("WIFI Connected."));
      NTPClient timeClient(ntpUDP,config.GetNTPServer());
      timeClient.begin();
   }
}

//Winter may need an offset depending on location.
//DST Pin will be HIGH for summer time ( no added offset )
void DSTCHECK(){
   myScreen.clearScreen();
   int currentDSTSwitchState =  digitalRead(DST_SWITCH);   
   long totalOffsetSeconds = config.GetDstOffsetSeconds() + config.GetTimeZoneOffsetSeconds();

   //Display DST Switch position.
   if( digitalRead(DST_SWITCH)==HIGH) 
   {
      myScreen.displayConnectStatus(myHelpers.string2char("Daylight savings-ON"));
      timeClient.setTimeOffset(totalOffsetSeconds); 
   }
   else
   {
      myScreen.displayConnectStatus(myHelpers.string2char("Daylight savings-OFF"));
      timeClient.setTimeOffset(config.GetTimeZoneOffsetSeconds()); 
   }
   LastKnownDSTSWITCH = currentDSTSwitchState;

   //Force an update which includes sync with NTPServer.
   timeClient.forceUpdate();
   delay(4000);

   myScreen.clearScreen();
   //Epoch Time is the time in seconds since Jan 1st 1970 required for screen time update.
   myScreen.updateScreen(timeClient.getEpochTime(),timeClient.getDay(),showColon); 
}

void loop() 
{

  //Quick Press Results in Rest LONG Press results in Reset AND Config
  if(digitalRead(GPIO_NUM_15)==LOW)
  {
    esp_restart();
    while(1){};
  }

  //DSTCHECK on Startup or when Position Changes of DST Swtich.
  if( firstTime || (LastKnownDSTSWITCH != digitalRead(DST_SWITCH))) 
  { 
    DSTCHECK(); 
    firstTime=false; 
  }

  //Only updates if the interval for update has been passed, normally 24 hours.
  timeClient.update();

  //Update Screen Date and Time.
  myScreen.displayTimeString( myHelpers.string2char(myHelpers.getTimeStampString(timeClient.getEpochTime(),showColon))); 
  myScreen.displayDateInfoString( myHelpers.string2char(myHelpers.getDateInfoString(timeClient.getEpochTime(),timeClient.getDay())));

  //Toggle the colon.
  showColon=!showColon;
  delay(500);

 }
