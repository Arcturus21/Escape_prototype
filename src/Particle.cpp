#include "../include/Particle.h"

using namespace sf;
using namespace std;

Particle::Particle()
{
    //ctor
    _velocity.x=_velocity.y=0;
    _lifetime = Time::Zero;

    _vertices.resize(Particle::getTailleFigure(Points));
    setColor(Color::White);
}

Particle::Particle(sf::Vector2f& velocity, sf::Time& lifetime, sf::PrimitiveType typeVertices)
{
    _velocity=velocity;
    _lifetime=lifetime;
    _velocity.x=_velocity.y=0;
    _vertices.resize(Particle::getTailleFigure(typeVertices));
}

Particle::~Particle()
{
    //dtor
}

void Particle::reset(sf::Vector2f velocity, sf::Time lifetime, sf::Vector2f emitter)
{
    _visible=true;
    _velocity = velocity;
    _lifetime = lifetime;

    int taille = Particle::getTailleFigure(_vertices.getPrimitiveType());
    _vertices[0].position = emitter;
    if(taille>1)
    {
        _vertices[1].position = sf::Vector2f(emitter.x, emitter.y+30);
        if(taille>2)
        {
            _vertices[2].position = sf::Vector2f(emitter.x+30, emitter.y+30);
            if(taille>3)
                _vertices[3].position = sf::Vector2f(emitter.x+30, emitter.y);
        }

    }
}

void Particle::update(sf::Time elapsed, float lifetime)
{
    float ratio = _lifetime.asSeconds() / lifetime;
    for(unsigned int i=0; i<_vertices.getVertexCount(); ++i)
    {
        _vertices[i].position += _velocity * elapsed.asSeconds();

        /// on met à jour l'alpha (transparence) de la particule en fonction de sa durée de vie
        _vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
    }
    /*_vertices[0].position.x += (_vertices[0].position.x - _vertices[1].position.x)/2;
    _vertices[0].position.y += (_vertices[0].position.y - _vertices[1].position.y)/2;
    _vertices[1].position.x += (_vertices[1].position.x - _vertices[2].position.x)/2;
    _vertices[1].position.y += (_vertices[1].position.y - _vertices[2].position.y)/2;
    _vertices[2].position.x += (_vertices[2].position.x - _vertices[3].position.x)/2;
    _vertices[2].position.y += (_vertices[2].position.y - _vertices[3].position.y)/2;
    _vertices[3].position.x += (_vertices[3].position.x - _vertices[0].position.x)/2;
    _vertices[3].position.y += (_vertices[3].position.y - _vertices[0].position.y)/2;*/
    /*_vertices[0].position.y += _rotAngle* elapsed.asSeconds();
    _vertices[1].position.x -= _rotAngle* elapsed.asSeconds();
    _vertices[2].position.y -= _rotAngle* elapsed.asSeconds();
    _vertices[3].position.x += _rotAngle* elapsed.asSeconds();*/

    /*rotate(_rotAngle);
    cout << "rot : " << getRotation() << endl;*/
}

void Particle::setColor(sf::Color color)
{
    for(unsigned int i=0; i<_vertices.getVertexCount(); ++i)
    {
        _vertices[i].color=color;
    }
}
