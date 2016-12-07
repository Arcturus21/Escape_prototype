#include "CrashTurn.h"

using namespace std;
using namespace sf;

const float CrashTurn::SPEED = PersonnalizedSprite::WALK_SPEED-10;

CrashTurn::CrashTurn(sf::Time frameTime, bool paused, bool looped) :
    PersonnalizedSprite(frameTime, paused, looped), _etatIA(idle)
{
    //ctor
}

CrashTurn::~CrashTurn()
{
    //dtor
}

void CrashTurn::init(PersonnalizedSprite& cible, TileMap& tilemap)
{
    _cible = &cible;
    _map = &tilemap;

    _etatIA = cherche;

    _pointActuel = tilemap.positionToTile(getPosition());
    /**Utiliser les fonctions en développements une fois prêtes*/

    _noeudTraverses.clear();
    _cheminParcouru.clear();
    _noeudTraverses.push_back(_pointActuel);
    _cheminParcouru.push_back(_pointActuel);
    chercherNextTile();
}

void CrashTurn::update(sf::Time frameTime)
{
    Vector2f ciblePos = _map->positionToTile(_cible->getPosition());

    if(_etatIA==chasse)
    {
        ///ajouter fonction qui déplace vers le joueur + vérifie s'ils sont toujours dans le même tile?
        if(chasseJoueur(frameTime))
        {
            _cible->catched(this);
            _etatIA=attrape;
        }
    }
    else if(_etatIA==cherche)
        moveTo(_pointCible, frameTime);
    else if(_etatIA==attrape)
    {
        _vectDepl = Vector2f(0,0);
    }

    ///Partie animation
    if(_vectDepl.x < 0)   /** TEMPORAIRE */
    {
        if(_vectDepl.y < 0)
        {
            setEtat(marcheGaucheHaut);
        }
        else if(_vectDepl.y == 0)
        {
            setEtat(marcheGauche);
        }
        else
        {
            setEtat(marcheGaucheBas);
        }
    }
    else if(_vectDepl.x > 0)
    {
        if(_vectDepl.y < 0)
        {
            setEtat(marcheDroiteHaut);
        }
        else if(_vectDepl.y == 0)
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
        if(_vectDepl.y < 0)
        {
            setEtat(marcheHaut);
        }
        else if(_vectDepl.y > 0)
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


void CrashTurn::moveTo(sf::Vector2f indTile, sf::Time frameTime)
{
    Vector2f posJoueur = _map->positionToTile(getCenter());
    _vectDepl = indTile;
    _vectDepl.x -= _pointActuel.x;
    _vectDepl.y -= _pointActuel.y;
    ///_vectDepl*=WALK_SPEED/100;
    _vectDepl*=SPEED * frameTime.asSeconds();

    deplacer(*_map, _vectDepl);

    if(_map->getBoudingBox(_pointCible.x, _pointCible.y, _map->getCouchePerso()-1).intersects(getGlobalBounds())
       && !_map->getBoudingBox(_pointActuel.x, _pointActuel.y, _map->getCouchePerso()-1).intersects(getGlobalBounds()))
    {
        _pointActuel=_pointCible;
        if(_pointActuel == _map->positionToTile(_cible->getPosition()))
        {
            _etatIA = chasse;
        }
        else
        {
            chercherNextTile();
        }
    }
}

void CrashTurn::chercherNextTile()
{
    if(!choixMeilleurTile(trouverTilesAdjacent(_pointActuel.x, _pointActuel.y)))
    {
        ///etat=demiTour;
        cout << "pas de chemin" << endl;
        init(*_cible, *_map);
    }
    else
    {
        _cheminParcouru.push_back(_pointCible);
    }
}

std::vector<sf::Vector2f> CrashTurn::trouverTilesAdjacent(int indx, int indy)
{
    vector<Vector2f> listeAdjac;
    Vector2f temp, dimMap;
    dimMap.x = _map->getNbTilesLargeurMonde();
    dimMap.y = _map->getNbTilesHauteurMonde();

    if(indx-1 >= 0)
    {
        temp.x=indx-1; temp.y = indy;
        if(!_map->tileDure(temp.x, temp.y) && !dejaDansListe(_cheminParcouru, temp))
        {
            listeAdjac.push_back(temp);
        }
    }

    if(indy-1 >= 0)
    {
        temp.x=indx; temp.y=indy-1;
        if(!_map->tileDure(temp.x, temp.y) && !dejaDansListe(_cheminParcouru, temp))
        {
            listeAdjac.push_back(temp);
        }
    }

    if(indx-1 >= 0 && indy-1 >=0)
    {
        temp.x=indx-1;
        if(!_map->tileDure(temp.x+1, temp.y) && !_map->tileDure(temp.x, temp.y+1) && !_map->tileDure(temp.x, temp.y) && !dejaDansListe(_cheminParcouru, temp))
        {
            listeAdjac.push_back(temp);
        }
    }

    if(indx+1 < dimMap.x)
    {
        temp.x=indx+1; temp.y = indy;
        if(!_map->tileDure(temp.x, temp.y) && !dejaDansListe(_cheminParcouru, temp))
        {
            listeAdjac.push_back(temp);
        }
    }

    if(indy+1 < dimMap.y)
    {
        temp.x=indx; temp.y=indy+1;
        if(!_map->tileDure(temp.x, temp.y) && !dejaDansListe(_cheminParcouru, temp))
        {
            listeAdjac.push_back(temp);
        }
    }

    if(indx+1 < dimMap.x && indy+1 < dimMap.y)
    {
        temp.x=indx+1;
        if(!_map->tileDure(temp.x, temp.y) && !_map->tileDure(indx, temp.y) && !_map->tileDure(temp.x, indy)&& !dejaDansListe(_cheminParcouru, temp))
        {
            listeAdjac.push_back(temp);
        }
    }

    if(indx+1 < dimMap.x && indy-1 >= 0)
    {
        temp.x=indx+1; temp.y=indy-1;
        if(!_map->tileDure(temp.x, temp.y) && !_map->tileDure(indx, temp.y) && !_map->tileDure(temp.x, indy) && !dejaDansListe(_cheminParcouru, temp))
        {
            listeAdjac.push_back(temp);
        }
    }

    if(indy+1 < dimMap.y && indx-1 >= 0)
    {
        temp.x=indx-1; temp.y=indy+1;
        if(!_map->tileDure(temp.x, temp.y) && !_map->tileDure(indx, temp.y) && !_map->tileDure(temp.x, indy) && !dejaDansListe(_cheminParcouru, temp))
        {
            listeAdjac.push_back(temp);
        }
    }

    return listeAdjac;
}

bool CrashTurn::dejaDansListe(std::vector<sf::Vector2f> liste, sf::Vector2f coordTile)
{
    int i=0;
    bool found=false;
    while(i<liste.size() && !found)
    {
        found = (liste[i] == coordTile);
        i++;
    }
    return found;
}

bool CrashTurn::choixMeilleurTile(std::vector<sf::Vector2f> listeTile)
{
    if(listeTile.size()==0)
        return false;
    else
        _noeudTraverses.push_back(_pointActuel);

    int meilleurInd=0;
    float meilleurDistance, distance;

    Vector2f posCible = _map->tileToPosition(listeTile[0]);
    meilleurDistance = calculDistance(posCible.x, posCible.y, _cible->getPosition().x, _cible->getPosition().y);

    for(int i=1; i<listeTile.size(); ++i)
    {
        posCible = _map->tileToPosition(listeTile[i]);
        distance = calculDistance(posCible.x, posCible.y, _cible->getPosition().x, _cible->getPosition().y);
        if(distance<meilleurDistance)
        {
            meilleurDistance=distance;
            meilleurInd=i;
        }
    }

    _pointCible = listeTile[meilleurInd];

    return true;
}

bool CrashTurn::chasseJoueur(sf::Time frameTime)
{
    Vector2f pointCible = _map->positionToTile(_cible->getPosition());
    _pointActuel = _map->positionToTile(getPosition());
    if(_cible->getPosition().x > getPosition().x+getLocalBounds().width)
    {
        _vectDepl.x = 1;
    }
    else if(_cible->getPosition().x+_cible->getLocalBounds().width < getPosition().x)
    {
        _vectDepl.x = -1;
    }
    else
    {
        _vectDepl.x = 0;
    }
    if(_cible->getPosition().y > getPosition().y+getLocalBounds().height)
    {
        _vectDepl.y = 1;
    }
    else if(_cible->getPosition().y+_cible->getLocalBounds().height < getPosition().y)
    {
        _vectDepl.y = -1;
    }
    else
    {
        _vectDepl.y = 0;
    }

    setVectDepl(_vectDepl*SPEED*frameTime.asSeconds());
    deplacer(*_map, _vectDepl);
    return getGlobalBounds().intersects(_cible->getGlobalBounds());
}

void PersonnalizedSprite::catched(PersonnalizedSprite* sprite)
{
    cout << "catched" <<endl;
}
