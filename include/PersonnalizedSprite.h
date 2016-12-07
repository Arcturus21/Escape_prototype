#ifndef PERSONNALIZEDSPRITE_H
#define PERSONNALIZEDSPRITE_H

#include <fstream>

#include "AnimatedSprite.h"
//#include "TileMap.h"

#define SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))
#define ABS(X) ((((X)<0)?(-(X)):(X)))

class TileMap;

typedef enum EtatPerso{
    reposGauche,
    reposDroite,
    reposHaut,
    reposBas,
    marcheGaucheBas,
    marcheGauche,
    marcheGaucheHaut,
    marcheHaut,
    marcheDroiteHaut,
    marcheDroite,
    marcheDroiteBas,
    marcheBas
}EtatPerso;

class PersonnalizedSprite : public AnimatedSprite
{
    public:
        explicit PersonnalizedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);
        virtual ~PersonnalizedSprite();

        bool load(std::string cheminFichier, sf::Texture& texture);

        int deplacer(TileMap& tileMap, sf::Vector2f& vecteur);
        int moveTo(sf::Vector2f positionObj, TileMap& tilemap);

        void setEtat(int etat);
        void addAnimation(Animation* anim, sf::Time frameTime);
        void playAnim(){play(*_tabAnimations[_etatActuel]);};

        sf::Vector2f getCenter() const
            {return sf::Vector2f(getPosition().x+getLocalBounds().width, getPosition().y+getLocalBounds().height);};

        sf::Vector2f& getVectDepl(){return _vectDepl;};
        void setVectDepl(sf::Vector2f vectDepl);

        void update(sf::Time frameTime){AnimatedSprite::update(frameTime);};

        void catched(PersonnalizedSprite* sprite);

        static const float WALK_SPEED;
        static const float VCHUTE_MAX;
        static const float DEFAULT_ACCELERATION;

    protected:
        sf::Vector2f calculPosition(sf::Vector2f movement);
        int essaiDeplacement(TileMap& tileMap, sf::Vector2f vecteur);
        void affine(TileMap& tileMap, sf::Vector2f vecteur);

        ///Gestion des etats et du changement d'animation
        int _nbEtat;
        int _etatActuel;
        std::vector<Animation*> _tabAnimations;
        std::vector<sf::Time> _tabFrameTime;

        sf::Vector2f _vectDepl;
        float _poids;
        float _speed;
        bool _peutBouger;

    private:
};

#endif // PERSONNALIZEDSPRITE_H
