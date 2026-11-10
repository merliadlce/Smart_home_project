//code mq2 capteur de gaz et fumée

// Sensor pins pin D6 LED output, pin A0 analog Input

#define timeSeconds 10 //capteur PIR code 
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define ledPin 6
#define sensorPin A0
#define buzPin 25

// Set GPIOs for LED and PIR Motion Sensor
const int led = 26;
const int motionSensor = 27;

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean motion = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}
//RFID
const int pinRST = 9;  // pin RST du module RC522
const int pinSDA = 10; // pin SDA du module RC522

MFRC522 rfid(pinSDA, pinRST);

//RFID

void setup() {
  Serial.begin(9600);
  pinMode(buzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  //RIFD
  SPI.begin();
  rfid.PCD_Init();
  Serial.begin(9600);
  //RFID

// pir sensor
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}


void loop() {
  Serial.print("Analog output: ");
  Serial.println(readSensor());
  delay(500);

  // Current time PIR SENSOR
  now = millis();
  if((digitalRead(led) == HIGH) && (motion == false)) {
    Serial.println("MOTION DETECTED!!!");
    motion = true;
  }
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW);
    startTimer = false;
    motion = false;
  }
  //PIR SENSOR

  //RFID
  if (rfid.PICC_IsNewCardPresent())  // on a dédecté un tag
  {
    if (rfid.PICC_ReadCardSerial())  // on a lu avec succès son contenu
    {

      Serial.println("Voici l'UID de ce tag en decimal :");
      Serial.print("const byte bonUID[");
      Serial.print(rfid.uid.size);
      Serial.print("] = {");

      for (byte i = 0; i < rfid.uid.size; i++)
      {
        Serial.print(rfid.uid.uidByte[i]);
        if (i < rfid.uid.size - 1)
        {
        Serial.print(", ");
         }
         else
         Serial.println("};");
      }
Serial.println();
      delay (2000);
    }
  }
//RFID
  
}

//  This function returns the analog data to calling function
int readSensor() {
  unsigned int sensorValue = analogRead(sensorPin);  // Read the analog value from sensor
  unsigned int outputValue = map(sensorValue, 0, 1023, 0, 255); // map the 10-bit data to 8-bit data

  if (outputValue > 65){
    analogWrite(ledPin, outputValue); // generate PWM signal

     digitalWrite(buzPin,HIGH);
    delay(100);
    digitalWrite(buzPin,LOW);
    delay(100);
  }
  else{
    digitalWrite(buzPin,LOW);
    digitalWrite(ledPin, LOW);
  return outputValue;             // Return analog moisture value
  }// MQ2

}




