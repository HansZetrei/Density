/*Titre : DENSITY LIGHT
  Auteur: Hans Simo
  Date: 2022-12-05
  Description: Detecteur Changement de luminosité via un capteur de lumiere
  Version: 0.0.1
*/
#include <Arduino.h>
#include <SPI.h>
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h"


const int buttonPin = 3;  // la broche sur laquelle le bouton pousoir est attaché
const int ledPin = 5;    // la broche sur laquelle la LED est attaché



int buttonPushCounter = 0;  // Nombre de fois qu'on appuie sur le boutton poussoir
int buttonState = 0;        // L'état du bouton poussoir
int lastButtonState = 0;    // Le précedent état du bouton poussoir
int brightness = 0;  // La luminosité de la LED
int fadeAmount = 5;
int val;  // nombre de fois que la luminosité s'estompe

void setup() {
  // iniatialise le boutonn poussoir comme entrée
  pinMode(buttonPin, INPUT);
  // initialise la LED comme sortie
  pinMode(ledPin, OUTPUT);

  wifiConnect();
  MQTTConnect();

  // initialise la communication série
  Serial.begin(9600);
}


void loop() {
  // lit la broche sur laquelle est attachée le bouton poussoir
  buttonState = digitalRead(buttonPin);
  analogWrite(ledPin, brightness);
  val = analogRead(0);
  // compare l'état actuel du bouton poussoir a son état précédent
  if (buttonState != lastButtonState) {
    // si l'état change alors le butonPushCounter sera incrémenté
    if (buttonState == HIGH) {
      // si l'état est HIGH alors l'état ira de off a on
      buttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");

      Serial.println(buttonPushCounter);
      Serial.println(val,DEC);

      appendPayload("Luminosité", val);
      sendPayload();

    } else {
      //si l'état est LOW alors l'état ira de on a off
      Serial.println("off");
    }
    // Delai d'attente pour éviter une redondance
    delay(50);
  }
  // enregistre l'état actuel comme étant l'état précédent pour la prochaine fois dans la boucle
  lastButtonState = buttonState;


  // décolore la LED tous les deux appuis en calculant le modulo du buttonPushCOunter
  if (buttonPushCounter % 2 == 0) {
   
   // Change la luminosité pour la prochaine boucle
  brightness = brightness + fadeAmount;

  // inverse le sens de la décoloration a chaque fois que la luminosité de la LED s'estompe ou brille au maaximum
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // attendre 30 millisecondes pour voir un effet d'atténuation
  delay(30);
  }
}

