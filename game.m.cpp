// game.m.cpp
#include <game/objects/actor.h>
#include <game/controllers/player_controller.h>
#include <engine/assets/handle.h>
#include <assert.h>
#include <game/base/game.h>
#include "engine/build.g.h"
#include "engine/input/input.h"
#include "engine/rendering/renderer.h"
#include "engine/scene/scene.h"
#include "engine/util/input_utils.h"
#include "engine/util/game_utils.h"

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

    mgb::Game game;
    game.initialize( "Simulation" );
    while ( game.poll() != mgb::Game::EngineEvent::STATUS_INITIALIZED )
    {
        // wait for initialization
    }

    game.startup();
    while ( game.poll() != mgb::Game::EngineEvent::STATUS_STARTED )
    {
        // wait for startup
    }

    while ( game.isRunning() )
    {
        game.update();
    }

    game.shutdown();

    cout << "Finished. Exiting..." << endl;

    return 0;
}