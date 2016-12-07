#include "Checkpoint.h"

using namespace sf;
using namespace std;

Checkpoint::Checkpoint(typeCheckpoint type, int ind, sf::Vector2f pos, sf::Vector2f dim)
{
    //ctor
    _rect = FloatRect(pos.x, pos.y, dim.x, dim.y);
    _type=type;
    _indice=ind;
}

Checkpoint::~Checkpoint()
{
    //dtor
}

void Checkpoint::update(sf::Time frameTime)
{
    //cout << "lol" << endl;
}
