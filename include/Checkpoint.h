#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <SFML/Graphics.hpp>
#include "PersonnalizedSprite.h"

typedef enum typeCheckpoint
{
    emetteurCrashTurn,
    trigger
}typeCheckpoint;

class Checkpoint : public Updatable
{
    public:
        Checkpoint(typeCheckpoint type, int ind, sf::Vector2f pos, sf::Vector2f dim);
        virtual ~Checkpoint();

        void update(sf::Time frameTime);

        void setPosition(sf::Vector2f pos){_rect.left=pos.x;_rect.top=pos.y;};
        sf::Vector2f getPosition(){return sf::Vector2f(_rect.left, _rect.top);};

        sf::FloatRect getGlobalBounds(){return _rect;};
        int getIndice(){return _indice;};

    protected:
        typeCheckpoint _type;
        int _indice;
        sf::FloatRect _rect;

    private:
};

#endif // CHECKPOINT_H
