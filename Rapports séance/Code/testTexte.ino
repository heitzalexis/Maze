#include<SoftwareSerial.h>
#include <NewPing.h>
#define RX 2
#define TX 3
#define TRIGGER_PIN1  8   //trig sensor
#define ECHO_PIN1     9   //echo sensor
#define MAX_DISTANCE 400
SoftwareSerial BlueT(RX,TX);
NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
char Data;

void setup(){
  Serial.begin(9600);
  BlueT.begin(9600);
}
void loop(){
//Affichage du résultat du ping1 utiles pour les tests
  Serial.print("Ping1:");
  Serial.println(sonar1.ping_cm());
//Affichage du ping1 dans l'appli Bluetooth (panel 12 tel Camille)
  BlueT.print("*1"+String(sonar1.ping_cm())+"*");
//Si le ping1 est sup à 6 alors ça affiche Tout droit dans la case du bas  
  if (sonar1.ping_cm()>6){
    BlueT.print("*2Toutdroit*");
//Sinon cela affiche arrêt  
  }
  else {
    BlueT.print("*2Arret*");
  }
}
