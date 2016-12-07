#include "../include/TileMap.h"

using namespace std;
using namespace sf;

TileMap::TileMap()
{
    //ctor
    _ratioSize=4;
    _posDepart.x=_posDepart.y=0;
    _couchePerso=_nbCouche=1;
    m_vertices.resize(1);
}

TileMap::~TileMap()
{
    //dtor
}

bool TileMap::load(Texture& tileset, int nbCouche, Vector2f nbTile, Vector2f dimTile, Vector2f nbTileMap, vector<TileProp>& tileProp)
{
    m_tileset=tileset;

    _nbTilesLargeurMonde=nbTileMap.x;
    _nbTilesHauteurMonde=nbTileMap.y;
    _dimTile=dimTile;
    _nbTiles = nbTile.x*nbTile.y;
    _props=tileProp;


    m_vertices[0].setPrimitiveType(Quads);
    m_vertices[0].resize(_nbTilesLargeurMonde*_nbTilesHauteurMonde*4);

    if(nbCouche < 1)
        return false;
    _nbCouche=nbCouche;

    _schema.resize(nbCouche);
    for(unsigned int k=0; k<_schema.size(); ++k)
    {
        _schema[k].resize(_nbTilesLargeurMonde);
        for(int i=0; i<_nbTilesLargeurMonde; ++i)
        {
            for(int j=0; j<_nbTilesHauteurMonde; j++)
            {
                int tileNumber=0;

                if(k!=0)
                {
                    tileNumber=-1;
                }

                _schema[k][i].push_back(tileNumber);
                int tu= tileNumber % (m_tileset.getSize().x/(int)_dimTile.x);
                int tv= tileNumber / (m_tileset.getSize().x/(int)_dimTile.x);

                Vertex* quad = &m_vertices[0][(i+j*_nbTilesLargeurMonde)*4];
                /// on définit ses quatres coins
                quad[0].position = Vector2f(i*(int)_dimTile.x*_ratioSize, j*(int)_dimTile.y*_ratioSize);
                quad[1].position = Vector2f((i+1)*(int)_dimTile.x*_ratioSize, j*(int)_dimTile.y*_ratioSize);
                quad[2].position = Vector2f((i+1)*(int)_dimTile.x*_ratioSize, (j+1)*(int)_dimTile.y*_ratioSize);
                quad[3].position = Vector2f(i*(int)_dimTile.x*_ratioSize, (j+1)*(int)_dimTile.y*_ratioSize);


                /// on définit ses quatres coordonnés de texture
                if(k==0)
                {
                    quad[0].texCoords = Vector2f(tu*(int)_dimTile.x, tv*(int)_dimTile.y);
                    quad[1].texCoords = Vector2f((tu+1)*(int)_dimTile.x, tv*(int)_dimTile.y);
                    quad[2].texCoords = Vector2f((tu+1)*(int)_dimTile.x, (tv+1)*(int)_dimTile.y);
                    quad[3].texCoords = Vector2f(tu*(int)_dimTile.x, (tv+1)*(int)_dimTile.y);
                }
                else
                {
                    quad[0].texCoords = Vector2f(0,0);
                    quad[1].texCoords = Vector2f(0,0);
                    quad[2].texCoords = Vector2f(0,0);
                    quad[3].texCoords = Vector2f(0,0);
                }

            }
        }
    }

    _posArrive.x=2*_dimTile.x*_ratioSize;
    _posArrive.y=2*_dimTile.y*_ratioSize;

    return true;
}

bool TileMap::loadTileset(std::ifstream& flux)
{
    string buf, buf2;
    int nb1=0, nb2=0;
    Vector2f nbTiles;

    flux >> buf >> buf2;
    m_tileset.loadFromFile(buf2);

    flux >> buf >> nbTiles.x >> nbTiles.y;
    _nbTiles = nbTiles.x*nbTiles.y;
    flux >> buf >> _dimTile.x >> _dimTile.y;

    for(int i=0; i<_nbTiles; ++i)
    {
        flux >> buf >> nb1;
        IntRect rect((i%(int)nbTiles.x)*_dimTile.x, (i/nbTiles.x)*_dimTile.y, _dimTile.x, _dimTile.y);
        _props.push_back(TileProp(rect, (nb1!=0)));
    }

    flux >> buf;
    return (buf=="<Tileset>");
}

