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

#include <Arduino.h>
#include "MyHelpers.h"

String MyHelpers::getDOMSuffix(uint8_t day){   

    if( day==11 || day==12 || day==13 ) return "th ";
    
    int remainder = (day % 10 );
    switch( remainder ){
       case  1 :
           return "st ";
       case  2 :
           return "nd ";
       case  3 :
          return "rd ";
       default :
          return "th ";
    }
}

String MyHelpers::getDateInfoString(time_t rawtime, uint8_t dayOfTheWeek) {
    struct tm * ti;
    ti = localtime (&rawtime);
    uint8_t month = ti->tm_mon ;
    uint8_t mday = ti->tm_mday;
    return weekDays[dayOfTheWeek] + " " + months[month]+ " " + String(mday) + getDOMSuffix(mday);        
}

String MyHelpers::getTimeStampString(time_t rawtime,bool showColon=true) {
    struct tm * ti;
    ti = localtime (&rawtime);
    uint8_t hours     = ti->tm_hour;
    String  hoursStr  = hours < 10 ? "0" + String(hours) : String(hours);
    uint8_t minutes   = ti->tm_min;
    String  minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);
    return  hoursStr + (showColon ? ":" : " " ) + minuteStr;
}

char * MyHelpers::string2char(String command){  
    char* p;
    if(command.length()!=0)
        p = const_cast<char*>(command.c_str());  
    else
        p = const_cast<char*>("");   
    return p;
}

