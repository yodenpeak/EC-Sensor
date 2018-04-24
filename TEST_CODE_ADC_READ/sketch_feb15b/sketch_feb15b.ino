#include <SoftwareSerial.h>
#include <AntoIO.h>
#include <SPI.h>

int latchPin = D3; // 12 ST_CP storage resister clock input
int clockPin = D2; // 11 SC_CP Shift register clock input
int dataPin = D1; //14 Serial data input

/*day
  1-7
  10 - 14
  15 - 25
  26 - 30
  31 - 35
  36 - 40
  41 - 50*/

float ECmin[7] = {1.0 , 1.2 , 1.3 , 1.2 , 1.1 , 1.0 , 0.3};
float ECmix[7] = {1.2 , 1.3 , 1.4 , 1.3 , 1.2 , 1.1 , 0.5};

SoftwareSerial NodeSerial(D5, D6); // RX | TX
/////////////////////////////////////////////////////////////////////////////////////////
const char *ssid = "Ohm";
const char *pass = "123456789@";
const char *user = "ohmalpha";
const char *token = "LSDXOu6PRAqNUHm9gayCrERpub8k0gwoc8Oef3ag"; //Key//
const char *thing = "NodeMCU";
////////////////////////////////////////////////////////////////////////////////////////

////////////////Vaule///////////////////////////////////////////////////////////////////
float EC1 = 0,EC2 = 0;
float Temperature1 = 0,Temperature2 = 0;
int day,daycount,daynow=0;
////////////////////////////////////////////////////////////////////////////////////////

// initialize AntoIO instance
AntoIO anto(user, token, thing);
void messageReceived(String thing, String channel, String payload);
    
void setup() {
  pinMode(D5, INPUT);
  pinMode(D6, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
  NodeSerial.begin(57600);
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH);

  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Anto library version: ");
  Serial.println(anto.getVersion());

  Serial.print("\nTrying to connect ");
  Serial.print(ssid);
  Serial.println("...");

  //Connect to anto.io saver get data form Anto to messageReceived//
  anto.begin(ssid, pass, messageReceived); 
  Serial.println("\nConnected Anto done");

  digitalWrite(latchPin, LOW);
  anto.sub("Daycount");
  anto.sub("EC1");
  anto.sub("EC2");
  anto.sub("Temp");

  delay(500);
  NodeSerial.print("\n"); 
  getdata();
}

void loop() {
  if(WiFi.status() !=  WL_CONNECTED || WiFi.status() == 6); 
    
  else{
//    Serial.println("Connect to anto.io");
    anto.mqtt.connect();
  }

//  Serial.println(WiFi.status());
  getdata();
  
  if(day != daynow){
    daynow = day;
    daycount++;
    }

  //
    
  anto.pub("Daycount",daycount);
  anto.pub("Temp",Temperature1);
  anto.pub("EC1",EC1);
  anto.pub("EC2",EC2);
  
  //NodeSerial.print("\n");
  if(EC1 != 0 && EC2 != 0 && Temperature1 != 0 && Temperature2 != 0){
    checkEC();
  }
  //

  delay(1000); 
}

void getdata(){
  while (NodeSerial.available() > 0) 
  {
    if (NodeSerial.read() == 'A')
      EC1 = NodeSerial.parseFloat();
    if (NodeSerial.read() == 'B')
      Temperature1 = NodeSerial.parseFloat();
    if (NodeSerial.read() == 'X')
      EC2 = NodeSerial.parseFloat();
    if (NodeSerial.read() == 'Y')
      Temperature2 = NodeSerial.parseFloat();
    if (NodeSerial.read() == 'D')
      day = NodeSerial.parseInt();
    if (NodeSerial.read() == '\n') 
    {
          Serial.print("NodeMCU"); 
          Serial.print(" : "); 
          Serial.println(day);
          Serial.print("EC-1 : ");
          Serial.print(EC1,4);
          Serial.print("\t Temperature-1 : ");
          Serial.print(Temperature1);
          Serial.println(" *C");
          Serial.print("EC-2 : ");
          Serial.print(EC2,4);
          Serial.print("\t Temperature-2 : ");
          Serial.print(Temperature2);
          Serial.println(" *C");  
    }
    delay(50);
  }
}

void messageReceived(String thing, String channel, String payload) {
  Serial.print("Recieved: ");
  Serial.print(thing);
  Serial.print("/");
  Serial.print(channel);
  Serial.print("-> ");
  Serial.println(payload);
}

void checkEC(){
  if(daycount <=2){
    if(EC1 <= ECmin[0]){
      // Active Relay 7 and 6 for increase EC value in garden1
      activerelay76();
      deactiverelay();
    }
    if(EC2 <= ECmin[0]){
      // Active Relay 4 and 3 for increase EC value in garden2
      activerelay43();
      deactiverelay();
    }
  }
  else if(daycount > 2 && daycount <= 7){
    if(EC1 <= ECmin[1]){
      activerelay76();
      deactiverelay();
    }
    if(EC2 <= ECmin[1]){
      activerelay43();
      deactiverelay();
    }
  }
  else if(daycount > 7 && daycount <= 18){
    if(EC1 <= ECmin[2]){
      activerelay76();
      deactiverelay();
    }
    if(EC2 <= ECmin[2]){
      activerelay43();
      deactiverelay();
    }
  }
  else if(daycount > 18 && daycount <= 23){
    if(EC1 <= ECmin[3]){
      activerelay76();
      deactiverelay();
    }
    if(EC2 <= ECmin[3]){
      activerelay43();
      deactiverelay();
    }
  }
  else if(daycount > 23 && daycount <= 28){
    if(EC1 <= ECmin[4]){
      activerelay76();
      deactiverelay();
    }
    if(EC2 <= ECmin[4]){
      activerelay43();
      deactiverelay();
    }
  }
  else if(daycount > 28 && daycount <= 33){
    if(EC1 <= ECmin[5]){
      activerelay76();
      deactiverelay();
    }
    if(EC2 <= ECmin[5]){
      activerelay43();
      deactiverelay();
    }
  }
  else if(daycount > 33){
  }
  Serial.println("Exit from checkEC");
}

void activerelay76(){
  Serial.println("Active Relay 7");
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b10000000);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  delay(60000);
  Serial.println("Active Relay 6");
  shiftOut(dataPin, clockPin, MSBFIRST, 0b01000000);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  delay(60000);
}

void activerelay43(){
  Serial.println("Active Relay 4");
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00010000);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  delay(60000);
  Serial.println("Active Relay 3");
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00001000);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  delay(60000);
}

void deactiverelay(){
  Serial.println("Deactive Relay");
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH);
  delay(10);
  digitalWrite(latchPin, LOW);
  NodeSerial.print("\n");
}

