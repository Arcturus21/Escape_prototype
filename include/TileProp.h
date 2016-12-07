#ifndef TILEPROP_H
#define TILEPROP_H

#include <iostream>
#include <SFML/Graphics.hpp>

class TileProp
{
    public:
        TileProp(sf::IntRect rect, bool solide=false);
        TileProp(sf::Vector2f position, sf::Vector2f dimension, bool solide=false);
        virtual ~TileProp();

        bool isSolide(){return _solide;};
        void setSolide(bool solide){_solide=solide;};
        sf::IntRect getRect(){return _rect;};

    protected:
    private:

        bool _solide;
        sf::IntRect _rect;
};

#endif // TILEPROP_H
