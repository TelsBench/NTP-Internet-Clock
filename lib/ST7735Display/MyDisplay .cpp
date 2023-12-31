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

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include "MyHelpers.h"
#include "MyDisplay.h"

MyDisplay::MyDisplay(){

 tft = new ::Adafruit_ST7735(TFT_CS,TFT_DC,TFT_RST);

}

void MyDisplay::clearScreen(){

    tft->fillScreen(ST7735_BLACK);
}


void MyDisplay::tftSetup(){  
 tft->initR(INITR_BLACKTAB);
 tft->setRotation(3);
 tft->fillScreen(ST7735_BLACK);
 tft->setTextWrap(false);
}

void MyDisplay::displayConnectStatus( char* status ){
  tft->fillRect(0,8,160,8,ST77XX_BLACK);
  tft->setTextColor(ST7735_GREEN, ST7735_BLACK);
  tft->setCursor(5, 8);   
  tft->setTextSize(1);
  tft->print(status);  
}

void MyDisplay::displayTimeString( char* timeStr ){
  tft->setTextColor(ST7735_RED, ST7735_BLACK);
  tft->setCursor(22, 8);   
  tft->setTextSize(4);
  tft->print(timeStr);  
}

void MyDisplay::displayDateInfoString( char* dateInfoStr ){
  tft->setTextColor(ST7735_CYAN, ST7735_BLACK);
  tft->setCursor(10, 80);   
  tft->setTextSize(2);
  tft->print(dateInfoStr);  
}

void MyDisplay::updateScreen(unsigned long epochTime, uint8_t day, bool showColon){
    String currentTime = myHelpers.getTimeStampString(epochTime,showColon);
    String  currentDateInfo = myHelpers.getDateInfoString(epochTime,day);
    displayTimeString(myHelpers.string2char(currentTime));
    displayDateInfoString(myHelpers.string2char(currentDateInfo));
}
