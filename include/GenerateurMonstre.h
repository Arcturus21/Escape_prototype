#ifndef GENERATEURMONSTRE_H
#define GENERATEURMONSTRE_H

#include <SFML/Graphics.hpp>

#include "Updatable.h"
#include "CrashTurn.h"
#include "LinkedParticleSystem.h"


class GenerateurMonstre : public Updatable
{
    public:
        GenerateurMonstre(sf::Vector2f pos, int nbMonstre,
                          CrashTurn& modele, LinkedParticleSystem& modeleParticle,
                          std::vector<CrashTurn*>& listMonstres, std::vector<AdvancedParticleSystem*>& listParticle, std::vector<Updatable*>& listUpdatable);
        virtual ~GenerateurMonstre();

        void update(sf::Time elapsed);
        bool genereMonstre();

    protected:
    private:

        CrashTurn _modele;
        LinkedParticleSystem _modeleParticle;
        sf::Vector2f _emitter;

        std::vector<CrashTurn*>* _listMonstresLevel;
        std::vector<AdvancedParticleSystem*>* _linkedParticle;
        std::vector<Updatable*>* _listUpdatable;

        int _nbMonstre, _nbMonstreGenerer;
        sf::Time _timeElapsed;

};

#endif // GENERATEURMONSTRE_H
