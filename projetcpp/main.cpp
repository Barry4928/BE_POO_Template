#include <Arduino.h>
#include "SystemController.h"

const int PIN_LED = D5;
const int PIN_BUZZER = D6;
const int PIN_BOUTON = D7;
const int PIN_POT = A0;

SystemController controleur(PIN_LED, PIN_BUZZER, PIN_BOUTON, PIN_POT);

void setup() {
    Serial.begin(115200);
    controleur.initialiser();
}

void loop() {
    controleur.mettreAJour();
    delay(100);
}