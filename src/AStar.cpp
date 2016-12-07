#include "../include/AStar.h"

using namespace std;
using namespace sf;

AStar::AStar()
{
    //ctor
}

AStar::~AStar()
{
    //dtor
}

void AStar::init(sf::Vector2f depart, sf::Vector2f arrivee, TileMap& tilemap)
{
    _arrivee.x = arrivee.x;
    _arrivee.y = arrivee.y;

    _depart.parent.first  = depart.x;
    _depart.parent.second = depart.y;

    _tilemap = &tilemap;
    _schema = _tilemap->getSchemaPerso();
    _listeFermee.clear();
    _listeOuverte.clear();
    _chemin.clear();
}

bool AStar::dejaPresentDansListe(pair<int,int> n, Noeud& noeud)
{
    Noeud::iterator i = noeud.find(n);
    if(i==noeud.end())
        return false;
    else
        return true;
}

void AStar::ajouterCasesAdjacentes(pair <int,int>& n)
{
    noeud tmp;
    //int compteur=0;
    Vector2f dimTile = _tilemap->getDimTile();

    /// on met tous les noeuds adjacents dans la liste ouverte (+verif)
    for(int i=n.first-1; i<=n.first+1; i++)
    {
        if((i<0) || i>=_tilemap->getNbTilesLargeurMonde()) /// en dehors de la map, on oublie
            continue;
        for(int j=n.second-1; j<=n.second+1; j++)
        {
            if((j<0) || j>=_tilemap->getNbTilesHauteurMonde()) /// en dehors de la map, on oublie
                continue;
            if((i==n.first) && (j==n.second)) /// case actuelle : on oublie
                continue;

            //cout << i << " " << j << " " << (*_schema)[i][j] << endl;
            if(_tilemap->isSolide((*_schema)[i][j])) /// tile solide, on oublie
            {
                //cout << "solide" << endl;
                continue;
            }
            else{
                //cout << "pas solide" << endl;
            }

            pair<int,int> it(i,j);
            if(!dejaPresentDansListe(it, _listeFermee))
            {
                /// Le noeud n'est pas présent dans la liste fermée
                /// calcul du cout G du noeud en cours d'étude : cout du parent + distance jusqu'au parent
                tmp.coutG = _listeFermee[n].coutG + distance(i*dimTile.x, j*dimTile.y, n.first*dimTile.x, n.second*dimTile.y);

                /// calcul du cout H du noeud à la destination
                tmp.coutH = distance(i*dimTile.x,j*dimTile.y,_arrivee.x*dimTile.x,_arrivee.y*dimTile.y);
                tmp.coutF = tmp.coutG + tmp.coutH;
                tmp.parent = n;

                if(dejaPresentDansListe(it, _listeOuverte))
                {
                    /// le noeud est déjà présent dans la liste ouverte, il faut comparer les couts
                    if(tmp.coutF<_listeOuverte[it].coutF)
                    {
                        /// si le nouveau chemin est meilleur, on met à jour
                        _listeOuverte[it]=tmp;
                    }

                    /// le noeud courant a un moins bon chemin, on ne change rien
                }
                else
                {
                    /// le noeud n'est pas présent dans la liste ouverte, on l'y ajoute
                    _listeOuverte[pair<int,int>(i,j)]=tmp;
                }
            }
        }
    }
}

std::pair<int,int> AStar::meilleurNoeud(Noeud& noeud)
{
    float coutF = noeud.begin()->second.coutF;
    pair<int,int> mNoeud = noeud.begin()->first;

    for(Noeud::iterator i = noeud.begin(); i!=noeud.end(); i++)
    {
        if(i->second.coutF<coutF)
        {
            coutF = i->second.coutF;
            mNoeud = i->first;
        }
    }
    return mNoeud;
}

void AStar::ajouterListeFermee(std::pair<int,int>& p)
{
    noeud& n = _listeOuverte[p];
    _listeFermee[p]=n;

    ///Supprimer de la liste ouverte (solution pas explorable)
    if(_listeOuverte.erase(p)==0)
        cerr << "Erreur, le noeud n'est pas dans la liste ouverte" << endl;
    return;
}

void AStar::retrouverChemin()
{
    /// l'arrivée est le dernier élément de la liste fermée
    noeud& tmp = _listeFermee[pair<int,int>(_arrivee.x,_arrivee.y)];

    point n;
    pair<int,int> prec;
    n.x = _arrivee.x;
    n.y = _arrivee.y;
    prec.first = tmp.parent.first;
    prec.second = tmp.parent.second;
    _chemin.push_back(n);

    while(prec != pair<int,int>(_depart.parent.first,_depart.parent.second))
    {
        n.x = prec.first;
        n.y = prec.second;
        _chemin.push_back(n);

        tmp = _listeFermee[tmp.parent];
        prec.first = tmp.parent.first;
        prec.second = tmp.parent.second;
    }

}

void AStar::traitementPrincipal()
{
    pair<int,int> courant;

    ///initialisation du noeud courant
    courant.first = _depart.parent.first;
    courant.second = _depart.parent.second;

    ///ajout de courant dans la liste ouverte
    _listeOuverte[courant]=_depart;
    ajouterListeFermee(courant);
    ajouterCasesAdjacentes(courant);

    ///tant que la destination n'est pas atteinte et qu'il reste des noeuds à explorer  dans la liste ouverte
    while(!((courant.first==_arrivee.x) && (courant.second == _arrivee.y)) && (!_listeOuverte.empty()))
    {
        ///on cherche le meilleur noeud de la liste ouverte, on sait qu'elle n'est pas vide donc il existe
        courant = meilleurNoeud(_listeOuverte);
        ///on le passe dans la liste fermee, il ne peut pas déjà y être
        ajouterListeFermee(courant);
        ///On recommence la recherche des noeuds adjacents
        ajouterCasesAdjacentes(courant);
    }

    ///si la destination est atteinte, on remonte le chemin
    if((courant.first == _arrivee.x) && (courant.second == _arrivee.y))
    {
        retrouverChemin();
    }
    cout << _depart.parent.first << " " << _depart.parent.second << endl;
    cout << _arrivee.x << " " << _arrivee.y << endl;
    cout << _listeOuverte.size() << endl;
    cout << _chemin.size() << endl;
}

point AStar::nextPoint()
{
    point retour;
    if(_chemin.size()>0)
    {
        retour = _chemin[_chemin.size()-1];
        _chemin.pop_back();
    }
    else
    {
        retour.x=-1;
        retour.y=-1;
    }
    return retour;
}
