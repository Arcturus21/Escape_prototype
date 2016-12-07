#include "SalleTileMap.h"

using namespace std;
using namespace sf;

SalleTileMap::SalleTileMap() : TileMap()
{
    //ctor
}

SalleTileMap::~SalleTileMap()
{
    //dtor
    for(unsigned int i=0; i<_listSalle.size(); i++)
        delete _listSalle[i];
    _listSalle.clear();
}

void SalleTileMap::reset()
{
     for(unsigned int i=0; i<_listSalle.size(); i++)
        delete _listSalle[i];
    _listSalle.clear();
    TileMap::reset();
}

bool SalleTileMap::load(const std::string& fichierMap)
{
    ///Ajouter gestion exception après
    ifstream flux(fichierMap.c_str());
    if(!flux)
        return false;

    string bufTxt, buf;

    flux >> bufTxt;
    if(bufTxt != "<Map>")
        return false;
    bufTxt="ok";

    while(bufTxt != "<Map>")
    {
        flux >> bufTxt;
        if(bufTxt=="<Tileset>")
        {
            if(!loadTileset(flux))
                return false;
            cout << "Tileset" << endl;
        }
        else if(bufTxt=="<MapProp>")
        {
            if(!loadMapprop(flux))
                return false;
            cout << "Mapprop" << endl;
        }
        else if(bufTxt=="<Salle>")
        {
            if(!loadSalle(flux))
                return false;
            cout << "Salle" << endl;
        }
        else if(bufTxt=="<Autre>")
        {
            if(!loadAutre(flux))
                return false;
            cout << "Autre" << endl;
        }
        else
        {
            cout << "Cas non traité" << endl;
        }

    }

    return (bufTxt=="<Map>");
}

bool SalleTileMap::loadSalle(std::ifstream &flux)
{
    string buf;
    int nb1, nb2, nb3, nb4;
    flux >> buf;
    while(buf != "<Salle>")
    {
        flux >> nb1 >> nb2 >> nb3 >> nb4;
        Salle *s = new Salle(nb1, nb2, nb3, nb4);
        _listSalle.push_back(s);
        flux >> buf;
    }
    return buf == "<Salle>";
}

View SalleTileMap::getViewSalle(int ind)
{
    cout << "indice: " << ind << endl << "size: " << _listSalle.size() << endl;
    View view = _listSalle[ind]->getViewSalle();
    cout << "view : " << view.getCenter().x << " " << view.getCenter().y << " " << view.getSize().x << " " << view.getSize().y << endl;
    return view;
}

FloatRect SalleTileMap::getBoundSalle(int ind)
{
    FloatRect rect = _listSalle[ind]->getBoundSalle();
    return rect;
}
