#ifndef MYCONFIG_H
#define MYCONFIG_H
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <StreamUtils.h>

#define Keys_SETUP_PASSWORD "SetupPassword"       
#define Keys_SSID "SSID"
#define Keys_SSID_PASSWORD "SSPassword"
#define Keys_NTP_SERVER1  "NTPServer"
#define Keys_NTP_INTERVAL_MILLIS  "ntpIntervalMillis"       
#define Keys_TIMEZONE_OFFSET_SECONDS  "timeZoneOffsetSeconds"
#define Keys_DST_OFFSET_SECONDS  "dstOffsetSeconds" 
#define Keys_VersionNumber  "VersionNumber"    

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
    const char*   GetSetupPassword();
    const char*   GetVersionNumber();
    bool          SetConfig(String configString);
    void          DisplaySettings();
    void          DumpEEPROM(int len);
    bool          ConfigExists();
    bool          HasCredentials();

};

#endif