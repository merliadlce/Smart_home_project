//code mq2 capteur de gaz et fumée

// Sensor pins pin D6 LED output, pin A0 analog Input

#include <Wire.h>

#define sensorPin 22
//#define buzPin 25
int buzzer=23;
void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
 
 
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
   // analogWrite(ledPin, outputValue); // generate PWM signal

     digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    delay(1000);
  }
  else{
    digitalWrite(buzzer,LOW);
   // digitalWrite(ledPin, LOW);
  return outputValue;             // Return analog moisture value
  }
}
