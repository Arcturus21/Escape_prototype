#include "Level.h"

using namespace std;
using namespace sf;

Level::Level()
{
    //ctor
}

Level::~Level()
{
    //dtor
}

bool Level::loadTexture(std::ifstream& flux)
{
    string buf;
    flux >> buf;
    while(buf!="<Texture>")
    {
        _listTextures.push_back(Texture());
        if(!_listTextures[_listTextures.size()-1].loadFromFile(buf))
        {
            cerr << "erreur chargement textures : " << buf;
        }
        flux >> buf;
    }
    return (buf=="<Texture>");
}

bool Level::loadMap(std::ifstream& flux)
{
    string buf;

    flux >> buf >> buf;
    if(!_tilemap.load(buf))
    {
        cerr << "Erreur chargement Map" << endl;
        return false;
    }
    do
    {
        flux >> buf;
    }while(buf!="<Map>");
    return buf=="<Map>";
}

bool Level::loadSprite(std::ifstream& flux)
{
    string buf, buf2;
    int nb;

    flux >> buf;
    while(buf!="<Sprite>")
    {
        if(buf=="<joueur>")
        {
            flux >> buf >> buf2 >> nb;
            _joueur.load(buf2, _listTextures[nb]);
            _joueur.setTilemap(_tilemap);
            _joueur.setPosition(_tilemap.getPosDepart());
            flux >> buf;
            if(buf!="<joueur>")
                return false;

            _listUpdatable.push_back(&_joueur);
        }
        else if(buf=="<limites>")
        {
            flux >> buf >> nb;
            _listSprite.push_back(PersonnalizedSprite());
            _listSprite[0].load(buf, _listTextures[nb]);
            _listSprite[0].setPosition(_tilemap.getPosDepart());

            _listUpdatable.push_back(&_listSprite[0]);

            flux >> buf >> nb;
            _listSprite.push_back(PersonnalizedSprite());
            _listSprite[1].load(buf, _listTextures[nb]);
            _listSprite[1].setPosition(_tilemap.getPosArrive());

            _listUpdatable.push_back(&_listSprite[1]);

            flux >> buf;
            if(buf!="<limites>")
                return false;
        }
        else if(buf=="<crashturn>")
        {
            flux >> buf;
            int i=0;
            while(buf!="<crashturn>")
            {
                flux >> buf2 >> nb;
                _listCrashTurn.push_back(CrashTurn());
                _listCrashTurn[i].load(buf2, _listTextures[nb]);
                _listCrashTurn[i].setPosition(300,2000);
                _listCrashTurn[i].init(_joueur, _tilemap);
                flux >> buf;
                if(buf=="black")
                    _listCrashTurn[i].setColor(Color::Black);
                else if(buf=="red")
                    _listCrashTurn[i].setColor(Color::Red);
                else if(buf=="Yellow")
                    _listCrashTurn[i].setColor(Color::Yellow);
                else if(buf=="Green")
                    _listCrashTurn[i].setColor(Color::Green);
                else
                    _listCrashTurn[i].setColor(Color::White);

                _listUpdatable.push_back(&(_listCrashTurn[i]));

                i++;
                flux >> buf;
            }
            if(buf!="<crashturn>")
                return false;
        }

        flux >> buf;
    }

    return (buf=="<Sprite>");
}

bool Level::loadParticle(std::ifstream& flux)
{
    string buf, buf2;
    int nb1, nb2, nb3, i=0;

    flux >> buf;

    while(buf!="<Particle>")
    {
        flux >> buf2 >> nb1;
        if(buf2=="point")
            _listParticle.push_back(LinkedParticleSystem(nb1, Points));
        else if(buf2=="quad")
            _listParticle.push_back(LinkedParticleSystem(nb1, Quads));
        flux >> nb1;
        if(nb1==1)
        {
            flux >> nb1;
            if(nb1==-1)
            {
                _listParticle[i].setCharacRef(_joueur);
            }
            else
            {
                _listParticle[i].setCharacRef(_listCrashTurn[nb1]);
                _listUpdatable.push_back(&(_listParticle[i]));
            }


        }
        else
        {
            flux >> nb1 >> nb2;
            cout << endl << nb1 << " " << nb2 << endl;
            _listParticle[i].setEmitter(Vector2f(nb1, nb2));
        }
        flux >> buf;
        if(buf=="black")
            _listParticle[i].setColor(Color::Black);
        else if(buf=="red")
            _listParticle[i].setColor(Color::Red);
        else if(buf=="Yellow")
            _listParticle[i].setColor(Color::Yellow);
        else if(buf=="Green")
            _listParticle[i].setColor(Color::Green);
        else
            _listParticle[i].setColor(Color::White);

        ++i;
        flux >> buf;
    }

    return (buf=="<Particle>");
}

bool Level::loadCheckpoint(std::ifstream& flux)
{
    string buf, buf2;
    int nb1, nb2, nb3;
    Vector2f pos, dim;

    flux >> buf;
    while(buf != "<Checkpoint>")
    {
        /*flux >> pos.x >> pos.y >> dim.x >> dim.y >> nb1;
        if(nb1==emetteurCrashTurn)
        {
            flux >> nb2 >> buf >> nb3 >> buf2;
        }
        else if(nb1==trigger)
        {

        }*/

        flux >> buf;
    }

    return (buf=="<Checkpoint>");
}

