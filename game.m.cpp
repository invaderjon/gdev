// game.m.cpp
#include "engine/build.gen.h"
#include "engine/game/resource_manager.h"
#include "engine/util/game_utils.h"
#include <sstream>

int main( int argc, char *argv[] )
{
    using namespace std;
    using namespace StevensDev;
    using namespace StevensDev::sgdu;

    int i;

    // print out debug information
    cout << "CS 585 Intro to Game Development" << endl;
    cout << "Game" << endl;
    cout << "Version: " << GAME_VERSION << endl;

    cout << "Arguments: ";
    for ( i = 1; i < argc; ++i )
    {
        cout << argv[i] << " ";
    }
    cout << endl;

    // prepare resources
    sgdg::ResourceManager resMgr;
    resMgr.fastLoadFont( "ubuntu-mono-r" );

    sf::Font& font = resMgr.getFont( "ubuntu-mono-r" );

    // create window
    sf::RenderWindow window;
    window.create( sf::VideoMode( 600, 400 ), "Game" );
    window.setFramerateLimit( 60 );

    // main loop
    sf::Clock timer;
    float fps = 60;
    bool running = true;
    while ( running )
    {
        fps = GameUtils::calculateFPS( timer.getElapsedTime().asSeconds(),
                                       fps );
        timer.restart();

        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            switch ( event.type )
            {
                case sf::Event::Closed:
                    running = false;
                    window.close();
                    break;

                default:
                    break;
            }
        }
        window.clear( sf::Color( 0, 0, 0 ) );

        // build debug text
        std::ostringstream oss;
        oss << "Introduction to Game Development" << endl;
        oss << "Mode: Debug" << endl;
        oss << "FPS: " << fps << endl;
        sf::String fpsMsg( oss.str() );

        sf::Text fpsText( fpsMsg, font );
        fpsText.setCharacterSize( 14 );

        window.draw( fpsText );

        window.display();
    }

    cout << "Finished. Exiting..." << endl;

    return 0;
}