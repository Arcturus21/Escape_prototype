#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "include/TileMap.h"
#include "include/Animation.h"
#include "include/AnimatedSprite.h"
#include "include/PersonnalizedSprite.h"
#include "include/AStar.h"
#include "include/AdvancedParticleSystem.h"
#include "include/CharacterSprite.h"
#include "include/Updatable.h"
#include "LinkedParticleSystem.h"
#include "CrashTurn.h"
#include "Level.h"

using namespace std;
using namespace sf;


void threadUpdate(vector<Updatable*> *tabUpdate)
{
    Clock clock;
    while(true)
    {
        Time frameTime = clock.restart();
        for(unsigned int i=0; i<tabUpdate->size(); ++i)
            (*tabUpdate)[i]->update(frameTime);
        sf::sleep(sf::milliseconds(10));
    }
}

int main(int argc, char* argv[])
{

    ContextSettings settings;
    settings.antialiasingLevel=8;

    RenderWindow app(VideoMode(1200,720,32), "Escape", Style::Default, settings);

    String tabLevel[] = {"level1.txt", "level2.txt", "levelSalle.txt"};
    int indActuel=0;
    valRetourLevel retourLevel;
    /*Thread thread(&threadUpdate, &tabUpdatable);
    thread.launch();*/

    Level level;
        /*if(!level.load("level1.txt"))
        {
            cout << "Erreur chargement level" << endl;
            return EXIT_FAILURE;
        }
        level.play(app);*/

    while(app.isOpen())
    {
        Event event;

        while(app.pollEvent(event))
        {
            if(event.type == Event::Closed)
                app.close();
            if(event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard::Escape)
                {
                    app.close();
                }
                else if(event.key.code == Keyboard::Space)
                {
                    if(!level.load("levelProto1.txt"))
                    {
                        cout << "Erreur chargement level" << endl;
                        return EXIT_FAILURE;
                    }
                    retourLevel = (valRetourLevel)level.play(app);
                }
                else if(event.key.code == Keyboard::Num1)
                {
                    if(!level.load("level1.txt"))
                    {
                        cout << "Erreur chargement level" << endl;
                        return EXIT_FAILURE;
                    }
                    level.play(app);
                }
                else if(event.key.code == Keyboard::Num2)
                {
                    if(!level.load("level2.txt"))
                    {
                        cout << "Erreur chargement level" << endl;
                        return EXIT_FAILURE;
                    }
                    level.play(app);
                }
                else if(event.key.code == Keyboard::Num3)
                {
                    if(!level.load("levelSalle.txt"))
                    {
                        cout << "Erreur chargement level" << endl;
                        return EXIT_FAILURE;
                    }
                    level.play(app);
                }
            }
        }
        app.clear(Color::Red);

        app.display();
    }

    level.clean();
    return EXIT_SUCCESS;
}
