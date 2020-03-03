//### Importation des bibliothèques qui nous seront utiles lors de notre projet
#include <NewPing.h> //Utile pour les capteurs HC-SR04
#include <SoftwareSerial.h> //Utile pour le module Bluetooth

//### Définition de variables utiles pour les sensors
#define TRIGGER_PIN1  2   //trig sensor (1) frontal 
#define ECHO_PIN1     3   //echo sensor (1) frontal
#define TRIGGER_PIN2  8   //trig sensor (2) gauche
#define ECHO_PIN2     11  //echo sensor (2) gauche
#define TRIGGER_PIN3  12  //trig sensor (3) droite
#define ECHO_PIN3  13     //echo sensor (3) droite
#define MAX_DISTANCE 400
 
NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);//sensor frontal
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);//sensor gauche
NewPing sonar3(TRIGGER_PIN3, ECHO_PIN3, MAX_DISTANCE);//sensor droite

//### Définition des variables utiles pour les moteurs

//-- MOTEUR DROIT --
int END=9; //Connecté à Arduino pin 9 (sortie PWM)
int IN1D=4; //Connecté à Arduino pin 4
int IN2D=5; //Connecté à Arduino pin 5
//-- MOTEUR GAUCHE --
int ENG=10; //Connecté à Arduino pin 10(Sortie PWM)
int IN3G=6; //Connecté à Arduino pin 6
int IN4G=7; //Connecté à Arduino pin 7

//###Définition des variables utiles pour le Bluetooth
#define RX 14 // /!\ Revoir les numéros de port
#define TX 15 // /!\ Revoir les numéros de port
SoftwareSerial BlueT(RX,TX);
char Data;

void setup() {

  Serial.begin(9600);
  pinMode(END,OUTPUT); // Configurer
  pinMode(ENG,OUTPUT); // les broches
  pinMode(IN1D,OUTPUT); // comme sortie
  pinMode(IN2D,OUTPUT);
  pinMode(IN3G,OUTPUT);
  pinMode(IN4G,OUTPUT);
  digitalWrite(END,LOW);// Moteur Droit - Ne pas tourner
  digitalWrite(ENG,LOW);// Moteur Gauche - Ne pas tourner
//initialiser le sens (vers l'avant)
  digitalWrite(IN1D,HIGH );//sens avant roue droite
  digitalWrite(IN2D,LOW);//sens avant roue droite
  digitalWrite(IN3G,HIGH);//sens avant roue gauche
  digitalWrite(IN4G,LOW);//sens avant roue gauche
  BlueT.begin(9600); //Initialise le bluetooth
}

void loop() {
//Affichage du résultat des différents ping utiles dans l'appli Bluetooth
  BlueT.print("*1"+String(sonar1.ping_cm())+"*");
  BlueT.print("*3"+String(sonar2.ping_cm())+"*");
  BlueT.print("*4"+String(sonar3.ping_cm())+"*");
//Affichage du résultat des différents ping utiles pour les tests
  Serial.print("Ping1:");
  Serial.print(sonar1.ping_cm());
  Serial.print("    |    ");
  Serial.print("Ping2:");
  Serial.print(sonar2.ping_cm());
  Serial.print("    |    ");
  Serial.print("Ping3:");
  Serial.println(sonar3.ping_cm());
 
if(sonar1.ping_cm()>25 && sonar2.ping_cm()>9 && sonar3.ping_cm()>9){ //S'il n'y a rien devant et que le robot est bien centré aller tout droit
  toutDroit();
} else if(sonar1.ping_cm()>=25 && sonar2.ping_cm()<=11 && sonar3.ping_cm()>11){ //S'il n'y a rien devant mais que le robot est trop à gauche, on tourne vers la droite
  rectifierADroite();
} else if(sonar1.ping_cm()>=25 && sonar2.ping_cm()>=11 && sonar3.ping_cm()<=11){ //S'il n'y a rien devant mais que le robot est trop à droite, on tourne vers la gauche
  rectifierAGauche();
/*} else if (sonar1.ping_cm()<12 && sonar2.ping_cm()<12 && sonar3.ping_cm()<12){ //S'il n'y a pas assez de distance ni devant, ni à gauche, ni à droite : on fait demi-tour
  impasse();*/
} else if(sonar1.ping_cm()<25) {//obstacle devant 
  //arret();
  if(sonar2.ping_cm()>30) { //si c'est loin à gauche, on tourne à gauche
    tournerAGauche();
  } else if (sonar2.ping_cm()<30 && sonar3.ping_cm()>30) { //si on n'a pas de place à gauche mais à droite alors on va à droite
    tournerADroite();
  } else if (sonar2.ping_cm()<30 && sonar3.ping_cm()<30) { //si pas de place ni à gauche ni à droite, on fait demi-tour
    impasse();
  }
}
} //fin de void loop()

