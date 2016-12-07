#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "PersonnalizedSprite.h"
#include "LinkedParticleSystem.h"
#include "TileMap.h"
#include "CrashTurn.h"
#include "Updatable.h"

typedef enum valRetourLevel{
    level_victoire,
    level_defaite,
    level_quitter
}valRetourLevel;

class Level
{
    public:
        Level();
        virtual ~Level();

        bool load(std::string fichier);
        void reset();

        int play(sf::RenderWindow& app);

        void update(sf::Time frameTime);
        void render(sf::RenderTarget& app);

        CharacterSprite* getCharacJoueur(){return &_joueur;};
    protected:

    private:
        bool loadTexture(std::ifstream& flux);
        bool loadMap(std::ifstream& flux);
        bool loadSprite(std::ifstream& flux);
        bool loadParticle(std::ifstream& flux);
        bool loadCheckpoint(std::ifstream& flux);
        bool loadScript(std::ifstream& flux);

        std::vector<sf::Texture> _listTextures;
        std::vector<PersonnalizedSprite> _listSprite;
        CharacterSprite _joueur;
        TileMap _tilemap;

        std::vector<LinkedParticleSystem> _listParticle;
        std::vector<Checkpoint> _listCheckpoint;

        ///IA ET OPPONENT
        std::vector<CrashTurn> _listCrashTurn;

        ///Général
        std::vector<Updatable*> _listUpdatable;
};

#endif // LEVEL_H
