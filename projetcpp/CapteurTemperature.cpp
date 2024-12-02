#include "CapteurTemperature.h"

CapteurTemperature::CapteurTemperature() {
    if (!sht31.begin(0x44)) {
        Serial.println("Erreur: SHT31 non détecté !");
        while (1);
    }
}

float CapteurTemperature::lireTemperature() {
    return sht31.readTemperature();
}