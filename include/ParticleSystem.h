#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <math.h>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "Updatable.h"


///Note perso : ajouter gestion paralaxe?

/*typedef struct Particle
{
    sf::Vector2f velocity;
    sf::Time lifetime;
}Particle;*/

class ParticleSystem : public sf::Drawable, public sf::Transformable, public Updatable
{
    public:
        ParticleSystem(unsigned int count);
        ParticleSystem();
        virtual ~ParticleSystem();

        void setEmitter(sf::Vector2f position){m_emitter=position;}

        void setLifetime(sf::Time lifetime){m_lifetime=lifetime;};
        void setColor(sf::Color color);
        sf::Color getColor(){return _color;};

        void update(sf::Time elapsed);

    protected:

        std::vector<Particle> m_particles;
        sf::VertexArray m_vertices;
        sf::Time m_lifetime;
        sf::Vector2f m_emitter;
        sf::Color _color;

        sf::Vector2f _angle;
        float _speed;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            /// on applique la transformation
            states.transform *= getTransform();

            /// nos particules n'utilisent pas de texture
            states.texture=NULL;

            /// on dessine enfin le vertex array
            target.draw(m_vertices, states);
        }


        void resetParticle(std::size_t index)
        {
            /// on donne une vitesse et une durée de vie aléatoires à la particule
            float angle = ((std::rand()%(int)(_angle.y-_angle.x))+_angle.x) * 3.14f / 180.f;
            float speed = (std::rand()%(int)_speed) + _speed;

            m_particles[index].reset(sf::Vector2f(cos(angle) * speed, sin(angle) * speed), sf::milliseconds((std::rand()%2000) + 1000), m_emitter);

            /// on remet à zéro la position du vertex correspondant à la particule
            m_vertices[index].position=m_emitter;
            m_vertices[index].color = _color;
        }
    private:

};

#endif // PARTICLESYSTEM_H
