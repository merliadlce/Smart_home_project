// Smart Irrigation System with LDR Control
// by combining moisture sensor and LDR
#include <IRremote.h> // add IR remote library
#include <Servo.h> // add servo motor library

const byte IR_RECEIVE_PIN = 2;   // pin for the IR sensor

Servo servo1; 
int motor_angle = 0;
int sensor_pin = A0;
/************/
int photo = A1;
int output_value;
int led = 8;

void setup() {
  pinMode(3, OUTPUT); // Water pump control pin
  pinMode(photo, INPUT);
  pinMode(led, OUTPUT);
  Serial.begin(300);
  Serial.begin(300);
  Serial.println("Initializing...");
  delay(5000);
   Serial.begin(300); // initialize serial communication
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  servo1.attach(9); // declare servo motor pin
  servo1.write(motor_angle); // move the motor to 0 deg
  Serial.println("Servo motor angle 0 degree");
  delay(5000);
}

void loop() {
  // Read moisture value
  output_value = analogRead(sensor_pin);
  output_value = map(output_value, 550, 10, 0, 100);
  Serial.print("Moisture: ");
  Serial.print(output_value);
  Serial.println("%");

  // Read LDR value
  int lecture = analogRead(photo);
  Serial.print("LDR Value: ");
  Serial.println(lecture);

  // Control water pump based on moisture
  if (output_value <0) { // Adjust threshold as needed
    digitalWrite(3, HIGH); // Turn on water pump
  } else {
    digitalWrite(3, LOW); // Turn off water pump
  }

  // Control LED based on LDR
  //PHOTORESISTANCE
  if (lecture <300) {
    digitalWrite(led, LOW); // Dark, turn off LED
  } else if (lecture >700) 
  delay(500);
  {
    digitalWrite(led, HIGH); // Bright, turn on LED
  }
  if(IrReceiver.decode()) // when button is pressed and code is received
  {
      String ir_code = String(IrReceiver.decodedIRData.command, HEX);
      if(ir_code != "19"){
        Serial.println(ir_code);
      }

    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    // check if digit1 button is pressed
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    if(ir_code=="1c")
    {
      Serial.println("servo motor angle 30 deg");
      motor_angle = 30;
      servo1.write(motor_angle); // move the motor to 30 deg
    }
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    // check if digit2 button is pressed
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    else if(ir_code=="45")
    {
      Serial.println("servo motor angle 60 deg");
      motor_angle = 60;
      servo1.write(motor_angle); // move the motor to 60 deg
    }  
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    // check if digit3 button is pressed
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    else if(ir_code=="44")
    {
      Serial.println("servo motor angle 90 deg");
      motor_angle = 90;          
      servo1.write(motor_angle);
    }  
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    // check if digit4 button is pressed
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    else if(ir_code=="7")
    {
      Serial.println("servo motor angle 120 deg");
      motor_angle = 120;
      servo1.write(motor_angle);
    }
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    // check if digit5 button is pressed
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    else if(ir_code=="e")
    {
      Serial.println("servo motor angle 0 deg");
      motor_angle = 0;
      servo1.write(motor_angle);
    }
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    // check if digit UP button is pressed
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN 
    else if(ir_code=="20")
    {
      if(motor_angle<150) motor_angle+=10; // increase motor angle
      Serial.print("Motor angle is ");
      Serial.println(motor_angle);
      servo1.write(motor_angle);         // and move the motor to that angle
    }
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    // check if digit DOWN button is pressed
    //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    else if(ir_code=="21")
    {
      if(motor_angle>0) motor_angle-=10; // decrease motor angle
      Serial.print("Motor angle is ");
      Serial.println(motor_angle);
      servo1.write(motor_angle);      // and move the motor to that angle
    }
    delay(250);
    IrReceiver.resume();     // again be ready to receive next code
  }
  
}
