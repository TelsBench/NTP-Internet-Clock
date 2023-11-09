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
bool MyConfig::ConfigExists(){
 return ConfigNumber==EEPROM.readInt(0);
}

bool MyConfig::HasCredentials()
{
   return (this->GetSSID()!="" && this->GetSSIDPassword()!="" );
}

void MyConfig::CreateDefaultConfig()
 {
      Serial.println("CREATING DEFAULT CONFIG");

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
    jsonDoc.clear();
    deserializeJson(jsonDoc, eepromStream);
    String  output;
    serializeJson(jsonDoc,output);
    return  output;
};

bool MyConfig::SetConfig(uint8_t* buffer,size_t length)
{
    //Put the received jsonString into the document.
    Serial.println("** SETTING CONFIG **");
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



