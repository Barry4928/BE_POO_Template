#include "EcranLCD.h"

EcranLCD::EcranLCD() {
    lcd.begin(16, 2);
    lcd.setRGB(0, 255, 0);
}

void EcranLCD::afficher(const String& message) {
    lcd.clear();
    int newlineIndex = message.indexOf('\n');
    if (newlineIndex != -1) {
        lcd.setCursor(0, 0);
        lcd.print(message.substring(0, newlineIndex));
        lcd.setCursor(0, 1);
        lcd.print(message.substring(newlineIndex + 1));
    } else {
        lcd.setCursor(0, 0);
        lcd.print(message);
    }
}

void EcranLCD::changerCouleur(uint8_t r, uint8_t g, uint8_t b) {
    lcd.setRGB(r, g, b);
}

void EcranLCD::activer() {}

void EcranLCD::desactiver() {}
