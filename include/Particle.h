#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Particle : public sf::Transformable, public sf::Drawable
{
    public:
        Particle(sf::Vector2f& velocity, sf::Time& lifetime, sf::PrimitiveType typeVertices);
        Particle();
        virtual ~Particle();

        void reset(sf::Vector2f velocity, sf::Time lifetime, sf::Vector2f emitter);

        void setVelocity(sf::Vector2f velocity){_velocity=velocity;};
        sf::Vector2f getVelocity(){return _velocity;};
        void setLifetime(sf::Time lifetime){_lifetime=lifetime;};
        sf::Time getLifetime(){return _lifetime;};
        void setPrimitiveType(sf::PrimitiveType typeVertices){_vertices.setPrimitiveType(typeVertices);
                                                                _vertices.resize(Particle::getTailleFigure(typeVertices));};
        void setColor(sf::Color color);
        void setRotAngle(float angle){_rotAngle = angle;};

        void update(sf::Time elapsed, float lifetime);

        void setVisible(bool visible){_visible=visible;};
        bool isVisible(){return _visible;};

        ///Variable public hérité de la structure précédente
        sf::Vector2f _velocity;
        sf::Time _lifetime;
        sf::VertexArray _vertices;

        float _rotAngle;

        static int getTailleFigure(sf::PrimitiveType type)
        {
            int taille;

            switch(type)
            {
            case sf::Points:
                taille=1;
                break;
            case sf::Lines:
            case sf::LinesStrip:
                taille=2;
                break;
            case sf::Triangles:
            case sf::TrianglesStrip:
            case sf::TrianglesFan:
                taille=3;
                break;
            default:
                taille=4;
                break;
            }

            return taille;
        }

    protected:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {

            if(_visible==true)
            {
                /// on applique la transformation
                states.transform *= getTransform();


                /// la texture est transmise via les paramètres
                states.texture=NULL;

                /// on dessine enfin le vertex array
                target.draw(_vertices, states);
            }
        }

    private:
        bool _visible;
};

#endif // PARTICLE_H
