#ifndef SALLETILEMAP_H
#define SALLETILEMAP_H

#include "TileMap.h"
#include "Salle.h"

class SalleTileMap : public TileMap
{
    public:
        SalleTileMap();
        virtual ~SalleTileMap();

        bool load(const std::string& fichierMap);

        int getNbSalle(){return _listSalle.size();};
        Salle getSalle(int ind){return *(_listSalle[ind]);};
        sf::View getViewSalle(int ind);
        sf::FloatRect getBoundSalle(int ind);

        void reset();

    protected:

    private:
        bool loadSalle(std::ifstream &flux);

        std::vector<Salle*> _listSalle;
};

#endif // SALLETILEMAP_H
