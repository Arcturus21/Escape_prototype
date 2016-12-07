#ifndef SALLE_H
#define SALLE_H

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

class Salle
{
    public:
        Salle(int x, int y, int w, int h);
        virtual ~Salle();

        sf::View getViewSalle(){return sf::View(sf::FloatRect(_x, _y, _w, _h));};
        sf::FloatRect getBoundSalle(){return sf::FloatRect(_x, _y, _w, _h);};

    protected:

    private:
        int _x, _y, _w, _h;
};

#endif // SALLE_H