bool TileMap::loadMapprop(std::ifstream& flux)
{
    string buf;
    int nb1, nb2;

    flux >> buf >> _dimTileInGame.x >> _dimTileInGame.y;
    flux >> buf >> _nbTilesLargeurMonde >> _nbTilesHauteurMonde;
    flux >> buf >> _nbCouche;
    flux >> buf >> _couchePerso;
    flux >> buf;

    m_vertices.resize(_nbCouche);
    _schema.resize(_nbCouche);
    for(int k=0; k<_nbCouche; ++k)
    {
        m_vertices[k].setPrimitiveType(Quads);
        m_vertices[k].resize(_nbTilesLargeurMonde*_nbTilesHauteurMonde*4);

        _schema[k].resize(_nbTilesLargeurMonde);
        for(int i=0; i<_nbTilesLargeurMonde; ++i)
        {
            _schema[k][i].resize(_nbTilesHauteurMonde);
            for(int j=0; j<_nbTilesHauteurMonde; ++j)
            {
                int tileNumber;
                flux >> tileNumber;
                _schema[k][i][j]=tileNumber;

                int tu= tileNumber % (m_tileset.getSize().x/(int)_dimTile.x);
                int tv= tileNumber / (m_tileset.getSize().x/(int)_dimTile.x);

                Vertex* quad = &m_vertices[k][(i+j*_nbTilesLargeurMonde)*4];

                if(tileNumber!=-1)
                {
                    /// on définit ses quatres coins
                    quad[0].position = Vector2f(i*(int)_dimTileInGame.x, j*(int)_dimTileInGame.y);
                    quad[1].position = Vector2f((i+1)*(int)_dimTileInGame.x, j*(int)_dimTileInGame.y);
                    quad[2].position = Vector2f((i+1)*(int)_dimTileInGame.x, (j+1)*(int)_dimTileInGame.y);
                    quad[3].position = Vector2f(i*(int)_dimTileInGame.x, (j+1)*(int)_dimTileInGame.y);

                    /// on définit ses quatres coordonnés de texture
                    quad[0].texCoords = Vector2f(tu*(int)_dimTile.x, tv*(int)_dimTile.y);
                    quad[1].texCoords = Vector2f((tu+1)*(int)_dimTile.x, tv*(int)_dimTile.y);
                    quad[2].texCoords = Vector2f((tu+1)*(int)_dimTile.x, (tv+1)*(int)_dimTile.y);
                    quad[3].texCoords = Vector2f(tu*(int)_dimTile.x, (tv+1)*(int)_dimTile.y);
                }
                else
                {
                    /// on définit ses quatres coins
                    quad[0].position = Vector2f(0,0);
                    quad[1].position = Vector2f(0,0);
                    quad[2].position = Vector2f(0,0);
                    quad[3].position = Vector2f(0,0);

                    quad[0].texCoords = Vector2f(0,0);
                    quad[1].texCoords = Vector2f(0,0);
                    quad[2].texCoords = Vector2f(0,0);
                    quad[3].texCoords = Vector2f(0,0);
                }

            }
        }
    }

    flux >> buf;
    return (buf == "<MapProp>");
}

bool TileMap::loadAutre(std::ifstream& flux)
{
    string buf;

    flux >> buf >> _posDepart.x >> _posDepart.y;
    flux >> buf >> _posArrive.x >> _posArrive.y;
    while(buf!="<Autre>")
    {
        flux >> buf;
    }
    return (buf=="<Autre>");
}

bool TileMap::load(const std::string& fichierMap)
{
    ///Ajouter gestion exception après
    ifstream flux(fichierMap.c_str());
    if(!flux)
        return false;

    string bufTxt, buf;
    int nb1, nb2;

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

void TileMap::reset()
{
    _props.clear();
    _schema.clear();
    m_vertices.clear();
}

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int nbTilesLargeur, unsigned int nbTilesHauteur)
{
    return true;
}

