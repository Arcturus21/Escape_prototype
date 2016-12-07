#include "../include/AdvancedParticleSystem.h"

using namespace std;
using namespace sf;

AdvancedParticleSystem::AdvancedParticleSystem(unsigned int count, sf::PrimitiveType typeVertices, bool applyCourant):
    ParticleSystem(count), _applyCourant(applyCourant), _typeVertice(typeVertices)
{
    /*m_vertices.setPrimitiveType(_typeVertice);
    if(m_vertices.getVertexCount()%Particle::getTailleFigure(_typeVertice)!=0)
    {
        m_vertices.resize(m_vertices.getVertexCount()-(m_vertices.getVertexCount()%Particle::getTailleFigure(_typeVertice)));
        m_particles.resize(m_vertices.getVertexCount());
    }*/

    for(size_t i = 0; i<m_particles.size(); ++i)
    {
        m_particles[i].setPrimitiveType(typeVertices);
        m_particles[i].setRotAngle(5);
    }

    _courant.x=_courant.y=0;
    _texture=NULL;
    _started = true;
}

AdvancedParticleSystem::AdvancedParticleSystem(AdvancedParticleSystem& mod) :
    ParticleSystem(mod.m_particles.size())
{
    _applyCourant=mod._applyCourant;
    _typeVertice=mod._typeVertice;
    _color=mod._color;

        for(size_t i = 0; i<m_particles.size(); ++i)
    {
        m_particles[i].setPrimitiveType(_typeVertice);
        m_particles[i].setRotAngle(5);
    }

    _courant.x=_courant.y=0;
    _texture=NULL;
    _started = true;
}

AdvancedParticleSystem::AdvancedParticleSystem()
{
    //ctor
}

AdvancedParticleSystem::~AdvancedParticleSystem()
{
    //dtor
}

bool AdvancedParticleSystem::setPropagationAngle(float angleMin, float angleMax)
{
    if(angleMin < 0 || angleMin>360 || angleMax < angleMin ||angleMax > 360)
        return false;
    _angle.x = angleMin;
    _angle.y = angleMax;
    return true;
}

void AdvancedParticleSystem::setSpeed(int speed)
{
    _speed = speed;
}

void AdvancedParticleSystem::setTexture(sf::Texture* texture)
{
    _texture=texture;
    int taille=Particle::getTailleFigure(_typeVertice);
    for(unsigned int i=0; i<m_vertices.getVertexCount(); i+=taille)
    {
        m_vertices[i].texCoords = sf::Vector2f(0,0);
        if(taille>1)
        {
            m_vertices[i+1].texCoords = sf::Vector2f(0,_texture->getSize().y);
            if(taille>2)
            {
                m_vertices[i+2].texCoords = sf::Vector2f(_texture->getSize().x,_texture->getSize().y);
                if(taille>3)
                    m_vertices[i+3].texCoords = sf::Vector2f(_texture->getSize().x,0);
            }
        }
    }
}

void AdvancedParticleSystem::update(sf::Time elapsed)
{
    //static int taille=Particle::getTailleFigure(_typeVertice);

    for(size_t i = 0; i<m_particles.size(); ++i)
    {
        /// on met à jour la durée de vie de la particule
        Particle& p = m_particles[i];
        p._lifetime -= elapsed;

        /// si la particule est arrivée en fin de vie, on la réinitialise
        if(p._lifetime <= sf::Time::Zero)
        {
            resetParticle(i);
            //i+=taille-1;
            continue;
        }

        if(_applyCourant)
        {
            p._velocity.x+=_courant.x* elapsed.asSeconds();
            p._velocity.y+=_courant.y* elapsed.asSeconds();
        }
        /// on met à jour la position du vertex correspondant
        p.update(elapsed, m_lifetime.asSeconds());
        m_vertices[i].position += p._velocity * elapsed.asSeconds();

        /// on met à jour l'alpha (transparence) de la particule en fonction de sa durée de vie
        float ratio = p._lifetime.asSeconds() / m_lifetime.asSeconds();
        m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
        /*m_vertices[i].color.r = static_cast<sf::Uint8>(m_vertices[i].color.r-ratio*255);
        m_vertices[i].color.g = static_cast<sf::Uint8>(m_vertices[i].color.g-ratio*255);
        m_vertices[i].color.b = static_cast<sf::Uint8>(m_vertices[i].color.b-ratio*255);*/
    }
}

void AdvancedParticleSystem::start()
{
    _started=true;
    for(size_t i = 0; i<m_particles.size(); ++i)
    {
        Particle& p = m_particles[i];
        if(p._lifetime <= sf::Time::Zero)
        {
            resetParticle(i);
        }
    }

}