bool Level::loadScript(std::ifstream& flux)
{
    string buf;
    while(buf!="<Script>")
    {
        flux >> buf;
    }
    return true;
}

bool Level::load(std::string fichier)
{
    string buf;

    reset();
    ///Ajouter gestion exception après
    ifstream flux(fichier.c_str());
    if(!flux)
        return false;

    flux >> buf;
    if(buf!="<Level>")
    {
        cout << "Fichier invalide" << endl;
        return false;
    }

    flux >> buf;
    while(buf!="<Level>")
    {
        if(buf=="<Texture>")
        {
            if(!loadTexture(flux))
            {
                cout << "Erreur chargement textures" << endl;
                return false;
            }
            cout << "Textures" << endl;
        }
        else if(buf=="<Map>")
        {
            if(!loadMap(flux))
            {
                cout << "Erreur chargement map" << endl;
                return false;
            }
            cout << "Map" << endl;
        }
        else if(buf=="<Sprite>")
        {
            if(!loadSprite(flux))
            {
                cout << "Erreur chargement sprite" << endl;
                return false;
            }
            cout << "Sprite" << endl;
        }
        else if(buf=="<Particle>")
        {
            if(!loadParticle(flux))
            {
                cout << "Erreur chargement particules" << endl;
                return false;
            }
            cout << "Particles" << endl;
        }
        else if(buf=="<Checkpoint>")
        {
            if(!loadCheckpoint(flux))
            {
                cout << "Erreur chargement checkpoint" << endl;
                return false;
            }
            cout << "Checkpoints" << endl;
        }
        else if(buf=="<Script>")
        {
            if(!loadScript(flux))
            {
                cout << "Erreur chargement script" << endl;
                return false;
            }
            cout << "Script" << endl;
        }
        else
        {
            cout << "Cas non traité" << endl;
        }
        flux >> buf;
    }

    return buf=="<Level>";
}

void Level::reset()
{
    _listCrashTurn.clear();
    _listCheckpoint.clear();
    _listParticle.clear();
    _listTextures.clear();
    _listSprite.clear();
    _listUpdatable.clear();
    _tilemap.reset();
}

void Level::update(sf::Time frameTime)
{
    /*_joueur.update(frameTime);
    for(int i=0; i<_listCrashTurn.size(); ++i)
    {
        _listCrashTurn[i].update(frameTime);
    }
    for(int i=0; i<_listParticle.size(); ++i)
    {
        _listParticle[i].update(frameTime);
    }
    for(int i=0; i<_listSprite.size(); ++i)
    {
        _listSprite[i].update(frameTime);
    }*/

    for(int i=0; i<_listUpdatable.size();++i)
    {
        _listUpdatable[i]->update(frameTime);
    }
    _listParticle[0].update(frameTime);

}

void Level::render(sf::RenderTarget& app)
{
    _tilemap.drawBackground(app);

    for(int i=0; i<_listSprite.size(); ++i)
        app.draw(_listSprite[i]);
    for(int i=0; i<_listParticle.size(); ++i)
        app.draw(_listParticle[i]);
    app.draw(_joueur);
    for(int i=0; i<_listCrashTurn.size(); ++i)
        app.draw(_listCrashTurn[i]);
}

int Level::play(sf::RenderWindow& app)
{
    Clock clock;

    View view(FloatRect(0,0,_tilemap.getDimMap().x, _tilemap.getDimMap().y));

    while(app.isOpen())
    {
        ///Compteur temps
        Time frameTime = clock.restart();
        /** GESTION DES EVENEMENTS */
        Event event;

        /** Evenements ponctuels */
        while(app.pollEvent(event))
        {
            if(event.type == Event::Closed)
                app.close();
            if(event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard::Escape)
                {
                    return level_quitter;
                }
            }

        }
        /** Evenements continues */
        if(Keyboard::isKeyPressed(Keyboard::Z))
        {
            _joueur.setDirection(1,-1);
        }
        else if(Keyboard::isKeyPressed(Keyboard::S))
        {
            _joueur.setDirection(1,1);
        }
        else
        {
            _joueur.setDirection(1,0);
        }
        if(Keyboard::isKeyPressed(Keyboard::Q))
        {
           _joueur.setDirection(0,-1);
        }
        else if(Keyboard::isKeyPressed(Keyboard::D))
        {
           _joueur.setDirection(0,1);
        }
        else
        {
           _joueur.setDirection(0,0);
        }

        update(frameTime);

        if(_listSprite[1].getGlobalBounds().intersects(_joueur.getGlobalBounds()))
            return level_victoire;
        else if(_joueur.getGlobalBounds().intersects(_listCrashTurn[0].getGlobalBounds()))
            return level_defaite;

        app.clear();
        app.setView(view);

        render(app);
        app.display();
    }
    return level_victoire;
}
