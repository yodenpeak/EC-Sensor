#include <OneWire.h>
#include <MCP3208.h>
#include <SPI.h>
#include "DS1302.h"
#include <SoftwareSerial.h>

SoftwareSerial ArduinoSerial(8, 9); // RX | TX
DS1302 rtc(2, 3, 4);

Time t;

int R1= 1000; //R 65.8 kohm(685)//1kohm(2000)
int Ra=25; //Resistance of powering Pins
int ECPin= A0;//////////////////channel1
int ECPower =13;
float PPMconversion=0.5;
int reading;
int tempPin = A3;
float Temperature=10;
float K=2.88;
float TemperatureCoef = 0.019; //this changes depending on what chemical we are measuring
float Vin= 5;
float Vdrop= 0;
float buffer=0;
//////////////////////////////////channel1/////////////////////////////////////////////
float EC=0;
float EC1 =0;
float EC2 =0;
int ppm =0;
float raw= 0;
float Rc= 0;
//*************************************************************************************
//////////////////////////////////channel2/////////////////////////////////////////////
int ECPin2= A1; 
//*************************************************************************************
void setup() {
  // put your setup code here, to run once:
  pinMode(3,INPUT);
  pinMode(2,OUTPUT);
  ArduinoSerial.begin(57600);
  analogReference(INTERNAL);
  Serial.begin(9600);
  pinMode(ECPin,INPUT);
  pinMode(ECPin2,INPUT);
  pinMode(ECPower,OUTPUT);//Setting pin for sourcing current
  delay(100);// gives sensor time to settle
  R1=(R1+Ra);
  Serial.println("*************************************************");
  Serial.println("**                 Project CPE#4               **");
  Serial.println("*************************************************");
  Serial.println("**  Sasaluk     Powvichai         57109011061  **");
  Serial.println("**  Suwaraphon Thanawiwatchaikun  57109011310  **");
  Serial.println("*************************************************");
  Serial.println();
  Serial.println();
  Serial.println("   _______  _____    ______ ");
  Serial.println("  / /      | |   \\\\ ||      ");
  Serial.println(" | |       | |___// ||====   ");
  Serial.println("  \\_\\_____ |_|      ||_____ ");
}

void loop() {
  // put your main code here, to run repeatedly:
  t = rtc.getTime();
  while(t.date == 0){}
  getdata();
  //if(t.min%15 == 0 && t.sec%20 == 0)  
  if(t.sec%5 == 0)
  {
    clockdate();
    GetEC();          //Calls Code to Go into GetEC() Loop [Below Main Loop] dont call this more that 1/5 hhz [once every five seconds] or you will polarise the water
    Serial.println("*************************** EC 1 ******************************");
    PrintReadings();
    sendEC1data();
    GetEC2();
    Serial.println("*************************** EC 2 ******************************");
    PrintReadings2();
    sendEC2data();
    //////
    ArduinoSerial.print("D");
    ArduinoSerial.print(t.date); 
    ArduinoSerial.print("\n");
    //////
  }
  // Cals Print routine [below main loop]
  delay(1000);
}

void GetEC()
  {
    Gettemp();
    //************Estimates Resistance of Liquid ****************//
    digitalWrite(ECPower,HIGH);
    raw= analogRead(ECPin);
    raw= analogRead(ECPin);// This is not a mistake, First reading will be low beause if charged a capacitor
    digitalWrite(ECPower,LOW);
    //***************** Converts to EC **************************//
    Vdrop= (Vin*raw)/1024.0;
    Rc=(Vdrop*R1)/(Vin-Vdrop);
    Rc=Rc-Ra; //acounting for Digital Pin Resitance
    EC = 1000/(Rc*K);
    //*************Compensating For Temperaure********************//
    EC1  =  (EC)/(1+ TemperatureCoef*(Temperature-25.0));
    if(EC1 > 0.1 && EC1 < 0.3){
      EC1 = 0.301;}
    //else if(EC25 <0.00)
    ppm=(EC1)*(PPMconversion*1000);
    ;
  }
void GetEC2()
  {
    Gettemp();
    //************Estimates Resistance of Liquid ****************//
    digitalWrite(ECPower,HIGH);
    raw= analogRead(ECPin2);
    raw= analogRead(ECPin2);// This is not a mistake, First reading will be low beause if charged a capacitor
    digitalWrite(ECPower,LOW);
    //***************** Converts to EC **************************//
    Vdrop= (Vin*raw)/1024.0;
    Rc=(Vdrop*R1)/(Vin-Vdrop);
    Rc=Rc-Ra; //acounting for Digital Pin Resitance
    EC = 1000/(Rc*K);
    //*************Compensating For Temperaure********************//
    EC2  =  (EC)/(1+ TemperatureCoef*(Temperature-25.0));
    if(EC2 > 0.1 && EC2 < 0.3){
      EC2 = 0.301;}
    ppm=(EC2)*(PPMconversion*1000);
    ;
  }
void PrintReadings(){
  Serial.print(" EC: ");
  Serial.print(EC1,4);
  Serial.print(" Simens  ");
  Serial.print(ppm);
  Serial.print(" ppm  ");
  Serial.print("Temperature: ");
  Serial.print(Temperature);
  Serial.println(" *C ");
};

void PrintReadings2(){
  Serial.print(" EC2: ");
  Serial.print(EC2,4);
  Serial.print(" Simens  ");
  Serial.print(ppm);
  Serial.print(" ppm  ");
  Serial.print("Temperature: ");
  Serial.print(Temperature);
  Serial.println(" *C ");
};

void Gettemp(){
  reading = analogRead(tempPin);
  Temperature = reading / 9.31;
}

void clockdate() 
{
        Serial.println("------Time------");   
        Serial.println(rtc.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN, '/'));
        Serial.println(rtc.getDOWStr());
        Serial.println(rtc.getTimeStr());
}

void sendEC1data(){
  ArduinoSerial.print("A");  
  ArduinoSerial.print(EC1,4);
  ArduinoSerial.print("B");
  ArduinoSerial.print(Temperature);
}

void sendEC2data(){
  ArduinoSerial.print("X");  
  ArduinoSerial.print(EC2,4);
  ArduinoSerial.print("Y");
  ArduinoSerial.print(Temperature);
}

void getdata(){
  while (ArduinoSerial.available() > 0) 
  {
    if (ArduinoSerial.read() == '\n') 
    {
      clockdate();
      GetEC();          //Calls Code to Go into GetEC() Loop [Below Main Loop] dont call this more that 1/5 hhz [once every five seconds] or you will polarise the water
      Serial.println("*************************** EC 1 ******************************");
      PrintReadings();
      sendEC1data();
      GetEC2();
      Serial.println("*************************** EC 2 ******************************");
      PrintReadings2();
      sendEC2data();
      //////
      ArduinoSerial.print("D");
      ArduinoSerial.print(t.date); 
      ArduinoSerial.print("\n");
    }
    delay(50);
  }
}
