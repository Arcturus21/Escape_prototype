#include "GenerateurMonstre.h"

using namespace sf;
using namespace std;

GenerateurMonstre::GenerateurMonstre(sf::Vector2f pos, int nbMonstre,
                          CrashTurn& modele, LinkedParticleSystem& modeleParticle,
                          std::vector<CrashTurn*>& listMonstres, std::vector<AdvancedParticleSystem*>& listParticle, std::vector<Updatable*>& listUpdatable) :
_modeleParticle(modeleParticle)
{
    //ctor
    _nbMonstre = nbMonstre;
    _nbMonstreGenerer=0;
    _modele = modele;
    _emitter = pos;
    _modele.setPosition(_emitter);
    _listMonstresLevel=&listMonstres;
    _linkedParticle=&listParticle;
    _listUpdatable=&listUpdatable;
}

GenerateurMonstre::~GenerateurMonstre()
{
    //dtor
}

void GenerateurMonstre::update(sf::Time elapsed)
{
    _timeElapsed+=elapsed;
    if(_timeElapsed.asMilliseconds()>100 && _nbMonstreGenerer<_nbMonstre)
    {
        genereMonstre();
        _timeElapsed = Time::Zero;
    }
    /*for(int i=0; i<_listMonstres.size(); i++)
    {
        _listMonstres[i].update(elapsed);
        //_linkedParticle[i].update(elapsed);
    }*/
}

bool GenerateurMonstre::genereMonstre()
{
    if(_nbMonstreGenerer >= _nbMonstre)
        return false;

    CrashTurn* nouveau= new CrashTurn();
    *nouveau = _modele;
    nouveau->init(*_modele.getCible(), *_modele.getMap());

    LinkedParticleSystem* particle = new LinkedParticleSystem(_modeleParticle);
    particle->setCharacRef(*nouveau);

    _listMonstresLevel->push_back(nouveau);
    _linkedParticle->push_back(particle);
    _listUpdatable->push_back(nouveau);
    _listUpdatable->push_back(particle);

    _nbMonstreGenerer++;

    return true;
}
