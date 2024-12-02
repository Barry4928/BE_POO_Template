#include "Buzzer.h"

Buzzer::Buzzer(int pinBuzzer) : pin(pinBuzzer) {
    pinMode(pin, OUTPUT);
    desactiver();
}

void Buzzer::activer() {
    digitalWrite(pin, HIGH);
}

void Buzzer::desactiver() {
    digitalWrite(pin, LOW);
}