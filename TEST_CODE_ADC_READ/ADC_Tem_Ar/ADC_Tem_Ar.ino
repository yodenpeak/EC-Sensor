int tempPin = A3;    // select the input pin for the potentiometer
int reading ;  // variable to store the value coming from the sensor
float Temperature = 0;

void setup() {
  Serial.begin(9600);
  // declare the ledPin as an OUTPUT:
}

void loop() {
  // read the value from the sensor:
  GetTem();
  Serial.print(reading);
  Serial.print("\t");
  Serial.println(Temperature);
  delay(1000);
}

void GetTem(){
  reading = analogRead(tempPin);
  float mv = ( reading/1024.0)*5000; 
  Temperature = mv / 10;
}
