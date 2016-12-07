#ifndef CRASHTURN_H
#define CRASHTURN_H

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "PersonnalizedSprite.h"
#include "CharacterSprite.h"
#include "AStar.h"

/** Algorithme crash&turn :
    - liste contenant le chemin parcouru (pour pouvoir le remonter et ne pas faire un choix deux fois)
    - liste contenant les noeuds ou il y a eu un choix (et le nombre de choix effectu� ?)

    + chercher les points atteignables (attention aux diagonales -> tester si les tiles d'� c�t� bloquent le passage)
            et non visit�s
    + si + d'1 points atteignable, tirer un points au hasard (en v�rifiant la distance par rapport au joueur)
        -> ajouter le noeuds � la liste
    + si le chemin ne m�ne nul part, remonter jusqu'au noeud libre pr�c�dent
    + s'il ne reste plus de noeud dans la liste, on efface tout et recommence au d�but

    -> attention au passage large (plus d'un tile) (osef?)
*/

/** REFLEXION POUR LE CRASH & TURN (aussi valable pour le A*)
    1. Consid�rer la map comme des tiles simples, les d�placements sont instantan�s de tiles en tiles
        - collision simple
        - pas de d�placement en diagonal
    2. Une fois le Tile de destination d�termin�, effectu� un d�placement vers son centre ou en utilisant le vecteur le plus court possible (mieux)
        - tester les collisions � ce moment
        - arr�ter d�s que le personnage entre dans le tile
        - arr�ter TOUT si vx = vy = 0
    3. D�s la collision avec le tile d'arriv�e, essayer d'atteindre le tile suivant (la fonction affine se chargera du glissement)

    R�sum� :
    1. d�part (1.0) arriv�e (1.1) vecteur (0.1)
    2. moveTo (1.1) -> pos+=vecteur
    3. �v�nement collision -> retour � 1.
    4. Tant que pos != arriv�e || (vx!=0 && vy!=0)
*/

typedef enum EtatCrashTurn
{
    idle,
    cherche,
    chasse,
    attrape
}EtatCrashTurn;

class CrashTurn : public PersonnalizedSprite
{
    public:
        CrashTurn(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true);
        virtual ~CrashTurn();

        void init(PersonnalizedSprite& cible, TileMap& tilemap);

        void update(sf::Time frameTime);

        void moveTo(sf::Vector2f indTile, sf::Time frameTime);
        void chercherNextTile();
        std::vector<sf::Vector2f> trouverTilesAdjacent(int indx, int indy);

        bool dejaDansListe(std::vector<sf::Vector2f> liste, sf::Vector2f coordTile);
        bool choixMeilleurTile(std::vector<sf::Vector2f> listeTile);

        bool chasseJoueur(sf::Time frameTime);

        static const float SPEED;

    protected:
        float calculDistance(float x1, float y1, float x2, float y2){return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);};
        sf::FloatRect getTileRect(int indx, int indy){return _map->getBoudingBox(indx, indy, _map->getCouchePerso()-1);};

    private:

        PersonnalizedSprite* _cible;
        TileMap *_map;

        std::vector<sf::Vector2f> _noeudTraverses;  ///Stock des INDICES DE TILES
        std::vector<sf::Vector2f> _cheminParcouru;

        sf::Vector2f _pointActuel, _pointCible;

        EtatCrashTurn _etatIA;
};

#endif // CRASHTURN_H
