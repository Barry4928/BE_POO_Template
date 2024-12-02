#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "Composant.h"

class LED : public Composant {
    int pin;

public:
    explicit LED(int pinLED);
    void activer() override;
    void desactiver() override;
};

#endif