void toutDroit () {
  Serial.println("tout droit");
  BlueT.print("*2J'avance tout droit*"); 
  digitalWrite(IN1D,HIGH);//sens
  digitalWrite(IN2D,LOW);//avant roue droite
  digitalWrite(IN3G,HIGH);//sens avant
  digitalWrite(IN4G,LOW);//roue gauche

  delay(250); 
  analogWrite(END,145);
  analogWrite(ENG,140);
  delay(500);
  analogWrite(END,0);
  analogWrite(ENG,0);  
}

void rectifierADroite (){
  Serial.println("rectifier à droite ");
  BlueT.print("*2Je rectifie a droite*");
  digitalWrite(IN1D,HIGH);
  digitalWrite(IN2D,LOW);
  digitalWrite(IN3G,HIGH);
  digitalWrite(IN4G,LOW);
  
  delay(250);
  analogWrite(END,130);
  analogWrite(ENG,140);
  delay(500);
  analogWrite(END,0);
  analogWrite(ENG,0);
  
 
}

void rectifierAGauche(){
  Serial.println("rectifier à gauche ");
  BlueT.print("*2Je rectifie a gauche*"); 
  digitalWrite(IN1D,HIGH );
  digitalWrite(IN2D,LOW);
  digitalWrite(IN3G,HIGH);
  digitalWrite(IN4G,LOW);
  
  delay(250);
  analogWrite(END,140);//120 170
  analogWrite(ENG,130);
  delay(500);
  analogWrite(END,0);
  analogWrite(ENG,0);
 
}

void impasse(){
  Serial.println("impaaaaaaaaaasse");
  BlueT.print("*2Je fais demi-tour*"); 
  digitalWrite(IN1D,HIGH );
  digitalWrite(IN2D,LOW);
  //roue gauche en sens inverse pour le faire tourner sur lui-même
  digitalWrite(IN3G,LOW);
  digitalWrite(IN4G,HIGH);
  //demi tour vers la gauche
  analogWrite(END,150);
  analogWrite(ENG,150);
  delay(700);
}

void arret(){
  Serial.println("arrêt");
  analogWrite(END,0);
  analogWrite(ENG,0);
  BlueT.print("*2Je m'arrete*"); 
}

void tournerAGauche(){ //techniquement c'est pareil que impasse
    Serial.println("tourner à gauche");
    BlueT.print("*2Je tourne a gauche*"); 
    digitalWrite(IN1D,HIGH );
    digitalWrite(IN2D,LOW);
    digitalWrite(IN3G,HIGH);
    digitalWrite(IN4G,LOW);
    //demi tour vers la gauche
    analogWrite(END,128);
    analogWrite(ENG,0);

}

void tournerADroite(){
    Serial.println("tourner à droite");
    BlueT.print("*2Je tourne a droite*");
    //On met le moteur droit en sens inverse
    digitalWrite(IN1D,HIGH );
    digitalWrite(IN2D,LOW);
    digitalWrite(IN3G,HIGH);
    digitalWrite(IN4G,LOW);
    //demi tour vers la droite
    analogWrite(END,0);
    analogWrite(ENG,128); 
}
