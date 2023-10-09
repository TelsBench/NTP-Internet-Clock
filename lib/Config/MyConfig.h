#ifndef MYCONFIG_H
#define ST773MYCONFIG_H
#include <ArduinoJson.h>
#include <WiFiUdp.h>

class MyConfig{

    private:
    StaticJsonDocument<1024> jsonDoc;
    void writeConfigToEEPROM();
    bool readConfigFromEEPROM();
    const int configEpromAddress = 0;

    public:
    MyConfig();
    const char*  GetSSID();
    const char*  GetPassword();
    const char*  GetNTPServer();
    const char*  GetPin();
    int          GetDstOffsetSeconds();
    int          GetTimeZoneOffsetSeconds();
    const char*  GetNtpServer();

    bool SetConfig(String configString);
    void dumpContentsOfEEPROM(bool displayHEX);

};

#endif