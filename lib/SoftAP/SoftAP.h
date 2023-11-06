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
#include <WiFi.h>
#include "MyDisplay.h"
#ifndef SoftAP_h
#define SoftAP_h


#define   rootSettingsPage "<!DOCTYPE html><html> <head> <link rel='icon' href='data:;base64,iVBORw0KGgo='> <meta charset='utf-8' /> <title>ESP32TELs NTP CLOCK Settings</title> <style> html { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; } table th, td { background-color: gainsboro; font-size: 15pt; font-weight: bold; color: black; padding: 8px; } table th { font-size: 24pt; color: black; background-color: #392d2d; color: white; } table th:nth-child(1) { width: 200px; } table th:nth-child(2) { width: 300px; } table td:nth-child(2) { text-align: left; } input { width: 90%; font-size: 15px; } button { background-color: red; color: white; width: 100px; height: 40px; font-weight: bold; kerning: unset; border-radius: 15px; border-block-color: white; border-width: 4px; cursor: pointer; } td#tdButton { text-align: right; } </style> <script type='text/javascript'> var Socket; function init() { console.log('im in Init'); Socket = new WebSocket('ws://' + window.location.hostname + ':81/'); Socket.onmessage = function (event) { console.log(event.data);setData(event.data); }; }; function ShowRebootMsg() { document.getElementById('msg').style.display='table-cell'; }; function submitData() { var data = {}; data.ssid=document.getElementById('ssid').value; data.sspassword=document.getElementById('sspassword').value; data.ntpurl=document.getElementById('ntpurl').value; data.ntpintervalmillis=document.getElementById('ntpintervalmillis').value; data.timezoneoffsetseconds=document.getElementById('timezoneoffsetseconds').value; data.dstoffsetseconds=document.getElementById('dstoffsetseconds').value; data.versionnumber=document.getElementById('versionnumber').value; var jsonString = JSON.stringify(data); console.log('Sending Back this :' + jsonString); Socket.send(jsonString); ShowRebootMsg(); }; function setData(jsonString) { console.log('Set Data ' + jsonString); var jsonData = JSON.parse(jsonString); for (var key in jsonData) { if (jsonData.hasOwnProperty(key)) { console.log(key + ':' + jsonData[key]); document.getElementById(key).value = jsonData[key]; } else console.log('Error : ' + key + ' Not Found !'); }; }; window.onload = init; </script> </head> <body> <Content-type:text html> <h1 id=title>TEL&apos;s ESP32 NTP CLOCK </h1> <table id='tblSettings' style='border:Solid 1pt black;border-collapse:collapse;padding:19px'> <thead></thead> <tbody> <tr> <th>SETTING</th> <th>VALUE</th> </tr> <tr> <td>SSID</td> <td><input id='ssid' placeholder='Router ID i.e. HUB-ZJWWFGX' /></td> </tr> <tr> <td>SSPassword</td> <td><input id='sspassword' placeholder='Router password' /></td> </tr> <tr> <td>NTPServer Url</td> <td><input id='ntpurl' placeholder='i.e. europe.pool.ntp.org' /></td> </tr> <tr> <td>NTP Sync Interval</td> <td> <select id='ntpintervalmillis'> <option value='86400000'>Every 24 Hours</option> <option value='10800000'>Every 3 Hours</option> <option value='21600000'>Every 6 Hours</option> <option value='43200000'>Every 12 Hours</option> </select> </td> </tr> <tr> <td>TimeZone Offset</td> <td> <select id='timezoneoffsetseconds'> <option value='10,+,0'>GMT Standard Time (GMT 00:00)</option> <option value='20,+,16200'>Afghanistan Standard Time (GMT+04:30)</option> <option value='30,-,32400'>Alaskan Standard Time (GMT,-,09:00)</option> <option value='40,+,10800'>Arab Standard Time (GMT+03:00)</option> <option value='50,+,14400'>Arabian Standard Time (GMT+04:00)</option> <option value='60,+,10800'>Arabic Standard Time (GMT,+,03:00)</option> <option value='70,-,14400'>Atlantic Standard Time (GMT,-,04:00)</option> <option value='80,+,34200'>AUS Central Standard Time (GMT,+,09:30)</option> <option value='90,+,36000'>AUS Eastern Standard Time (GMT,+,10:00)</option> <option value='100,+,14400'>Azerbaijan Standard Time (GMT,+,04:00)</option> <option value='110,-,3600'>Azores Standard Time (GMT,-,01:00)</option> <option value='120,-,21600'>Canada Central Standard Time (GMT,-,06:00)</option> <option value='130,-,3600'>Cape Verde Standard Time (GMT,-,01:00)</option> <option value='140,+,14400'>Caucasus Standard Time (GMT,+,04:00)</option> <option value='150,+,34200'>Cen. Australia Standard Time (GMT,+,09:30)</option> <option value='160,-,21600'>Central America Standard Time (GMT,-,06:00)</option> <option value='170,+,21600'>Central Asia Standard Time (GMT,+,06:00)</option> <option value='180,-,14400'>Central Brazilian Standard Time (GMT,-,04:00)</option> <option value='190,+,3600'>Central Europe Standard Time (GMT,+,01:00)</option> <option value='200,+,39600'>Central Pacific Standard Time (GMT,+,11:00)</option> <option value='210,-,21600'>Central Standard Time (GMT,-,06:00)</option> <option value='220,+,28800'>China Standard Time (GMT,+,08:00)</option> <option value='230,+,10800'>E. Africa Standard Time (GMT,+,03:00)</option> <option value='240,+,36000'>E. Australia Standard Time (GMT,+,10:00)</option> <option value='250,+,7200'>E. Europe Standard Time (GMT,+,02:00)</option> <option value='260,-,10800'>E. South America Standard Time (GMT,-,03:00)</option> <option value='270,-,18000'>Eastern Standard Time (GMT,-,05:00)</option> <option value='270,+,7200'>Egypt Standard Time (GMT,+,02:00)</option> <option value='290,+,18000'>Ekaterinburg Standard Time (GMT,+,05:00)</option> <option value='300,+,43200'>Fiji Standard Time (GMT,+,12:00)</option> <option value='310,+,7200'>FLE Standard Time (GMT,+,02:00)</option> <option value='320,+,14400'>Georgian Standard Time (GMT,+,04:00)</option> <option value='330,-,10800'>Greenland Standard Time (GMT,-,03:00)</option> <option value='340,+,7200'>GTB Standard Time (GMT,+,02:00)</option> <option value='350,-,36000'>Hawaiian Standard Time (GMT,-,10:00)</option> <option value='360,+,19800'>India Standard Time (GMT,+,05:30)</option> <option value='370,+,12600'>Iran Standard Time (GMT,+,03:30)</option> <option value='380,+,7200'>Israel Standard Time (GMT,+,02:00)</option> <option value='390,+,32400'>Korea Standard Time (GMT,+,09:00)</option> <option value='400,-,7200'>Mid,-,Atlantic Standard Time (GMT,-,02:00)</option> <option value='410,-,25200'>Mountain Standard Time (GMT,-,07:00)</option> <option value='420,-,25200'>Mountain Standard Time Mexico (GMT,-,07:00)</option> <option value='430,+,23400'>Myanmar Standard Time (GMT,+,06:30)</option> <option value='440,+,21600'>N. Central Asia Standard Time (GMT,+,06:00)</option> <option value='450,+,7200'>Namibia Standard Time (GMT,+,02:00)</option> <option value='460,+,20700'>Nepal Standard Time (GMT,+,05:45)</option> <option value='470,+,43200'>New Zealand Standard Time (GMT,+,12:00)</option> <option value='480,-,12600'>Newfoundland Standard Time (GMT,-,03:30)</option> <option value='490,+,28800'>North Asia East Standard Time (GMT,+,08:00)</option> <option value='500,+,25200'>North Asia Standard Time (GMT,+,07:00)</option> <option value='510,-,14400'>Pacific SA Standard Time (GMT,-,04:00)</option> <option value='520,-,28800'>Pacific Standard Time (GMT,-,08:00)</option> <option value='530,+,3600'>Romance Standard Time (GMT,+,01:00)</option> <option value='540,+,10800'>Russian Standard Time (GMT,+,03:00)</option> <option value='550,-,10800'>SA Eastern Standard Time (GMT,-,03:00)</option> <option value='560,-,18000'>SA Pacific Standard Time (GMT,-,05:00)</option> <option value='570,-,14400'>SA Western Standard Time (GMT,-,04:00)</option> <option value='580,-,39600'>Samoa Standard Time (GMT,-,11:00)</option> <option value='590,+,25200'>SE Asia Standard Time (GMT,+,07:00)</option> <option value='600,+,28800'>Singapore Standard Time (GMT,+,08:00)</option> <option value='610,+,7200'>South Africa Standard Time (GMT,+,02:00)</option> <option value='620,+,21600'>Sri Lanka Standard Time (GMT,+,06:00)</option> <option value='630,+,28800'>Taipei Standard Time (GMT,+,08:00)</option> <option value='640,+,36000'>Tasmania Standard Time (GMT,+,10:00)</option> <option value='650,+,32400'>Tokyo Standard Time (GMT,+,09:00)</option> <option value='670,+,46800'>Tonga Standard Time (GMT,+,13:00)</option> <option value='680,-,18000'>US Eastern Standard Time (GMT,-,05:00)</option> <option value='690,-,25200'>US Mountain Standard Time (GMT,-,07:00)</option> <option value='700,+,36000'>Vladivostok Standard Time (GMT,+,10:00)</option> <option value='710,+,28800'>W. Australia Standard Time (GMT,+,08:00)</option> <option value='720,+,3600'>W. Central Africa Standard Time (GMT,+,01:00)</option> <option value='730,+,3600'>W. Europe Standard Time (GMT,+,01:00)</option> <option value='740,+,18000'>West Asia Standard Time (GMT,+,05:00)</option> <option value='750,+,36000'>West Pacific Standard Time (GMT,+,10:00)</option> <option value='760,+,32400'>Yakutsk Standard Time (GMT,+,09:00)</option> </select> </td> </tr> <tr> <td>DST Offset Minutes</td> <td> <select id='dstoffsetseconds'> <option value='0'>0 Hours</option> <option value='1800'>30 Mins</option> <option value='3600'>1 Hour</option> </select> </td> </tr> <tr> <td>Version No</td> <td><input id='versionnumber' readonly placeholder='Version1.1 ' /></td> </tr> <tr> <td id='msg' colspan=2 style='background-color:#ffffff; color:rgb(252, 0, 0);text-align:center;font-weight:bold;font-size:20px;display:none;' >Saved - Rebooting Clock - * Check Clock Display *</td> </tr> <tr> <td></td> <td id='tdButton'><button type='button' onclick='submitData();'>SUBMIT</button></td> </tr> </tbody> </table> </body></html>"

#define   testJsonConfig   "{\"data\":{\"ssid\": \"ssid\", \"sspassword\": \"sspassword\", \"ntpurl\": \"europe.pool.ntp.org\", \"ntpintervalmillis\": \"86400000\", \"timezoneoffsetseconds\": \"100,+,14400\", \"dstoffsetseconds\": \"3600\", \"versionnumber\": \"1.9\" }}"

class SoftAP
{

  

  private:

  const char*  SOFT_ROUTER_ID       = "ESP32-NTP-CLOCK";
  const char*  SOFT_ROUTER_PASSWORD = "biteme";



  public:
   
    MyDisplay myScreen;

    SoftAP(MyDisplay &mydisplay);
    String page;
    int SomeInt;
    void   StartServer();
    void   EndServer();
    void   DebugMessage( String msg, bool newline=false);
    void   SendSocketEvent(String event);



};

#endif
