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

//private lib declarations under the project directory hence the quote marks for a local search.
MyHelpers myHelpers;
MyDisplay myScreen;

/*   DEFINES    */
//Router
#define ssid  "SSID"
#define password "PASSWORD"

//NTP Server
#define ntpServer1 "europe.pool.ntp.org"

//Daylight Saving Offset & TimeZoneOffset Seconds
#define daylightSavingsffsetSeconds 3600 
#define timeZoneOffsetSeconds  0
#define totalOffsetSeconds  timeZoneOffsetSeconds + daylightSavingsffsetSeconds

//Period Between NTP Syncs
#define millisecondsBetweenNTPSyncs 10800000 //3 hours
#define DST_SWITCH  GPIO_NUM_0  // Daylight Saving Switch input GPIO Pin.

// Main Loop Control
bool firstTime = true;

//Colon Only Appears every half second
bool showColon = true;

//Inital State of LASTKnownDSTSwitch
int  LastKnownDSTSWITCH=LOW;

//NTP Setup
WiFiUDP ntpUDP;
NTPClient timeClient = NTPClient(ntpUDP,"",0);

/***************************************************/

bool GetConncted(){

  const int GETTING_CONNECTED_MAX_TRIES=30;
  int CONNECT_TRY_COUNT =0;
   bool isConnected = false;
   String hostname = "TELs ESP32 CLOCK";
   WiFi.setHostname(hostname.c_str());

  //Future Revisions will set this via the web
  WiFi.begin(ssid, password);  
   
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



  //Daylight Savings Switch, no need for an external pullup resitor.
  pinMode(DST_SWITCH,INPUT_PULLUP); 
  
  //Enables debugging print statements
  Serial.begin(115200);

  //Setup Screen and Config.
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
      myScreen.displayConnectStatus(myHelpers.string2char("Connected."));
   }

  //Setup the time client, Syncs with NTP Server every 3 Hours normally. which is too often, so extend to 24 hours worth of milliseconds.
  timeClient = NTPClient(ntpUDP, ntpServer1, timeZoneOffsetSeconds, millisecondsBetweenNTPSyncs); 
  timeClient.begin();

}

//During summer Offset needs to be applied to the time. IE Winter @6PM changes to 7PM after summer tranition.
//DST Pin will be HIGH for summer time ( no added offset )
void DSTCHECK(){
   myScreen.clearScreen();
   int currentDSTSwitchState =  digitalRead(DST_SWITCH);

   //Display DST Switch position.
   if( digitalRead(DST_SWITCH)==HIGH) 
   {
       myScreen.displayConnectStatus(myHelpers.string2char("Daylight savings-ON"));
      timeClient.setTimeOffset(totalOffsetSeconds); 
   }
   else
   {
       myScreen.displayConnectStatus(myHelpers.string2char("Daylight savings-OFF"));
        timeClient.setTimeOffset(timeZoneOffsetSeconds); 
   }
   LastKnownDSTSWITCH = currentDSTSwitchState;

   //Force an update which includes sync with NTPServer.
   timeClient.forceUpdate();
   delay(4000);

   myScreen.clearScreen();
   //Epoch Time is the time in seconds since Jan 1st 1970 required for screen time update.
   myScreen.updateScreen(timeClient.getEpochTime(),timeClient.getDay(),showColon); 
}


void loop() {

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
