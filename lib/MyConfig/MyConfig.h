#ifndef MYCONFIG_H
#define MYCONFIG_H
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <StreamUtils.h>

     
#define Keys_SSID "ssid"
#define Keys_SSID_PASSWORD "sspassword"
#define Keys_NTP_SERVER1  "ntpurl"
#define Keys_NTP_INTERVAL_MILLIS  "ntpintervalmillis"       
#define Keys_TIMEZONE_OFFSET_SECONDS  "timezoneoffsetseconds"
#define Keys_DST_OFFSET_SECONDS  "dstoffsetseconds" 
#define Keys_VersionNumber  "versionnumber"  


#define ConfigNumber  2012709
#define EEPROM_OFFSET 10

class MyConfig
{

    private:
    StaticJsonDocument<1024> jsonDoc;

    const int configEpromAddress = 0;

    public:
    MyConfig();
    void   WriteConfigToEEPROM();
    String  ReadConfigFromEEPROM();
    void          CreateDefaultConfig();
    void          ClearEEPROM();
    const char*   GetSSID();
    const char*   GetSSIDPassword();
    const char*   GetNTPServer();
    int           GetTimeZoneOffsetSeconds();
    int           GetDstOffsetSeconds();
    long          GetNtpIntervalMillis();
    const char*   GetVersionNumber();
    bool          SetConfig(uint8_t* buffer, size_t length);
    void          DisplaySettings();
    void          DumpEEPROM(int len);
    bool          ConfigExists();
    bool          HasCredentials();

};

#endif