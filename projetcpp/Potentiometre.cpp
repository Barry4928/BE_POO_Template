#include "Potentiometre.h"

Potentiometre::Potentiometre(int pinPot, float tempMin, float tempMax)
    : pin(pinPot), temperatureMin(tempMin), temperatureMax(tempMax) {
    pinMode(pin, INPUT);
}

float Potentiometre::lireTemperatureReglee() const {
    int valeurBrute = analogRead(pin);
    return map(valeurBrute, 0, 1023, temperatureMin * 100, temperatureMax * 100) / 100.0;
}