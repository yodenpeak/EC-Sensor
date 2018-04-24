#include <MCP3208.h>
#include <SPI.h>

MCP3208 adc(D8);

int latchPin = D3; // 12 ST_CP storage resister clock input
int clockPin = D2; // 11 SC_CP Shift register clock input
int dataPin = D1; //14 Serial data input
float Vin = 3.3;
int Read;
float V;
float temp;

void setup() {
  adc.begin();
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(115200);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}

void loop() {
  //Read = adc.analogRead(0);// Serial.println(adc.analogRead(0));
  V = (Vin/4096.0)* adc.analogRead(4);
  GetTem();
  Serial.print(Read);
  Serial.print("\t");
  Serial.print(temp);
  Serial.print("    "); 
  Serial.println(V); 
  /////////////////////////////////////////////////////////
  shiftOut(dataPin, clockPin, MSBFIRST, 0b10100010);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  delay(500);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00100000);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
  delay(500);
  /////////////////////////////////////////////////////////
}

void GetTem(){
  Read = adc.analogRead(0);
  float mv = ( Read/4096.0)*4000; 
  Serial.println(mv);
  temp = (mv) / 10; 
}
