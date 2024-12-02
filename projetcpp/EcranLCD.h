#ifndef ECRANLCD_H
#define ECRANLCD_H

#include <Wire.h>
#include <rgb_lcd.h>
#include <Arduino.h>
#include "Composant.h"

class EcranLCD : public Composant {
    rgb_lcd lcd;

public:
    EcranLCD();
    void afficher(const String& message);
    void changerCouleur(uint8_t r, uint8_t g, uint8_t b);
    void activer() override;
    void desactiver() override;
};

#endif