//code mq2 capteur de gaz et fumée

// Sensor pins pin D6 LED output, pin A0 analog Input

#include <Wire.h>
#define ledPin 6
#define sensorPin A0
#define buzPin 25
void setup() {
  Serial.begin(9600);
  pinMode(buzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  Serial.print("Analog output: ");
  Serial.println(readSensor());
  delay(500);
}

//  This function returns the analog data to calling function
int readSensor() {
  unsigned int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor
  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255); // map the 10-bit data to 8-bit data

  if (outputValue > 65){
    analogWrite(ledPin, outputValue); // generate PWM signal

     digitalWrite(buzPin,HIGH);
    delay(1);
    digitalWrite(buzPin,LOW);
    delay(1);
  }
  else{
    digitalWrite(buzPin,LOW);
    digitalWrite(ledPin, LOW);
  return outputValue;             // Return analog moisture value
  }
}
