#include "LED.h"

LED::LED(int pinLED) : pin(pinLED) {
    pinMode(pin, OUTPUT);
    desactiver();
}

void LED::activer() {
    digitalWrite(pin, HIGH);
}

void LED::desactiver() {
    digitalWrite(pin, LOW);
}