#include "../include/PersonnalizedSprite.h"
#include "TileMap.h"

using namespace std;
using namespace sf;

const float PersonnalizedSprite::WALK_SPEED = 1600;
const float PersonnalizedSprite::VCHUTE_MAX = 1;
const float PersonnalizedSprite::DEFAULT_ACCELERATION = 1;

PersonnalizedSprite::PersonnalizedSprite(Time frameTime, bool paused, bool looped) : AnimatedSprite(frameTime, paused, looped)
{
    //ctor
    _nbEtat=0;
    _etatActuel=0;
    _poids=10;
    _speed=WALK_SPEED;
    _peutBouger=true;
}

PersonnalizedSprite::~PersonnalizedSprite()
{
    //dtor
}

bool PersonnalizedSprite::load(std::string cheminFichier, sf::Texture& texture)
{
    int num, nbAnim;
    double duree;
    IntRect rect;
    string buf;

    Animation *animation=NULL;

    ifstream flux(cheminFichier.c_str());
    if(!flux)
        return false;

    flux >> num;
    _isPaused = (num==1);
    flux >> num;
    _isLooped = (num==1);
    flux >> duree;
    _frameTime = sf::seconds(duree);
    _texture = &texture;

    flux >> num;
    if(num < 1)
        return false;

    for(int i=0; i<num; i++)
    {
        flux >> buf >> nbAnim;

        animation = new Animation();
        animation->setSpriteSheet(*_texture);
        for(int j=0; j<nbAnim; ++j)
        {
            flux >> rect.left >> rect.top >> rect.width >> rect.height;
            animation->addFrame(rect);
        }
        addAnimation(animation, seconds(0.2));
    }
    setAnimation(*_tabAnimations[0]);
    flux >> num >> nbAnim;
    setPosition(Vector2f(num, nbAnim));
    flux >> num >> nbAnim;
    setScale(Vector2f(num, nbAnim));

}

int PersonnalizedSprite::essaiDeplacement(TileMap& tileMap, Vector2f vecteur)
{
    Vector2f test = calculPosition(vecteur);
    FloatRect rectTest = getGlobalBounds();
    rectTest.left = test.x;
    rectTest.top = test.y;
    if(!tileMap.testCollision(rectTest))
    {
        setPosition(test);
        return 1;
    }
    return 0;
}

void PersonnalizedSprite::affine(TileMap& tileMap, Vector2f vecteur)
{
    int i, sens=1;
    if(vecteur.x<i)
        sens=-1;
    else if(vecteur.x > i)
        sens=1;
    else
        sens=0;
    for(i=0; i<ABS(vecteur.x);++i)
    {
        if(essaiDeplacement(tileMap, Vector2f(SGN(vecteur.x), 0))==0)
            break;
    }
    if(vecteur.y<i)
        sens=-1;
    else if(vecteur.y > i)
        sens=1;
    else
        sens=0;
    for(i=0; i<ABS(vecteur.y);++i)
    {
        if(essaiDeplacement(tileMap, Vector2f(0,SGN(vecteur.y)))==0)
            break;
    }

    /*FloatRect rectTest = getGlobalBounds();
    Vector2f test;
    if(vecteur.x > 0)
    {
        for(float i=vecteur.x; i>0; i-=1)
        {
            test = calculPosition(Vector2f(i, 0));
            rectTest.left = test.x;
            rectTest.top = test.y;
            //if(!tileMap.testCollision(rectTest))
            if(essaiDeplacement(tileMap, Vector2f(i, 0))==0)
            {
                _vectDepl.x = i;
                break;
            }
        }
    }
    else
    {
        for(float i=vecteur.x; i<0; i+=1)
        {
            test = calculPosition(Vector2f(i, 0));
            rectTest.left = test.x;
            rectTest.top = test.y;
            //if(!tileMap.testCollision(rectTest))
            if(essaiDeplacement(tileMap, Vector2f(i, 0))==0)
            {
                _vectDepl.x = i;
                break;
            }
        }
    }
    if(vecteur.y > 0)
    {
        for(float i=vecteur.y; i>0; i-=1)
        {
            test = calculPosition(Vector2f(i, 0));
            rectTest.left = test.x;
            rectTest.top = test.y;
            //if(!tileMap.testCollision(rectTest))
            if(essaiDeplacement(tileMap, Vector2f(0,i))==0)
            {
                _vectDepl.y = i;
                break;
            }
        }
    }
    else
    {
        for(float i=vecteur.y; i<0; i+=1)
        {
            test = calculPosition(Vector2f(i, 0));
            rectTest.left = test.x;
            rectTest.top = test.y;
            //if(!tileMap.testCollision(rectTest))
            if(essaiDeplacement(tileMap, Vector2f(0,i))==0)
            {
                _vectDepl.y = i;
                break;
            }
        }
    }*/
}

int PersonnalizedSprite::deplacer(TileMap& tileMap, Vector2f& vecteur)
{
    Vector2f vectTemp = tileMap.getDimTile();
    if(vecteur.x>=getLocalBounds().width || vecteur.y >= getLocalBounds().height)
    {
        vectTemp.x = vecteur.x/2;
        vectTemp.y = vecteur.y/2;
        deplacer(tileMap, vectTemp);

        vectTemp.x = vecteur.x-vectTemp.x;
        vectTemp.y = vecteur.y-vectTemp.y;
        deplacer(tileMap, vectTemp);
        return 3;
    }
    else if(essaiDeplacement(tileMap, vecteur)==1)
    {
        //setPosition(calculPosition(_vectDepl));
        return 1;
    }
    affine(tileMap, vecteur);
    return 2;
}

Vector2f PersonnalizedSprite::calculPosition(sf::Vector2f movement)
{
    return getPosition()+movement;
}

int PersonnalizedSprite::moveTo(Vector2f positionObj, TileMap& tilemap)
{
    Vector2f movement;
    int direction=0;
    if(positionObj.x < getPosition().x)
        direction=-1;
    else if(positionObj.x > getPosition().x)
        direction=1;
    movement.x += direction;

    if(positionObj.y < getPosition().y)
        direction=-1;
    else if(positionObj.y > getPosition().y)
        direction=1;
    else
        direction=0;
    movement.y += direction;
    if(movement.x > _speed)
        movement.x = _speed;
    if(movement.y > _speed)
        movement.y = _speed;
    return deplacer(tilemap, movement);
}

void PersonnalizedSprite::setEtat(int etat)
{
    if(etat >=0 && etat < _nbEtat && _etatActuel!=etat)
    {
        _etatActuel=etat;
        setAnimation(*_tabAnimations[_etatActuel]);
        setFrameTime(_tabFrameTime[_etatActuel]);
        play();
    }
}

void PersonnalizedSprite::addAnimation(Animation* anim, sf::Time frameTime)
{
    if(anim!=NULL)
    {
        _tabAnimations.push_back(anim);
        _tabFrameTime.push_back(frameTime);
        _nbEtat++;
    }
}

void PersonnalizedSprite::setVectDepl(Vector2f vectDepl)
{
    _vectDepl.x = vectDepl.x;
    _vectDepl.y = vectDepl.y;

    if(_vectDepl.x + _vectDepl.y > WALK_SPEED)
    {
        double ratioSpeed = (_vectDepl.x + _vectDepl.y) / WALK_SPEED;
        _vectDepl.x=ratioSpeed;
        _vectDepl.y=ratioSpeed;
    }
}
