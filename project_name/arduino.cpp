#include <Arduino.h>        // Bibliothèque de base pour Arduino
#include <string>

using namespace std;

enum class Etat { OFF, READY, ALERT };

// Composant de base pour l'ESP8266
class Composant {
public:
    virtual ~Composant() {}
    virtual void activer() = 0;
    virtual void desactiver() = 0;
};

// LED avec gestion de couleur
class LED : public Composant {
    int pin;
    string couleur;
public:
    explicit LED(int pinLED) : pin(pinLED) { pinMode(pin, OUTPUT); }

    void activer() override { digitalWrite(pin, HIGH); }
    void desactiver() override { digitalWrite(pin, LOW); }

    void setCouleur(const string& newColor) {
        couleur = newColor;
        Serial.println("LED couleur: " + String(couleur.c_str()));
        activer();
    }
};

// Buzzer simple
class Buzzer : public Composant {
    int pin;
public:
    explicit Buzzer(int pinBuzzer) : pin(pinBuzzer) { pinMode(pin, OUTPUT); }

    void activer() override { digitalWrite(pin, HIGH); }
    void desactiver() override { digitalWrite(pin, LOW); }
};

// Ecran simulé sur le port série
class Ecran {
public:
    void afficher(const string& message) { Serial.println("Affiche: " + String(message.c_str())); }
};

// Bouton pour contrôle de l'état
class Bouton {
    int pin;
public:
    explicit Bouton(int pinBouton) : pin(pinBouton) { pinMode(pin, INPUT_PULLUP); }
    bool estAppuye() const { return digitalRead(pin) == LOW; }  // Bouton appuyé si LOW
};

// Contrôleur général du système
class SystemController {
    Etat etat;
    Ecran ecran;
    LED led;
    Buzzer buzzer;
    Bouton bouton;

    unsigned long lastCheckTime;
    const unsigned long checkInterval = 2000;

public:
    SystemController(int pinLED, int pinBuzzer, int pinBouton) 
        : etat(Etat::OFF), led(pinLED), buzzer(pinBuzzer), bouton(pinBouton), lastCheckTime(0) {}

    void initialiser() {
        led.desactiver();
        buzzer.desactiver();
        ecran.afficher("Système initialisé.");
    }

    bool verifierEtat() {
        if (!bouton.estAppuye()) {
            Serial.println("Erreur : Bouton non appuyé!");
            return false;
        }
        etat = Etat::READY;
        return true;
    }

    void mettreAJour() {
        unsigned long currentTime = millis();
        if (currentTime - lastCheckTime >= checkInterval) {
            lastCheckTime = currentTime;

            if (!verifierEtat()) {
                etat = Etat::ALERT;
            }
        }

        switch (etat) {
            case Etat::OFF:
                ecran.afficher("OFF");
                led.setCouleur("Rouge");
                buzzer.desactiver();
                break;
            case Etat::READY:
                ecran.afficher("READY");
                led.setCouleur("Vert");
                buzzer.desactiver();
                break;
            case Etat::ALERT:
                ecran.afficher("ALERT");
                led.setCouleur("Rouge");
                buzzer.activer();
                break;
        }
    }
};

// Configuration matérielle
const int PIN_LED = 5;    // GPIO 5
const int PIN_BUZZER = 4; // GPIO 4
const int PIN_BOUTON = 0; // GPIO 0

SystemController controleur(PIN_LED, PIN_BUZZER, PIN_BOUTON);

void setup() {
    Serial.begin(115200);
    controleur.initialiser();
}

void loop() {
    controleur.mettreAJour();
    delay(100);
}
