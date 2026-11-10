#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();//Initialisation du Bus SPI
  rfid.PCD_Init();//Initialisation du lecteur RFID
  Serial.println("Approchez votre carte RFID pour ouvrir la porte...");
}

void loop() {
  // Vérifie si une carte RFID est détectée
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // Récupère l'identifiant de la carte
    String carteID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      carteID.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
      carteID.concat(String(rfid.uid.uidByte[i], HEX));
    }

    Serial.print("Carte détectée : ");
    Serial.println(carteID);

    // Vérifie si l'identifiant de la carte est autorisé
    if (carteID == "XXXXXXXXXXXX") { // Remplacez XXXXXXXXXXXX par l'identifiant de la carte autorisée
      Serial.println("Accès autorisé. La porte s'ouvre.");
      // Executer l'action correspondante 
    } else {
      Serial.println("Accès non autorisé. La porte reste fermée.");
    }

    // Attendez que la carte soit retirée avant de continuer
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
