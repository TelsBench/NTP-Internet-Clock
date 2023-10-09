#include <MyConfig.h>
#include <ArduinoJson.h>
#include <EEPROM.h>


/* 
    My Config Class
*/
 MyConfig::MyConfig(){

       //Simuates a freshly blown chip with no config
       EEPROM.begin(1024);
       for(int j=0; j< 255; j++)
       {
            EEPROM.write(j,0xFF);
       }    
       EEPROM.commit();       
       
      //Actual real life what happens now if we had no config.
      if(EEPROM.read(configEpromAddress)==0xFF){
        // jsonDoc["ssid"] = "BTHub6-6GWX";
        // jsonDoc["password"] = "KadLn3rJwX6r";

        jsonDoc["ssid"]                   = "";
        jsonDoc["password"]               = "";
        jsonDoc["pin"]                    = "";
        jsonDoc["ntpServer1"]             = "";
        jsonDoc["timeZoneOffsetSeconds"]  = "";
        jsonDoc["dstOffsetSeconds"]       = 0;   

        writeConfigToEEPROM(); 

     }

 }


void MyConfig::dumpContentsOfEEPROM( bool displayHEX){
    Serial.println("");
    Serial.println("Dumping contents of EEPROM");
    Serial.println("==========================");
    for( int z=0; z<144;z++){

        Serial.print(" ");
        if( z%16==0 ) Serial.println(" ");

        u_int8_t val = EEPROM.read(z);
        if(displayHEX==false)
        {
          Serial.print(char(val)); 
        }
        else
        {
          Serial.print(val,HEX);
        }
        //line break.
   
    }

    Serial.println("");
}

void MyConfig::writeConfigToEEPROM()
{
  EEPROM.begin(1024);
  String serialisedJson=String("");
  serializeJson(jsonDoc,serialisedJson);
  Serial.println("Serialized JSON DOC = " + serialisedJson);

  //Write length to the EEPROM.
  EEPROM.write(configEpromAddress, serialisedJson.length());
  for (unsigned int i = 0; i < serialisedJson.length(); i++) EEPROM.write( configEpromAddress + 1 + i  , serialisedJson[i]);
  EEPROM.commit();

}

bool MyConfig::readConfigFromEEPROM()
{

  //0xFF retruned from address 0 means it has never been written to. So return false;
  u_int8_t configJsonStringLength = EEPROM.read(0);

  char data[configJsonStringLength + 1];
  for (int i = 0; i < configJsonStringLength; i++)
  {
    data[i] = EEPROM.read( configEpromAddress + 1 + i);
  }
  data[configJsonStringLength] = '\0'; 

  DeserializationError  error  =  deserializeJson(jsonDoc,data);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return false;
  }

  return  true;
}

bool MyConfig::SetConfig(String configString){

    DeserializationError error =  deserializeJson(this->jsonDoc,configString);
    if (error) {
     Serial.println(F("**ERROR** deserializeJson() failed: "));
     Serial.println(configString);
     Serial.println(error.f_str());
     return false;
   }
    
    writeConfigToEEPROM();
    readConfigFromEEPROM();
    return true;

}

const char*  MyConfig::GetSSID(){
 return this->jsonDoc["ssid"];
}
const char*  MyConfig::GetPassword(){
 return this->jsonDoc["password"];
}
 const char*  MyConfig::GetNTPServer(){
 return this->jsonDoc["ntpServer1"];
}

const char*  MyConfig.GetPin(){

  return this->jsonDoc["pin"];

}

int    MyConfig::GetDstOffsetSeconds(){
 return this->jsonDoc["timeZoneOffsetSeconds"];
}

int    MyConfig::GetTimeZoneOffsetSeconds(){
 return this->jsonDoc["timeZoneOffsetSeconds"];
}



        // jsonDoc["ssid"]                   = "";
        // jsonDoc["password"]               = "";
        // jsonDoc["pin"]                    = "";
        // jsonDoc["ntpServer1"]             = "";
        // jsonDoc["timeZoneOffsetSeconds"]  = "";
        // jsonDoc["dstOffsetSeconds"]       = 0; 




