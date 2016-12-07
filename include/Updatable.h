#ifndef UPDATABLE_H
#define UPDATABLE_H

#include <SFML/System.hpp>
#include <iostream>

class Updatable
{
    public:
        Updatable();
        virtual ~Updatable();

        virtual void update(sf::Time frameTime) = 0;

    protected:
    private:
};

#endif // UPDATABLE_H
