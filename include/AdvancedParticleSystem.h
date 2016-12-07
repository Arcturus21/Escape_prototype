#ifndef ADVANCEDPARTICLESYSTEM_H
#define ADVANCEDPARTICLESYSTEM_H

#include "ParticleSystem.h"
#include <iostream>

/*
Faire hériter une classe de ParticleSystem et implémeter:
- gestion gravité
- gestion de polygone plus complexe (vertices colorés, triangles, carrés, +)
- gestion d'affichage de texture sur le même principe? -> tableau de Drawable*
- gestion des collisions?
*/


class AdvancedParticleSystem : public ParticleSystem
{
    public:
        AdvancedParticleSystem(unsigned int count, sf::PrimitiveType typeVertices=sf::Points, bool applyCourant=false);
        AdvancedParticleSystem(AdvancedParticleSystem& mod);
        AdvancedParticleSystem();
        virtual ~AdvancedParticleSystem();

        void setCourant(sf::Vector2f courant){_courant.x=courant.x;
                                                _courant.y=courant.y;};

        bool setPropagationAngle(float angleMin, float angleMax);
        void setSpeed(int speed);

        void update(sf::Time elapsed);

        void setTexture(sf::Texture* texture);
        sf::Texture* getTexture(){return _texture;};

        void start();
        void stop(){_started=false;};
        bool isStarted(){return _started;};

    protected:

        bool _applyCourant;
        sf::PrimitiveType _typeVertice;
        sf::Vector2f _courant;
        sf::Texture *_texture;

        bool _started;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {


            /// on applique la transformation
            states.transform *= getTransform();

            /// nos particules n'utilisent pas de texture
            states.texture=_texture;

            /// on dessine enfin le vertex array
            //target.draw(m_vertices, states);
            for(size_t i=0; i<m_particles.size(); ++i)
                target.draw(m_particles[i], states);
        }

        int resetParticle(std::size_t index)
        {
            int taille=Particle::getTailleFigure(_typeVertice);
            if(true)
            {
                /// on donne une vitesse et une durée de vie aléatoires à la particule
                float angle = ((std::rand()%(int)(_angle.y-_angle.x))+_angle.x) * 3.14f / 180.f;
                float speed = (std::rand()%(int)_speed) + _speed;

                sf::Time lifetime = sf::milliseconds((std::rand()%(int)m_lifetime.asMilliseconds()));
                /*for(int i=0; i<taille; ++i)
                {*/
                    m_particles[index].reset(sf::Vector2f(cos(angle) * speed, sin(angle) * speed), lifetime, m_emitter);
                    if(_started)
                        m_particles[index].setColor(_color);
                    else
                        m_particles[index].setVisible(false);

                    /*m_particles[index+i].setVelocity(sf::Vector2f(cos(angle) * speed, sin(angle) * speed));
                    m_particles[index+i].setLifetime(sf::milliseconds((std::rand()%2000) + 1000));*/

                    /// on remet à zéro la position du vertex correspondant à la particule
                    /*m_vertices[index+i].position=m_emitter;
                    m_vertices[index+i].color = _color;*/
                    //m_vertices[index+i].color = _color;
                    /*quad[i].position = sf::Vector2f(m_emitter.x+30*(i%taille), m_emitter.y+30*(i/taille));
                    std::cout << m_emitter.x+30*(i%taille) << " " << m_emitter.y+30*(i/taille) << std::endl;*/
                /*}
                m_vertices[index].position = m_emitter;
                if(taille>1)
                {
                    m_vertices[index+1].position = sf::Vector2f(m_emitter.x, m_emitter.y+30);
                    if(taille>2)
                    {
                        m_vertices[index+2].position = sf::Vector2f(m_emitter.x+30, m_emitter.y+30);
                        if(taille>3)
                            m_vertices[index+3].position = sf::Vector2f(m_emitter.x+30, m_emitter.y);
                    }

                }*/
            }



            return taille;
        }

    private:
};

#endif // ADVANCEDPARTICLESYSTEM_H
