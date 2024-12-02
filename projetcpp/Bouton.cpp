#include "Bouton.h"

Bouton::Bouton(int pinBouton) : pin(pinBouton) {
    pinMode(pin, INPUT_PULLUP);
}

bool Bouton::estAppuye() const {
    return digitalRead(pin) == HIGH;
}