FloatRect TileMap::getBoudingBox(int i, int j, int couche)
{
    VertexArray quad(Quads, 4);
    quad[0]=m_vertices[couche][(i+j*_nbTilesLargeurMonde)*4];
    quad[1]=m_vertices[couche][(i+j*_nbTilesLargeurMonde)*4+1];
    quad[2]=m_vertices[couche][(i+j*_nbTilesLargeurMonde)*4+2];
    quad[3]=m_vertices[couche][(i+j*_nbTilesLargeurMonde)*4+3];
    return quad.getBounds();
}

bool TileMap::tileDure(int i, int j)
{
    return isSolide(_schema[_couchePerso-1][i][j]);
}

bool TileMap::isSolide(int indice)
{
    bool retour=false;
    if(indice!=-1)
        retour=_props[indice].isSolide();
    return retour;
}

bool TileMap::testCollision(FloatRect& rectPerso)
{
    bool collide=false;
    int i=0, j=0;

    if(rectPerso.left < 0 || rectPerso.left + rectPerso.width >= _nbTilesLargeurMonde*_dimTile.x*_ratioSize
       || rectPerso.top < 0 || rectPerso.top + rectPerso.height >= _nbTilesHauteurMonde*_dimTile.y*_ratioSize)
        return true;

    int xmin = rectPerso.left/(_dimTile.x*_ratioSize), xmax = (rectPerso.left + rectPerso.width)/(_dimTile.x*_ratioSize);
    int ymin = rectPerso.top/(_dimTile.y*_ratioSize), ymax = (rectPerso.top + rectPerso.height)/(_dimTile.y*_ratioSize);

    i=xmin;
    while(i <= xmax && !collide)
    {
        j=ymin;
        while(j<=ymax && !collide)
        {
            //cout << _couchePerso << endl;
            int numTile = _schema[_couchePerso-1][i][j];
            if(numTile != -1)
                collide = _props[numTile].isSolide();
            j++;
        }
        i++;
    }

    return collide;
}

void TileMap::affine(sf::Vector2f& position, const sf::Vector2f& dimension)
{

}

bool TileMap::enregistrerMap(const string cheminFichier, const string cheminTileset)
{
    if(cheminTileset.empty() || cheminFichier.empty())
        return false;

    ofstream fichier(cheminFichier.c_str());
    if(!fichier)
        return false;

    fichier << cheminTileset << endl;

    int nb1 = m_tileset.getSize().x/_dimTile.x, nb2 = m_tileset.getSize().y/_dimTile.y;

    fichier << nb1 << " " << nb2 << endl;

    for(int i=0; i<_nbTiles; ++i)
    {
        if(_props[i].isSolide())
            fichier << "1" << endl;
        else
            fichier << "0" << endl;
    }

    fichier << _nbTilesLargeurMonde << " " << _nbTilesHauteurMonde << endl;

    fichier << _nbCouche << endl;
    for(int k=0; k<_nbCouche; ++k)
    {
        for(int i=0; i<_nbTilesLargeurMonde; ++i)
        {
            for(int j=0; j<_nbTilesHauteurMonde; ++j)
            {
                fichier << _schema[k][i][j] << " ";
            }
            fichier << endl;
        }
        fichier << endl;
    }

    fichier << _couchePerso << endl;
    fichier << _posDepart.x << " " << _posDepart.y << endl;
    fichier << _posArrive.x << " " << _posArrive.y << endl;

    return true;
}

