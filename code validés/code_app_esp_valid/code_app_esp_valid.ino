#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"

/*******dht firebase*********/

#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>


/************************/

#define DHT11_PIN 15
#define DHTTYPE DHT11

#define ventilateurpin 5

DHT dht(DHT11_PIN, DHTTYPE);

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "TECNO SPARK 10 5G"
#define WIFI_PASSWORD "6a79jf929fx5k24"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCxQoPPWrirjnwXAdh-5X-_1HUFvbOuEYU"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "anatomerveille15@gmail.com"
#define USER_PASSWORD "15304am"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://smart-home-app-6e3a6-default-rtdb.firebaseio.com"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String tempPath;
String humPath;
//String presPath;

// BME280 sensor
//Adafruit_BME280 bme; // I2C
float temperature;
float humidite;
//float pressure;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 180000;

/*******************DHT FIREBASE FIN *******************/

/******************RFID**************/
/*#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21
#define RST_PIN 22
#define LED_G 25 //define green LED pin
#define LED_R 14//define red LED
#define BUZZER 12 //buzzer pin
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myServo; //define servo name 
*/



/********************/

MDNSResponder mdns;

const char* ssid = "TECNO SPARK 10 5G";
const char* password = "6a79jf929fx5k24";


WebServer server(80);
String webpage = "";

int led1 = 2;
int led2 = 21;
int led3 = 22;

/*gaz sensor***********/
int Buzzer = 32;        // used for ESP32
int Gas_analog = 4;    // used for ESP32
//int Gas_digital = 4;   // used for ESP32

/*
//PIR
#define timeSeconds 10
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
}*/
/*******************/

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Write float values to the database
void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.print("Writing value: ");
    Serial.print (value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}



void setup(void) {

 Serial.begin(115200);   // Initiate a serial communication
 /* SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myServo.attach(13); //servo pin
  myServo.write(0); //servo start position
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader...");
  Serial.println();
*/
/************************************/
  Serial.begin(115200);
  dht.begin();
  pinMode(ventilateurpin, OUTPUT);
  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  // Update database path for sensor readings
  tempPath = databasePath + "/temperature"; // --> UsersData/<user_uid>/temperature
  humPath = databasePath + "/humidite"; // --> UsersData/<user_uid>/humidity






  //webpage += "<h1>ESP32 Web Server</h1><p>LED at GPIO4 <a href=\"led1ON\"><button>ON</button></a>&nbsp;<a href=\"led1OFF\"><button>OFF</button></a></p>";
  //webpage += "<p>LED at GPIO5 <a href=\"led2ON\"><button>ON</button></a>&nbsp;<a href=\"led2OFF\"><button>OFF</button></a></p>";
 
 
 
    /***sans web serveur**/
 /**********GAZ*************/
 
  pinMode(Buzzer, OUTPUT);      
 // pinMode(Gas_digital, INPUT);
  // Serial port for debugging purposes
 
  
  /*// PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);*/





  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, LOW);
  pinMode(led3, OUTPUT);
  digitalWrite(led3, LOW);

  delay(1000);

  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   if (mdns.begin("esp")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", []() {
    server.send(200, "text/html", webpage);
  });
  
  server.on("/led1ON", []() {
    server.send(200, "text/html", webpage);
    digitalWrite(led1, HIGH);
    delay(1000);
  });
  
  server.on("/led1OFF", []() {
    server.send(200, "text/html", webpage);
    digitalWrite(led1, LOW);
    delay(1000);
  });
 
  server.on("/led2ON", []() {
    server.send(200, "text/html", webpage);
    digitalWrite(led2, HIGH);
    delay(1000);
  });

  server.on("/led2OFF", []() {
    server.send(200, "text/html", webpage);
    digitalWrite(led2, LOW);
    delay(1000);
  });

  server.on("/led3ON", []() {
    server.send(200, "text/html", webpage);
    digitalWrite(led3, HIGH);
    delay(1000);
  });

   server.on("/led3OFF", []() {
    server.send(200, "text/html", webpage);
    digitalWrite(led3, LOW);
    delay(1000);
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
 


/***********gaz*****/
int gassensorAnalog = digitalRead(Gas_analog);

  Serial.print("Gas Sensor: ");
  Serial.print(gassensorAnalog);
  Serial.print("\t");
  Serial.print("Gas Class: ");
 
  
  if (gassensorAnalog >0  ) {
    Serial.println("Gas");
    digitalWrite (Buzzer, HIGH) ; //send tone
    delay(100);
    digitalWrite (Buzzer, LOW) ;  //no tone
  }
  else {
    Serial.println("No Gas");
  }
  delay(1000);

  
//pir
/*  // Current time
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
*/
  /****************************/

// Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    // Get latest sensor readings
    temperature = dht.readTemperature();
    humidite = dht.readHumidity();
    //pressure = dht.readPressure()/100.0F;

    // Send readings to database:
    sendFloat(tempPath, temperature);
    sendFloat(humPath, humidite);
    
  }
if (temperature >25) {
    analogWrite(ventilateurpin, 1);
   
  }
  else {
    analogWrite(ventilateurpin, 0);
  }


/***************rfid******************************/
/*
// Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "43 EC 77 A9") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(100);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo.write(180);
    delay(500);
    myServo.write(0);
    digitalWrite(LED_G, LOW);
  }
 
 else   {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 500);
    delay(1000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
*/
}
