#ifndef LINKEDPARTICLESYSTEM_H
#define LINKEDPARTICLESYSTEM_H

#include "AdvancedParticleSystem.h"
#include "PersonnalizedSprite.h"

class LinkedParticleSystem : public AdvancedParticleSystem
{
    public:
        LinkedParticleSystem(unsigned int count=1, sf::PrimitiveType typeVertices=sf::Points, bool applyCourant=false);
        virtual ~LinkedParticleSystem();

        void update(sf::Time frameTime);

        void setCharacRef(PersonnalizedSprite& character);

        void display()
        {
            std::cout << std::endl << "display particle " << std::endl;
            if(_charac==NULL)
                std::cout << "Charac null" << std::endl;
            else
                std::cout << "Charac non null" << std::endl;
            std::cout << "point diffusion : " << _pointDiffusionRel.x << " " << _pointDiffusionRel.y << std::endl;
        };

    protected:
    private:
        void calculPointDiffusion();

        PersonnalizedSprite* _charac;
        sf::Vector2f _pointDiffusionRel;
};

#endif // LINKEDPARTICLESYSTEM_H
