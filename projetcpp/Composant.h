#ifndef COMPOSANT_H
#define COMPOSANT_H

// Classe de base pour tous les composants
class Composant {
public:
    virtual ~Composant() {}
    virtual void activer() = 0;
    virtual void desactiver() = 0;
};

#endif