#ifndef ASTAR_H
#define ASTAR_H

#include <iostream>
#include <map>
#include <list>

#include <SFML/Graphics.hpp>

#include "TileMap.h"
#include "TileProp.h"

typedef struct point{

    int x,y;

}point;

typedef struct noeud{

    float coutG, coutH, coutF;
    std::pair<int,int> parent;

}noeud;

typedef std::map< std::pair<int,int>, noeud> Noeud;

///a modifier en utilisant des distances et pas des tiles

class AStar
{
    public:
        AStar();
        virtual ~AStar();
        void init(sf::Vector2f depart, sf::Vector2f arrivee, TileMap& tilemap);

        bool dejaPresentDansListe(std::pair<int,int> n, Noeud& noeud);
        void ajouterCasesAdjacentes(std::pair <int,int>& n);
        std::pair<int,int> meilleurNoeud(Noeud& noeud);
        void ajouterListeFermee(std::pair<int,int>& p);
        void retrouverChemin();
        void traitementPrincipal();

        point nextPoint();

    protected:
    private:
        float distance(int x1, int y1, int x2, int y2){return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);};

        Noeud _listeOuverte, _listeFermee;
        noeud _depart;
        point _arrivee;
        std::vector<point> _chemin;

        TileMap* _tilemap;
        std::vector<std::vector<int> >* _schema;
};

/**
    - déplacement en fonction de la vitesse du personnage
    - tester tous le mouvement pour les collisions (cf cours de fred sur openclassroom)
    - faire "glisser" le personnage jusqu'au prochain noeud

*/

#endif // ASTAR_H