bool TileMap::modifierTile(sf::Vector2i& posSouris, int numTile, int coucheActuel, int placerTile)
{
    if(numTile < 0 || numTile > _nbTiles)
        return false;

    Vector2i indiceSouris;
    indiceSouris.x = posSouris.x/(_dimTile.x*_ratioSize);
    indiceSouris.y = posSouris.y/(_dimTile.y*_ratioSize);

    if(indiceSouris.x < 0 || indiceSouris.x >= _nbTilesLargeurMonde || indiceSouris.y < 0 || indiceSouris.y >= _nbTilesHauteurMonde)
        return false;

    if(coucheActuel<0 || coucheActuel>=_nbCouche)
        return false;

    if(placerTile==1)
        _schema[coucheActuel][indiceSouris.x][indiceSouris.y] = numTile;
    else
        _schema[coucheActuel][indiceSouris.x][indiceSouris.y] = -1;

    /// on en déduit sa position dans la texture du tileset
    int tu=numTile % (m_tileset.getSize().x/(int)_dimTile.x);
    int tv=numTile / (m_tileset.getSize().x/(int)_dimTile.x);

    /// on récupère un pointeur vers le quad à définir dans le tableau de vertex
    Vertex* quad = &m_vertices[coucheActuel][(indiceSouris.x+indiceSouris.y*_nbTilesLargeurMonde)*_ratioSize];  ///*4 car 4 vertex par tiles

    quad[0].position = Vector2f(indiceSouris.x*(int)_dimTile.x*4*placerTile, indiceSouris.y*(int)_dimTile.y*4*placerTile);
    quad[1].position = Vector2f((indiceSouris.x+1)*(int)_dimTile.x*4*placerTile, indiceSouris.y*(int)_dimTile.y*4*placerTile);
    quad[2].position = Vector2f((indiceSouris.x+1)*(int)_dimTile.x*4*placerTile, (indiceSouris.y+1)*(int)_dimTile.y*4*placerTile);
    quad[3].position = Vector2f(indiceSouris.x*(int)_dimTile.x*4*placerTile, (indiceSouris.y+1)*(int)_dimTile.y*4*placerTile);

    /// on définit ses quatres coordonnés de texture
    quad[0].texCoords = Vector2f(tu*(int)_dimTile.x*placerTile, tv*(int)_dimTile.y*placerTile);
    quad[1].texCoords = Vector2f((tu+1)*(int)_dimTile.x*placerTile, tv*(int)_dimTile.y*placerTile);
    quad[2].texCoords = Vector2f((tu+1)*(int)_dimTile.x*placerTile, (tv+1)*(int)_dimTile.y*placerTile);
    quad[3].texCoords = Vector2f(tu*(int)_dimTile.x*placerTile, (tv+1)*(int)_dimTile.y*placerTile);


    return true;
}

void TileMap::getProp(string& buf, Texture& tileset, Vector2f& nbTile, Vector2f& dimTile, Vector2f& nbTileMap, vector<TileProp>& tileProp)
{
    ifstream flux(buf.c_str()); ///à changer
    flux >> buf;

    tileset = m_tileset;

    nbTile.x = m_tileset.getSize().x/_dimTile.x;
    nbTile.y = m_tileset.getSize().y/_dimTile.y;
    dimTile.x = _dimTile.x;
    dimTile.y = _dimTile.y;
    nbTileMap.x = _nbTilesLargeurMonde;
    nbTileMap.y = _nbTilesHauteurMonde;
    tileProp = _props;
}

