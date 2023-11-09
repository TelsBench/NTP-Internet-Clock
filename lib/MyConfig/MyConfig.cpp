#include <MyConfig.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

//Default Constructor
MyConfig::MyConfig(){};

void MyConfig::ClearEEPROM()
 {
    Serial.println("CLEARING OUT EEPROM");
    //Simuates a freshly blown chip all 0xFF's
    for(int j=0; j< 255; j++)
    {
      EEPROM.write(j,0xFF);
    }    
    EEPROM.commit();
 };

//Check if config number exists this is an Int at EEProm address Zero
bool MyConfig::ConfigExists(){
 return ConfigNumber==EEPROM.readInt(0);
}

bool MyConfig::HasCredentials()
{
   return (this->GetSSID()!="" && this->GetSSIDPassword()!="" );
}

//Default Settings for when required.
void MyConfig::CreateDefaultConfig()
 {
    //Write the ConfigNumber to the start of EEPROM as an INT.
    EEPROM.writeInt( 0,ConfigNumber);
    EEPROM.commit();
    //Create a Default Set of parameters to jsonDOC and save to EEPROM;
    jsonDoc[Keys_SSID] = "";
    jsonDoc[Keys_SSID_PASSWORD] = "";
    jsonDoc[Keys_NTP_SERVER1] = "europe.pool.ntp.org";     
    jsonDoc[Keys_NTP_INTERVAL_MILLIS] = 86400000;        
    jsonDoc[Keys_TIMEZONE_OFFSET_SECONDS] = "190,+,3600";
    jsonDoc[Keys_DST_OFFSET_SECONDS] = 3600;    
    jsonDoc[Keys_VersionNumber]="1.0";      

    WriteConfigToEEPROM(); 
 }

//Save jsonDoc config to EEPROM.
void MyConfig::WriteConfigToEEPROM()
{
    EepromStream eepromStream(EEPROM_OFFSET, EEPROM.length());
    serializeJson(jsonDoc, eepromStream);
    EEPROM.commit();
    this->ReadConfigFromEEPROM();
}

//Read from EEPROM to jsonDoc.
String MyConfig::ReadConfigFromEEPROM()
{
  EepromStream eepromStream(EEPROM_OFFSET, EEPROM.length());
  jsonDoc.clear();
  deserializeJson(jsonDoc, eepromStream);
  String  output;
  serializeJson(jsonDoc,output);
  return  output;
}

//Used to take the Socket Data and store it in the EEPOM & Read into JSON
//Happens when the data is received via the socket during web configuration.
bool MyConfig::SetConfig(uint8_t* buffer,size_t length)
{
  //Put the received jsonString into the document.
  for(int i=0; i<length;i++)
  {
    Serial.print((char)(buffer[i]));
    EEPROM.writeByte( EEPROM_OFFSET+i, buffer[i]);       
  }
  Serial.println("");
  EEPROM.writeByte( EEPROM_OFFSET + length,0);  
  EEPROM.commit();
  this->ReadConfigFromEEPROM();
  return true;
}

//Debugging Methods
void MyConfig::DisplaySettings()
{
    Serial.println("");
    Serial.println("Dumping contents of EEPROM");
    Serial.println("==========================");
    Serial.println(String("SSID........................: ") + GetSSID());
    Serial.println(String("SSID Password.............. : ") + GetSSIDPassword());
    Serial.println(String("NTPServer...................: ") + GetNTPServer());
    Serial.println(String("TimeZone Offset MilliSeconds: ") + GetTimeZoneOffsetSeconds());
    Serial.println(String("DST Offset Seconds..........: ") + GetDstOffsetSeconds());
    Serial.println(String("NTP Servers Interval Seconds: ") + GetNtpIntervalMillis());
    Serial.println(String("Version Number.............: ") + GetVersionNumber());
};

//Debugging Aid
void MyConfig::DumpEEPROM(int len)
{
    Serial.println("*** DUMPING CONTENTS OF EEPROM  ***");
    Serial.println("*** Config Number : " + String(EEPROM.readInt(0)));
    Serial.println("***********************************");
    for( int x=EEPROM_OFFSET; x<len; x++)
    {  
        Serial.print(char(EEPROM.readChar(x)) );
    }
    Serial.println("");
};

//Property Accessors used to set individual variables in Main.cpp
const char*  MyConfig::GetSSID()
{
 return this->jsonDoc[Keys_SSID];
};
const char*  MyConfig::GetSSIDPassword()
{      
 return this->jsonDoc[Keys_SSID_PASSWORD];
};
const char*  MyConfig::GetNTPServer()
 {
 return this->jsonDoc[Keys_NTP_SERVER1];
}

// This protracted acessor is required because the stored value for
// ZoneOffsetSeconds has this format.  NN/NNN,SIGN,OFFSET i.e. "50,+,10800"
// It is stored as such because the dropdown box will have duplicates of an offset value
// Therefore It has been pefixed with a unique index so the currently stored option
// can be displayed selected on page load. There is probably an easier way to code what is below
// but this works ok for now.
int  MyConfig::GetTimeZoneOffsetSeconds()
{
  String zoneStr = this->jsonDoc[Keys_TIMEZONE_OFFSET_SECONDS];
  char chArray[sizeof(zoneStr)];
  zoneStr.toCharArray(chArray, sizeof(zoneStr));
  int comma1Idx = zoneStr.indexOf(",");
  char sign = chArray[comma1Idx+1];
  int comma2Idx = comma1Idx+2;
  String zoneOffsetSecondsStr  =  zoneStr.substring(comma2Idx+1,zoneStr.length());
  int offsetSeconds = zoneOffsetSecondsStr.toInt();
  return(sign=='-' ? (-1 * offsetSeconds) : offsetSeconds);

};
int MyConfig::GetDstOffsetSeconds()
{
  return this->jsonDoc[Keys_DST_OFFSET_SECONDS];
};
long MyConfig::GetNtpIntervalMillis()
{
  return this->jsonDoc[Keys_NTP_INTERVAL_MILLIS];
}

const char* MyConfig::GetVersionNumber()
{
  return this-> jsonDoc[Keys_VersionNumber];
}



