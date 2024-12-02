#ifndef SYSTEMCONTROLLER_H
#define SYSTEMCONTROLLER_H

#include <vector>
#include "LED.h"
#include "Buzzer.h"
#include "EcranLCD.h"
#include "CapteurTemperature.h"
#include "Bouton.h"
#include "Potentiometre.h"

class SystemController {
    std::vector<Composant*> composants;
    LED led;
    Buzzer buzzer;
    EcranLCD ecran;
    CapteurTemperature capteur;
    Bouton bouton;
    Potentiometre potentiometre;

    const float TEMP_ALERTE = 21;
    const int TEMPS_ATTENTE = 15000;

    unsigned long tempsDebutAlerte = 0;
    bool boutonAppuye = false;

    enum class Etat { NORMAL, ALERTE, RING };
    Etat etat;

public:
    SystemController(int pinLED, int pinBuzzer, int pinBouton, int pinPot);
    ~SystemController();
    SystemController& operator+(Composant* composant);
    void initialiser();
    void mettreAJour();
};

#endif