bool TileMap::changeNbLine(int nbLine)
{
    int tempNbTileHaut = _nbTilesHauteurMonde;


    _nbTilesHauteurMonde+=nbLine;
    if(_nbTilesHauteurMonde<1)
        _nbTilesHauteurMonde=1;

    for(int k=0; k<_nbCouche; ++k)
    {
        for(int i=0; i<_nbTilesLargeurMonde; ++i)
        {
            _schema[k][i].resize(_nbTilesHauteurMonde);
        }
    }



    for(unsigned int k=0; k<_nbCouche; ++k)
    {
        VertexArray verticeTemp(m_vertices[k]);
        m_vertices[k].resize(_nbTilesLargeurMonde*_nbTilesHauteurMonde*_ratioSize);
        for(int i=0; i<_nbTilesLargeurMonde; ++i)
        {
            for(int j=0; j<_nbTilesHauteurMonde; ++j)
            {
                if(j < tempNbTileHaut)
                    m_vertices[k][(i+j*_nbTilesLargeurMonde)*4] = verticeTemp[(i+j*_nbTilesLargeurMonde)*4];
                else
                {
                    Vertex* quad = &m_vertices[k][(i+j*_nbTilesLargeurMonde)*4];
                    _schema[k][i][j]=-1;

                    if(k==0)
                    {
                        /// on définit ses quatres coins
                        quad[0].position = Vector2f(i*(int)_dimTile.x*4, j*(int)_dimTile.y*4);
                        quad[1].position = Vector2f((i+1)*(int)_dimTile.x*4, j*(int)_dimTile.y*4);
                        quad[2].position = Vector2f((i+1)*(int)_dimTile.x*4, (j+1)*(int)_dimTile.y*4);
                        quad[3].position = Vector2f(i*(int)_dimTile.x*4, (j+1)*(int)_dimTile.y*4);

                        /// on définit ses quatres coordonnés de texture
                        quad[0].texCoords = Vector2f(0, 0);
                        quad[1].texCoords = Vector2f((int)_dimTile.x, 0);
                        quad[2].texCoords = Vector2f((int)_dimTile.x, (int)_dimTile.y);
                        quad[3].texCoords = Vector2f(0, (int)_dimTile.y);
                    }
                    else
                    {
                        quad[0].position = Vector2f(0,0);
                        quad[1].position = Vector2f(0,0);
                        quad[2].position = Vector2f(0,0);
                        quad[3].position = Vector2f(0,0);

                        quad[0].texCoords = Vector2f(0, 0);
                        quad[1].texCoords = Vector2f(0, 0);
                        quad[2].texCoords = Vector2f(0, 0);
                        quad[3].texCoords = Vector2f(0, 0);
                    }
                }
            }
        }
    }

    return true;
}

void TileMap::changeNbCol(int nbCol)
{
    int tempNbTileLarg = _nbTilesLargeurMonde;

    _nbTilesLargeurMonde+=nbCol;
    if(_nbTilesLargeurMonde<1)
        _nbTilesLargeurMonde=1;

    for(int k=0; k<_nbCouche; ++k)
    {
        _schema[k].resize(_nbTilesLargeurMonde);
        for(int i=tempNbTileLarg-1; i<_nbTilesLargeurMonde; ++i)
        {
            _schema[k][i].resize(_nbTilesHauteurMonde);
        }
    }

    for(unsigned int k=0; k<_nbCouche; ++k)
    {

        VertexArray verticeTemp(m_vertices[k]);
        m_vertices[k].resize(_nbTilesLargeurMonde*_nbTilesHauteurMonde*4);
        for(int i=0; i<_nbTilesLargeurMonde; ++i)
        {
            for(int j=0; j<_nbTilesHauteurMonde; j++)
            {
                if(i<tempNbTileLarg)
                {
                    Vertex* quad1 = &m_vertices[k][(i+j*_nbTilesLargeurMonde)*4];
                    Vertex* quad2 = &verticeTemp[(i+j*tempNbTileLarg)*4];

                    quad1[0].position = quad2[0].position;
                    quad1[1].position = quad2[1].position;
                    quad1[2].position = quad2[2].position;
                    quad1[3].position = quad2[3].position;

                    quad1[0].texCoords = quad2[0].texCoords;
                    quad1[1].texCoords = quad2[1].texCoords;
                    quad1[2].texCoords = quad2[2].texCoords;
                    quad1[3].texCoords = quad2[3].texCoords;
                }
                else
                {
                    Vertex* quad = &m_vertices[k][(i+j*_nbTilesLargeurMonde)*4];
                    _schema[k][i][j]=-1;

                    if(k==0)
                    {
                        /// on définit ses quatres coins
                        quad[0].position = Vector2f(i*(int)_dimTile.x*4, j*(int)_dimTile.y*4);
                        quad[1].position = Vector2f((i+1)*(int)_dimTile.x*4, j*(int)_dimTile.y*4);
                        quad[2].position = Vector2f((i+1)*(int)_dimTile.x*4, (j+1)*(int)_dimTile.y*4);
                        quad[3].position = Vector2f(i*(int)_dimTile.x*4, (j+1)*(int)_dimTile.y*4);

                        /// on définit ses quatres coordonnés de texture
                        quad[0].texCoords = Vector2f(0,0);
                        quad[1].texCoords = Vector2f((int)_dimTile.x, 0);
                        quad[2].texCoords = Vector2f((int)_dimTile.x, (int)_dimTile.y);
                        quad[3].texCoords = Vector2f(0, (int)_dimTile.y);
                    }
                    else
                    {
                        quad[0].position = Vector2f(0,0);
                        quad[1].position = Vector2f(0,0);
                        quad[2].position = Vector2f(0,0);
                        quad[3].position = Vector2f(0,0);

                        quad[0].texCoords = Vector2f(0, 0);
                        quad[1].texCoords = Vector2f(0, 0);
                        quad[2].texCoords = Vector2f(0, 0);
                        quad[3].texCoords = Vector2f(0, 0);
                    }
                }

            }

        }
    }


}

