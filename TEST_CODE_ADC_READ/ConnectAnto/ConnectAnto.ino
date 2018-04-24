#include <AntoIO.h>

float temp = 25.55;
float EC = 3.5;

const char *ssid = "Ohm";
const char *pass = "123456789@";
const char *user = "ohmalpha";
const char *token = "qY8N8cwszSUHXanf24wj2EaIMabOPnsO3GHlm2iK"; //Key//
const char *thing = "NodeMCU";

// initialize AntoIO instance
AntoIO anto(user, token, thing);
void messageReceived(String thing, String channel, String payload);

void setup() {
  Serial.begin(115200);
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

  anto.sub("Date");
  anto.sub("EC_Meter");
  anto.sub("Temp");

}

void loop() {
  anto.mqtt.loop();  
  
  if (!anto.mqtt.isConnected())
    Serial.println("Disconnected");

  Serial.print("TEMP: ");
  Serial.print(temp);
  Serial.print("\t");
  Serial.print("EC: ");
  Serial.println(EC);

  anto.pub("Temp",temp);
  anto.pub("EC_Meter",EC);

  delay(1000);
}

void messageReceived(String thing, String channel, String payload) {
  Serial.print("Recieved: ");
  Serial.print(thing);
  Serial.print("/");
  Serial.print(channel);
  Serial.print("-> ");
  Serial.println(payload);
/*
  if(channel.equals("Temp")){
        float value = payload.toFloat();
        tempCutOff = value;
  }
  else if(channel.equals("EC_Meter")){
        float value = payload.toFloat();
        humidCutOff = value;
  }*/
}
