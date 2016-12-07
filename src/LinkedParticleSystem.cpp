#include "LinkedParticleSystem.h"

using namespace sf;

LinkedParticleSystem::LinkedParticleSystem(unsigned int count, sf::PrimitiveType typeVertices, bool applyCourant) :
    AdvancedParticleSystem(count, typeVertices, applyCourant)
{
    //ctor
}

LinkedParticleSystem::~LinkedParticleSystem()
{
    //dtor
}

void LinkedParticleSystem::setCharacRef(PersonnalizedSprite& character)
{
    _charac = &character;
    calculPointDiffusion();
}

void LinkedParticleSystem::calculPointDiffusion()
{
    FloatRect temp = _charac->getLocalBounds();
    _pointDiffusionRel.x = temp.width*_charac->getScale().x/3;
    _pointDiffusionRel.y = temp.height*_charac->getScale().y;
    _pointDiffusionRel.y -= _pointDiffusionRel.y/3;
}

void LinkedParticleSystem::update(sf::Time frameTime)
{
    if(_charac->getVectDepl().x == 0 && _charac->getVectDepl().y == 0)
        stop();
    else if(!isStarted())
        start();
    Vector2f posPerso = _charac->getPosition();
    posPerso.x += _pointDiffusionRel.x;
    posPerso.y += _pointDiffusionRel.y;
    setEmitter(posPerso);
    AdvancedParticleSystem::update(frameTime);
}
