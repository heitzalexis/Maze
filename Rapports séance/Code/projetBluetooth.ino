#include<SoftwareSerial.h>
#define RX 14
#define TX 15
SoftwareSerial BlueT(RX,TX);


void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("Bonjour - Prêt pour les commandes AT");
  BlueT.begin(9600);
  delay(500);
}

void loop() {
  while(BlueT.available()){ //du module vers le moniteur
    Serial.print(char(BlueT.read()));
  }
  while (Serial.available()){//du moniteur vers le module
    BlueT.write(char(Serial.read()));
  }
}
