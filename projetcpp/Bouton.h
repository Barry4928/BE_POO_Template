#ifndef BOUTON_H
#define BOUTON_H

#include <Arduino.h>

class Bouton {
    int pin;

public:
    explicit Bouton(int pinBouton);
    bool estAppuye() const;
};

#endif