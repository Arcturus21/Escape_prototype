#include "../include/ParticleSystem.h"

using namespace std;
using namespace sf;

ParticleSystem::ParticleSystem(unsigned int count) :
    m_particles(count), m_vertices(sf::Points, count), m_lifetime(seconds(1)), m_emitter(0,0)
{
    //ctor
    _color=Color::White;
    _speed=50;
    _angle.x=0;
    _angle.y=360;
}

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{
    //dtor
}

void ParticleSystem::setColor(sf::Color color)
{
    /*for(int i=0; i<m_particles.size(); ++i)
    {
        m_vertices[i].color=color;
    }*/
    _color=color;
}

void ParticleSystem::update(sf::Time elapsed)
{
    for(size_t i = 0; i<m_particles.size(); ++i)
    {
        /// on met à jour la durée de vie de la particule
        Particle& p = m_particles[i];
        p._lifetime -= elapsed;

        /// si la particule est arrivée en fin de vie, on la réinitialise
        if(p._lifetime <= sf::Time::Zero)
            resetParticle(i);

        /// on met à jour la position du vertex correspondant
        m_vertices[i].position += p._velocity * elapsed.asSeconds();

        /// on met à jour l'alpha (transparence) de la particule en fonction de sa durée de vie
        float ratio = p._lifetime.asSeconds() / m_lifetime.asSeconds();
        m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
    }
}
