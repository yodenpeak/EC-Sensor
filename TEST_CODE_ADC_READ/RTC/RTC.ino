// DS1302:  RS -> Arduino Digital 2
//          DA pin   -> Arduino Digital 3
//          CL pin  -> Arduino Digital 4
//          VCC -> 5V
//          GND -> GND

#include "DS1302.h"
#include <SoftwareSerial.h>

SoftwareSerial ArduinoSerial(8, 9); // RX | TX
DS1302 rtc(2, 3, 4);

Time t;
int a = 10;
int b = 35;

void clockdate() 
{
        Serial.println("------Time------");   
        Serial.println(rtc.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN, '/'));
        Serial.println(rtc.getDOWStr());
        Serial.println(rtc.getTimeStr());
}
 
void setup()
{
  pinMode(3,INPUT);
  pinMode(2,OUTPUT);
  ArduinoSerial.begin(57600);
  Serial.begin(9600);
  ////// Time Set Run 1 time /////////
  /*
  rtc.halt(false);
  rtc.writeProtect(false);
  rtc.setDOW(MONDAY);         
  rtc.setTime(13, 45,00);      
  rtc.setDate(9, 4, 2018);  //(D,M,Y)  
  rtc.writeProtect(true);
  */
}

 

void loop()
{ 
  //clockdate();
  t = rtc.getTime();
  Serial.print(t.sec); 
  Serial.println("\t");
  senddata();
  ArduinoSerial.print("\n");
  delay(1000);
}

void senddata(){
  ArduinoSerial.print("EC");
  ArduinoSerial.print(a);
  ArduinoSerial.print("Tem");
  ArduinoSerial.print(b);
}
  
