// game.m.cpp
#include <game/objects/actor.h>
#include <game/controllers/player_controller.h>
#include <engine/assets/handle.h>
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

    // set up game
    sgds::World::initialize( sgds::RectangleBounds( 0, 0, 800, 600 ),
                             sgds::RectangleBounds( 0, 0, 800, 600 ),
                             sgds::RectangleBounds( 0, 0, 800, 600 ),
                             1.0f );
    sgdi::Input& input = sgdi::Input::inst();
    sgds::Scene& scene = sgds::Scene::inst();
    sgds::World& world = sgds::World::inst();
    sgdr::Renderer renderer;

    assert( renderer.loadTexture( "block", "res/texture/block.png" ) );
    sgdr::RenderableSprite sprite( renderer.getTexture( "block" ) );

    mgo::Actor actor(
        "test", sprite,
        sgds::RectangleBounds( world.dpToWU( sprite.getPositionX() ),
                               world.dpToWU( sprite.getPositionY() ),
                               world.dpToWU( sprite.getWidth() ),
                               world.dpToWU( sprite.getHeight() ) ),
        0, sgdd::JsonEntity() );



    renderer.addSprite( &actor.sprite() );
    scene.addTickable( &input );
    scene.addTickable( new mgc::PlayerController( &actor ) );
    scene.setRenderer( &renderer );

    renderer.setupWindow( 800, 600 );

    while ( renderer.isActive() )
    {
        scene.tick();
    }

    cout << "Finished. Exiting..." << endl;

    return 0;
}