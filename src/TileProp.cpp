#include "../include/TileProp.h"

using namespace sf;

TileProp::TileProp(IntRect rect, bool solide)
{
    //ctor
    _rect = rect;
    _solide = solide;
}

TileProp::TileProp(Vector2f position, Vector2f dimension, bool solide)
{
    _rect = IntRect((int)position.x, (int)position.y, (int)dimension.x, (int)dimension.y);
    _solide=solide;
}

TileProp::~TileProp()
{
    //dtor
}
