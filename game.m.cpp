// game.m.cpp
#include "engine/build.g.h"
#include "engine/input/input.h"
#include "engine/rendering/renderer.h"
#include "engine/scene/scene.h"
#include "engine/scene/test_input_controller.h"
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

    // set up game
    sgdi::Input& input = sgdi::Input::inst();
    sgds::Scene& scene = sgds::Scene::inst();
    sgdr::Renderer renderer;

    assert( renderer.loadTexture( "block", "res/texture/block.png" ) );
    sgdr::RenderableSprite sprite( renderer.getTexture( "block" ) );

    sgds::TestInputController controller( &sprite );

    renderer.addSprite( &sprite );

    scene.addTickable( &input );
    scene.addTickable( &controller );
    scene.setRenderer( &renderer );

    renderer.setupWindow( 800, 600 );

    while ( renderer.isActive() )
    {
        scene.tick();
    }

    cout << "Finished. Exiting..." << endl;

    return 0;
}