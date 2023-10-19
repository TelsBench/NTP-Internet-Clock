#include <MyConfig.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

//Default Constructor
 MyConfig::MyConfig()
 {



 };

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
bool MyConfig::ConfigExists(){
 return ConfigNumber==EEPROM.readInt(0);
}
void MyConfig::CreateDefaultConfig()
 {
      Serial.println("CREATING DEFAULT CONFIG");
      if(EEPROM.read(configEpromAddress)==0xFF)
      {
        jsonDoc[Keys_SETUP_PASSWORD] = "biteme";       
        jsonDoc[Keys_SSID] = "BTHub6-6GWX";
        jsonDoc[Keys_SSID_PASSWORD] = "KadLn3rJwX6r";
        jsonDoc[Keys_NTP_SERVER1] = "europe.pool.ntp.org";     
        jsonDoc[Keys_NTP_INTERVAL_MILLIS] = 86400000;        
        jsonDoc[Keys_TIMEZONE_OFFSET_SECONDS] = 0;
        jsonDoc[Keys_DST_OFFSET_SECONDS] = 3600;    
        jsonDoc[Keys_VersionNumber]="1.0";      

        WriteConfigToEEPROM(); 

      }

 };

void MyConfig::WriteConfigToEEPROM()
{
    Serial.println("WRITING JSON DOC TO EEPROM");
    EepromStream eepromStream(EEPROM_OFFSET, EEPROM.length());
    serializeJson(jsonDoc, eepromStream);

    EEPROM.commit();
    this->ReadConfigFromEEPROM();
};
 String MyConfig::ReadConfigFromEEPROM()
{
    Serial.println("READING EEPROM INTO JSON DOC");
    EepromStream eepromStream(EEPROM_OFFSET, EEPROM.length());
    deserializeJson(jsonDoc, eepromStream);

    String  output;
    serializeJson(jsonDoc,output);

    return  output;

};

//Debugging Methods
void MyConfig::DisplaySettings()
{
    Serial.println("");
    Serial.println("Dumping contents of EEPROM");
    Serial.println("==========================");
    Serial.println(String("SSID........................: ") + GetSSID());
    Serial.println(String("SSID Password.............. : ") + GetSSIDPassword());
    Serial.println(String("Setup Password..............: ") + GetSetupPassword());
    Serial.println(String("NTPServer...................: ") + GetNTPServer());
    Serial.println(String("TimeZone Offset MilliSeconds: ") + GetTimeZoneOffsetSeconds());
    Serial.println(String("DST Offset Seconds..........: ") + GetDstOffsetSeconds());
    Serial.println(String("NTP Servers Interval Seconds: ") + GetNtpIntervalMillis());
     Serial.println(String("Version Number.............: ") + GetVersionNumber());
};
void MyConfig::DumpEEPROM(int len)
{
    Serial.println("DUMPING CONTENTS OF EEPROM");
    for( int x=EEPROM_OFFSET; x<len; x++)
    {
        Serial.print( char(EEPROM.readChar(x)) );
    }
};

//Property Accessors
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
int    MyConfig::GetTimeZoneOffsetSeconds()
{
 return this->jsonDoc[Keys_TIMEZONE_OFFSET_SECONDS];
};
int MyConfig::GetDstOffsetSeconds()
{
 return this->jsonDoc[Keys_DST_OFFSET_SECONDS];
};
long MyConfig::GetNtpIntervalMillis()
{
  return this->jsonDoc[Keys_NTP_INTERVAL_MILLIS];
}
const char* MyConfig::GetSetupPassword()
{
  return this-> jsonDoc[Keys_SETUP_PASSWORD];
}
const char* MyConfig::GetVersionNumber()
{
  return this-> jsonDoc[Keys_VersionNumber];
}



