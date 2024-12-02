#ifndef CAPTEURTEMPERATURE_H
#define CAPTEURTEMPERATURE_H

#include <Adafruit_SHT31.h>
#include <Arduino.h>

class CapteurTemperature {
    Adafruit_SHT31 sht31;

public:
    CapteurTemperature();
    float lireTemperature();
};

#endif