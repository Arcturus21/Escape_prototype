#ifndef TILEMAP_H
#define TILEMAP_H

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "TileProp.h"
#include "Checkpoint.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
    public:
        TileMap();
        virtual ~TileMap();
        bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int nbTilesLargeur, unsigned int nbTilesHauteur);
        bool load(const std::string& fichierMap);
        bool load(sf::Texture& tileset, int nbCouche, sf::Vector2f nbTile, sf::Vector2f dimTile, sf::Vector2f nbTileMap, std::vector<TileProp>& tileProp);
        sf::FloatRect getBoudingBox(int i, int j, int couche=0);
        bool tileDure(int i, int j);

        bool testCollision(sf::FloatRect &rect);

        bool enregistrerMap(const std::string cheminFichier, const std::string cheminTileset);

        sf::Vector2f getDimMap(){return sf::Vector2f(_nbTilesLargeurMonde*_dimTileInGame.x,_nbTilesHauteurMonde*_dimTileInGame.y);};

        bool modifierTile(sf::Vector2i& posSouris, int numTile, int coucheActuel, int placerTile=1);

        void setSolide(int indice, bool solide){_props[indice].setSolide(solide);};
        bool isSolide(int indice);

        void getProp(std::string& buf, sf::Texture& tileset, sf::Vector2f& nbTile, sf::Vector2f& dimTile, sf::Vector2f& nbTileMap, std::vector<TileProp>& tileProp);

        sf::Vector2f getPosDepart(){return tileToPosition(_posDepart);};
        sf::Vector2f getPosArrive(){return tileToPosition(_posArrive);};
        void setPosDepart(sf::Vector2f posDepart){_posDepart=posDepart;};
        void setPosArrive(sf::Vector2f posArrive){_posArrive=posArrive;};

        bool changeNbLine(int nbLine);
        void changeNbCol(int nbCol);

        int getNbCouche(){return _nbCouche;};
        int getCouchePerso(){return _couchePerso;};
        void setCouchePerso(int couche){_couchePerso=couche;};
        sf::Vector2f getDimTile(){return _dimTile;};

        void drawBackground(sf::RenderTarget& target) const;
        void drawForeground(sf::RenderTarget& target) const;
        void drawCouches(sf::RenderTarget& target, unsigned int couche) const;

        /* A FAIRE
        *   réécrire les fonctions précédentes en prenant en compte la couche (soit rajouter un paramètre, soit une boucle, etc)
        *   différencier la couche du personnage et la couche active
        *   ajouter fonctionnalité pour changer de couche
        *   Ecrire les fonctions ci-dessous ////DONE
        */

        int ajouterCouche();
        int enleverCouche();

        std::vector<std::vector<int> >* getSchemaPerso(){std::cout << "couche perso : " << _couchePerso-1 << std::endl; return &_schema[_couchePerso-1];};

        int getNbTilesLargeurMonde(){return _nbTilesLargeurMonde;};
        int getNbTilesHauteurMonde(){return _nbTilesHauteurMonde;};

        sf::Vector2f positionToTile(sf::Vector2f position) const;
        sf::Vector2f tileToPosition(sf::Vector2f tile) const;

        void reset();

    protected:
    private:
        bool loadTileset(std::ifstream& flux);
        bool loadMapprop(std::ifstream& flux);
        bool loadAutre(std::ifstream& flux);

        void affine(sf::Vector2f& position, const sf::Vector2f& dimension); ///trouver comment affiner et stocker une coordonnée à la fois

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            /// on applique la transformation
            states.transform *= getTransform();

            /// on applique la texture du tileset
            states.texture = &m_tileset;

            /// on dessine le tableau de vertex
            target.draw(m_vertices[0], states);
        }

        std::vector<sf::VertexArray > m_vertices;
        int _couchePerso, _nbCouche;
        sf::Texture m_tileset;

        float _ratioSize;

        std::vector<std::vector<std::vector<int> > > _schema;
        int _nbTilesLargeurMonde, _nbTilesHauteurMonde;
        sf::Vector2f _dimTile, _dimTileInGame;

        int _nbTiles;
        std::vector<TileProp> _props;

        sf::Vector2f _posDepart, _posArrive;
};

#endif // TILEMAP_H
