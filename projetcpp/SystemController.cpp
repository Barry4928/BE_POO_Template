
#include "SystemController.h"

SystemController::SystemController(int pinLED, int pinBuzzer, int pinBouton, int pinPot)
    : led(pinLED), buzzer(pinBuzzer), bouton(pinBouton), potentiometre(pinPot, 7.0, 21.0), etat(Etat::NORMAL) {
    *this + new LED(pinLED) + new Buzzer(pinBuzzer);
}

SystemController& SystemController::operator+(Composant* composant) {
    composants.push_back(composant);
    return *this;
}

SystemController::~SystemController() {
    for (auto composant : composants) {
        delete composant;
    }
}

void SystemController::initialiser() {
    for (auto composant : composants) {
        composant->desactiver();
    }
    ecran.afficher("System Init...");
    delay(2000);
    ecran.afficher("Pret !");
    delay(2000);
}

void SystemController::activerComposants() {
    for (auto composant : composants) {
        composant->activer();
    }
}

void SystemController::desactiverComposants() {
    for (auto composant : composants) {
        composant->desactiver();
    }
}

void SystemController::lireTemperatureReglee() {
    float temperatureReglee = potentiometre.lireTemperatureReglee();
    ecran.afficher("Temp Reglage: \n" + String(temperatureReglee) + " C");
}

void SystemController::verifierEtat() {
    float temperature = capteur.lireTemperature();
    ecran.afficher("Temp: " + String(temperature) + " C");

    if (temperature >= TEMP_ALERTE) {
        if (etat != Etat::ALERTE && etat != Etat::RING) {
            etat = Etat::ALERTE;
            tempsDebutAlerte = millis();
            ecran.afficher("Alerte: regler\n la temperature!");
            ecran.changerCouleur(255, 165, 0);
            led.activer();
        }
    } else if (temperature < TEMP_ALERTE && etat == Etat::ALERTE) {
        etat = Etat::NORMAL;
        buzzer.desactiver();
        led.desactiver();
        ecran.afficher("Systeme Normal");
        ecran.changerCouleur(0, 255, 0);
    }
}

void SystemController::gererAlerte() {
    unsigned long tempsRestant = (TEMPS_ATTENTE - (millis() - tempsDebutAlerte)) / 1000;

    if (capteur.lireTemperature() >= TEMP_ALERTE && etat == Etat::ALERTE) {
        if (tempsRestant > 0 && !boutonAppuye) {
            ecran.afficher("Temps restant:" + String(tempsRestant) + "s");
        }
    }

    if (bouton.estAppuye() && tempsRestant > 0) {
        boutonAppuye = true;
        etat = Etat::NORMAL;
        ecran.afficher("Alerte Arretee");
        ecran.changerCouleur(0, 255, 0);
        led.desactiver();
        buzzer.desactiver();
        float temperatureReelle = capteur.lireTemperature();
        ecran.afficher("Temp: " + String(temperatureReelle) + " C");
        delay(2000);
        return;
    }

    if (tempsRestant <= 0 && !boutonAppuye) {
        ecran.afficher("ALARME: Appuyez!");
        ecran.changerCouleur(255, 0, 0);
        led.activer();
        buzzer.activer();
    }

    if (millis() - tempsDebutAlerte >= TEMPS_ATTENTE && !boutonAppuye) {
        etat = Etat::RING;
    }
}

void SystemController::gererRing() {
    ecran.afficher("ALARME! Appuyez");
    ecran.changerCouleur(255, 0, 0);
    buzzer.activer();

    if (bouton.estAppuye()) {
        boutonAppuye = true;
        etat = Etat::NORMAL;
        ecran.afficher("Alerte Arretee");
        ecran.changerCouleur(0, 255, 0);
        buzzer.desactiver();
        led.desactiver();
        float temperatureReelle = capteur.lireTemperature();
        ecran.afficher("Temp: " + String(temperatureReelle) + " C");
        delay(2000);
    }
}

void SystemController::mettreAJour() {
    switch (etat) {
    case Etat::NORMAL:
        verifierEtat();
        desactiverComposants();
        break;
    case Etat::ALERTE:
        gererAlerte();
        break;
    case Etat::RING:
        gererRing();
        break;
    }
}
