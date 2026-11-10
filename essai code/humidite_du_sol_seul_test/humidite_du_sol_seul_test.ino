void setup(){
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop(){
  int humidite = analogRead(A0);
  Serial.print(humidite);
  Serial.print(" = ");

  if(humidite < 370) {
Serial.println("Sol mouillé");
  }       
  if(humidite >= 370 & humidite < 600){
Serial.println("Sol humide");
  }          
  if(humidite >= 600 & humidite < 1000) {
Serial.println("Sol sec, pensez à arroser");
  } 
  if(humidite >= 1000) {
Serial.println("Capteur hors sol");
  } 
delay(200);
}
