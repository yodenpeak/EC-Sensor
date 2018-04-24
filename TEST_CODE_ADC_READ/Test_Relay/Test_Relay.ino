#include <SPI.h>

int latchPin = D3; // 12 ST_CP storage resister clock input
int clockPin = D2; // 11 SC_CP Shift register clock input
int dataPin = D1; //14 Serial data input

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //////IC 74HC595N////////////
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  ////////////////////////////
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("ON");
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b11111111);
  digitalWrite(latchPin, HIGH);
  delay(2000);
  Serial.println("OFF");
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b0000000);
  digitalWrite(latchPin, HIGH);
  delay(2000);

}
