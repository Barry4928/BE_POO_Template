
#include <Wire.h>
#include <rgb_lcd.h>
#include <Adafruit_SHT31.h>
#include<vector>
#include<iostream>
using namespace std;
// Classe de base pour tous les composants
class Composant {
public:
    virtual ~Composant() {}
    virtual void activer() = 0;
    virtual void desactiver() = 0;
};

// LED
class LED : public Composant {
    int pin;
public:
    explicit LED(int pinLED) : pin(pinLED) {
        pinMode(pin, OUTPUT);
        desactiver();
    }

    void activer() override {
        digitalWrite(pin, HIGH);
    }

    void desactiver() override {
        digitalWrite(pin, LOW);
    }
};

// Buzzer
class Buzzer : public Composant {
    int pin;
public:
    explicit Buzzer(int pinBuzzer) : pin(pinBuzzer) {
        pinMode(pin, OUTPUT);
        desactiver();
    }

    void activer() override {
        digitalWrite(pin, HIGH);
    }

    void desactiver() override {
        digitalWrite(pin, LOW);
    }
};

// Ecran LCD RGB (Grove)
class EcranLCD : public Composant {
    rgb_lcd lcd;
public:
    EcranLCD() {
        lcd.begin(16, 2); // Initialisation de l'écran LCD
        lcd.setRGB(0, 255, 0); // Couleur verte par défaut
    }

    void afficher(const String& message) {
    lcd.clear();
    int newlineIndex = message.indexOf('\n');
    if (newlineIndex != -1) {
        // Si le message contient un saut de ligne, séparez-le en deux parties
        lcd.setCursor(0, 0); // Curseur sur la première ligne
        lcd.print(message.substring(0, newlineIndex)); // Affiche la première partie
        lcd.setCursor(0, 1); // Curseur sur la deuxième ligne
        lcd.print(message.substring(newlineIndex + 1)); // Affiche la deuxième partie
    } else {
        // Si pas de saut de ligne, affiche sur la première ligne uniquement
        lcd.setCursor(0, 0); // Curseur sur la première ligne
        lcd.print(message);
    }
    }

    void changerCouleur(uint8_t r, uint8_t g, uint8_t b) {
        lcd.setRGB(r, g, b);
    }

    void activer() override {}
    void desactiver() override {}
};

// Capteur de température (SHT31)
class CapteurTemperature {
    Adafruit_SHT31 sht31;
public:
    CapteurTemperature() {
        if (!sht31.begin(0x44)) { // Adresse I2C du capteur SHT31
            Serial.println("Erreur: SHT31 non détecté !");
            while (1); // Bloque si le capteur ne fonctionne pas
        }
    }

    float lireTemperature() {
        return sht31.readTemperature();
    }
};

// Bouton
class Bouton {
    int pin;
public:
    explicit Bouton(int pinBouton) : pin(pinBouton) {
        pinMode(pin, INPUT_PULLUP);
    }

    bool estAppuye() const {
        return digitalRead(pin) == HIGH;
    }
};
// Classe Potentiometre
class Potentiometre {
    int pin;
    float temperatureMin;
    float temperatureMax;

public:
    Potentiometre(int pinPot, float tempMin, float tempMax)
        : pin(pinPot), temperatureMin(tempMin), temperatureMax(tempMax) {
        pinMode(pin, INPUT); // Configure la broche comme une entrée
    }

    // Méthode pour lire la température réglée
    float lireTemperatureReglee() const {
        int valeurBrute = analogRead(pin);
        return map(valeurBrute, 0, 1023, temperatureMin * 100, temperatureMax * 100) / 100.0; 
        // Division par 100 pour un résultat float
    }
};


// Contrôleur principal
class SystemController {
    vector<Composant*> composants;//vecteur pour stocker les composants a activer et desactiver
    LED led;
    Buzzer buzzer;
    EcranLCD ecran;
    CapteurTemperature capteur;
    Bouton bouton;
    Potentiometre potentiometre;
    const float TEMP_ALERTE = 21; // Température seuil d'alerte
    const int TEMPS_ATTENTE = 15000; // Temps pour entrer le code (15 secondes)

    unsigned long tempsDebutAlerte = 0;
    bool boutonAppuye = false;

    enum class Etat { NORMAL, ALERTE, RING };
    Etat etat;

public:
    SystemController(int pinLED, int pinBuzzer, int pinBouton,int pinPot)
        : led(pinLED), buzzer(pinBuzzer), bouton(pinBouton), potentiometre(pinPot,7.0,21.0),etat(Etat::NORMAL) {
    *this + new LED(pinLED) + new Buzzer(pinBuzzer);

        }
    SystemController& operator+(Composant* composant) {
    composants.push_back(composant); // Ajoute le composant au vecteur
    return *this;                    // Retourne l'objet pour un chaînage éventuel
}

    ~SystemController(){
      //destructeur
      for(auto composant: composants){
        delete  composant;
      }
    }
    void initialiser() {
       for(auto composant: composants){
        composant->desactiver();
       }
        ecran.afficher("System Init...");
        delay(2000);
        ecran.afficher("Pret !");
        delay(2000);
    }
      void activerComposants() {
        for (auto composant : composants) {
            composant->activer();
        }
    }

