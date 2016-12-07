#include "CharacterSprite.h"

CharacterSprite::CharacterSprite(sf::Time frameTime, bool paused, bool looped) :
PersonnalizedSprite(frameTime, paused, looped)
{
    //ctor
    _etatActuel=reposBas;
    _direction.x=_direction.y=0;  ///?
    _tilemap=NULL;
}

CharacterSprite::~CharacterSprite()
{
    //dtor
}

void CharacterSprite::setDirection(int axis, int value)
{
    if(axis==0)
        _direction.x=value;
    else if(axis==1)
        _direction.y=value;
}

void CharacterSprite::update(sf::Time frameTime)
{
    ///Partie mouvements
    _vectDepl.x = _direction.x * WALK_SPEED;
    _vectDepl.y = _direction.y * WALK_SPEED;
    _vectDepl *= frameTime.asSeconds();
    setVectDepl(_vectDepl);

    deplacer(*_tilemap, _vectDepl);

    ///Partie animation
    if(_direction.x == -1)   /** TEMPORAIRE */
    {
        if(_direction.y == -1)
        {
            setEtat(marcheGaucheHaut);
        }
        else if(_direction.y == 0)
        {
            setEtat(marcheGauche);
        }
        else
        {
            setEtat(marcheGaucheBas);
        }
    }
    else if(_direction.x == 1)
    {
        if(_direction.y == -1)
        {
            setEtat(marcheDroiteHaut);
        }
        else if(_direction.y == 0)
        {
            setEtat(marcheDroite);
        }
        else
        {
            setEtat(marcheDroiteBas);
        }
    }
    else
    {
        if(_direction.y == -1)
        {
            setEtat(marcheHaut);
        }
        else if(_direction.y == 1)
        {
            setEtat(marcheBas);
        }
        else
        {
            if(_etatActuel==marcheHaut)
                setEtat(reposHaut);
            else if(_etatActuel==marcheBas)
                setEtat(reposBas);
            else if(_etatActuel==marcheGauche || _etatActuel==marcheGaucheHaut || _etatActuel==marcheGaucheBas)
                setEtat(reposGauche);
            else if(_etatActuel==marcheDroite || _etatActuel==marcheDroiteHaut || _etatActuel==marcheDroiteBas)
                setEtat(reposDroite);
        }
    }
    playAnim();
    AnimatedSprite::update(frameTime);
}
