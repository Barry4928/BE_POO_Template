#ifndef POTENTIOMETRE_H
#define POTENTIOMETRE_H

#include <Arduino.h>

class Potentiometre {
    int pin;
    float temperatureMin;
    float temperatureMax;

public:
    Potentiometre(int pinPot, float tempMin, float tempMax);
    float lireTemperatureReglee() const;
};

#endif