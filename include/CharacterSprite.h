#ifndef CHARACTERSPRITE_H
#define CHARACTERSPRITE_H

#include <iostream>
#include "PersonnalizedSprite.h"

class CharacterSprite : public PersonnalizedSprite
{
    public:
        CharacterSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);
        virtual ~CharacterSprite();


        void update(sf::Time frameTime);

        void setTilemap(TileMap& tilemap){_tilemap=&tilemap;};
        void setDirection(int axis, int value);
        sf::Vector2f getDirection(){return _direction;};

        void catched(PersonnalizedSprite* sprite){std::cout << "catched2" << std::endl;};

    protected:
    private:

        TileMap *_tilemap;
        sf::Vector2f _direction;
};

#endif // CHARACTERSPRITE_H