    void desactiverComposants() {
        for (auto composant : composants) {
            composant->desactiver();
        }
    }

void lireTemperatureReglee() {
    float temperatureReglee = potentiometre.lireTemperatureReglee();
    ecran.afficher("Temp Reglage: \n" + String(temperatureReglee) + " C");
}

void verifierEtat() {
    float temperature = capteur.lireTemperature();
    ecran.afficher("Temp: " + String(temperature) + " C");

    // Si la température dépasse le seuil et que l'état n'est pas déjà ALERTE
    if (temperature >= TEMP_ALERTE) {
        if (etat != Etat::ALERTE && etat != Etat::RING) {
            etat = Etat::ALERTE;
            tempsDebutAlerte = millis(); // Démarre ou réinitialise le timer pour l'alerte
            ecran.afficher("Alerte: regler\n la temperature!");
            ecran.changerCouleur(255, 165, 0); // Orange
            led.activer(); // Allume la LED
        }
    } else if (temperature < TEMP_ALERTE && etat == Etat::ALERTE) {
        // La température redescend sous le seuil, mais on ne passe pas à NORMAL immédiatement
        etat = Etat::NORMAL;
        buzzer.desactiver();
        led.desactiver();
        ecran.afficher("Systeme Normal");
        ecran.changerCouleur(0, 255, 0); // Vert
    }
}


void gererAlerte() {
    // Calcul du temps restant pour le compte à rebours
    unsigned long tempsRestant = (TEMPS_ATTENTE - (millis() - tempsDebutAlerte)) / 1000; // Conversion en secondes

    // Si la température dépasse 30°C et que le timer a été redémarré
    if (capteur.lireTemperature() >= TEMP_ALERTE && etat == Etat::ALERTE) {
        // Affichage du temps restant sur l'écran LCD
        if (tempsRestant > 0 && !boutonAppuye) {
            ecran.afficher("Temps restant:"+String(tempsRestant)+"s");
        }
    }

    // Si le bouton est appuyé avant la fin du timer
    if (bouton.estAppuye() && tempsRestant > 0) {
        boutonAppuye = true;
        etat = Etat::NORMAL; // Passer à l'état normal
        ecran.afficher("Alerte Arretee");
        ecran.changerCouleur(0, 255, 0); // Vert
        led.desactiver(); // Éteindre la LED
        buzzer.desactiver(); // Désactiver le buzzer
        // Lire la température réelle après avoir arrêté l'alerte
        float temperatureReelle = capteur.lireTemperature();
        ecran.afficher("Temp: " + String(temperatureReelle) + " C");
        delay(2000); // Attente avant de revenir à l'état normal
        return; // Quitter la fonction avant que le timer ne se termine
    }

    // Si 15 secondes sont écoulées sans appuyer sur le bouton
    if (tempsRestant <= 0 && !boutonAppuye) {
        ecran.afficher("ALARME: Appuyez!");
        ecran.changerCouleur(255, 0, 0); // Rouge
        led.activer();    // Activer la LED
        buzzer.activer(); // Activer le buzzer
    }

    // Vérifie si 15 secondes sont écoulées sans appuyer sur le bouton, passer à l'état RING
    if (millis() - tempsDebutAlerte >= TEMPS_ATTENTE && !boutonAppuye) {
        etat = Etat::RING;
    }
}



    void gererRing() {
        ecran.afficher("ALARME! Appuyez");
        ecran.changerCouleur(255, 0, 0); // Rouge
        buzzer.activer();

        if (bouton.estAppuye()) {
            boutonAppuye = true;
            etat = Etat::NORMAL;
            ecran.afficher("Alerte Arretee");
            ecran.changerCouleur(0, 255, 0); // Vert
            buzzer.desactiver();  // Désactiver le buzzer
            led.desactiver();     // Éteindre la LED
            // Lire la température réelle après avoir arrêté l'alarme
            float temperatureReelle = capteur.lireTemperature();
            ecran.afficher("Temp: " + String(temperatureReelle) + " C");
            delay(2000);
        }
    }

void mettreAJour() {
        switch (etat) {
        case Etat::NORMAL:
            verifierEtat();
            desactiverComposants(); // Désactiver tous les composants
            break;
        case Etat::ALERTE:
            gererAlerte();
            break;
        case Etat::RING:
            gererRing();
            break;
        }
    }




};



// Broches matérielles pour l'ESP8266
const int PIN_LED = D5;       // GPIO14 pour la LED
const int PIN_BUZZER = D6;    // GPIO12 pour le buzzer
const int PIN_BOUTON = D7;    // GPIO13 pour le bouton
const int PIN_POT = A0;
SystemController controleur(PIN_LED, PIN_BUZZER, PIN_BOUTON,PIN_POT);

void setup() {
    Serial.begin(115200);
    controleur.initialiser();
}

void loop() {
    controleur.lireTemperatureReglee();
    controleur.mettreAJour();
    delay(100);
}