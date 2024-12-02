#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "Composant.h"

class Buzzer : public Composant {
    int pin;

public:
    explicit Buzzer(int pinBuzzer);
    void activer() override;
    void desactiver() override;
};

#endif