int TileMap::ajouterCouche()
{
    if(_nbCouche >= 9)
        return _nbCouche;
    else
        _nbCouche++;
    cout << _nbCouche << endl;
    _schema.resize(_nbCouche);
    _schema[_nbCouche-1].resize(_nbTilesLargeurMonde);
    m_vertices.resize(_nbCouche);
    m_vertices[_nbCouche-1].setPrimitiveType(Quads);
    m_vertices[_nbCouche-1].resize(_nbTilesLargeurMonde*_nbTilesHauteurMonde*4);

    for(int i=0; i<_nbTilesLargeurMonde; ++i)
    {
        for(int j=0; j<_nbTilesHauteurMonde; ++j)
        {
            _schema[_nbCouche-1][i].push_back(-1);

            Vertex* quad = &m_vertices[_nbCouche-1][(i+j*_nbTilesLargeurMonde)*4];

            /// on définit ses quatres coins
            /*quad[0].position = Vector2f(i*(int)_dimTile.x*4, j*(int)_dimTile.y*4);
            quad[1].position = Vector2f((i+1)*(int)_dimTile.x*4, j*(int)_dimTile.y*4);
            quad[2].position = Vector2f((i+1)*(int)_dimTile.x*4, (j+1)*(int)_dimTile.y*4);
            quad[3].position = Vector2f(i*(int)_dimTile.x*4, (j+1)*(int)_dimTile.y*4);*/
            quad[0].position = Vector2f(0,0);
            quad[1].position = Vector2f(0,0);
            quad[2].position = Vector2f(0,0);
            quad[3].position = Vector2f(0,0);

            /// on définit ses quatres coordonnés de texture
            quad[0].texCoords = Vector2f(0,0);
            quad[1].texCoords = Vector2f(0,0);
            quad[2].texCoords = Vector2f(0,0);
            quad[3].texCoords = Vector2f(0,0);
        }
    }

    return _nbCouche;
}

int TileMap::enleverCouche()
{
    if(_nbCouche >1)
    {
        m_vertices.pop_back();
        _schema.pop_back();
        _nbCouche--;
    }

    return _nbCouche;
}

void TileMap::drawBackground(sf::RenderTarget& target) const
{
    RenderStates states;
    states.transform*=getTransform();
    states.texture = &m_tileset;

    for(int i=0; i<_couchePerso; ++i)
        target.draw(m_vertices[i], states);
}

void TileMap::drawForeground(sf::RenderTarget& target) const
{
    RenderStates states;
    states.transform*=getTransform();
    states.texture = &m_tileset;

    for(unsigned int i=_couchePerso; i<_nbCouche; ++i)
        target.draw(m_vertices[i], states);
}

void TileMap::drawCouches(sf::RenderTarget& target, unsigned int couche) const
{
    RenderStates states;
    states.transform*=getTransform();
    states.texture = &m_tileset;
    for(unsigned int i=0; i<_nbCouche && i<couche; ++i)
    {
        target.draw(m_vertices[i], states);
    }
    //target.draw(m_vertices[couche-1], states);
}

Vector2f TileMap::positionToTile(Vector2f position) const
{
    Vector2f tile;
    tile.x = (int)(position.x/(_dimTile.x*_ratioSize));
    tile.y = (int)(position.y/(_dimTile.y*_ratioSize));
    return tile;
}

Vector2f TileMap::tileToPosition(Vector2f tile) const
{
    Vector2f position;
    position.x = tile.x*(_dimTile.x*_ratioSize);
    position.y = tile.y*(_dimTile.y*_ratioSize);
    return position;
}
