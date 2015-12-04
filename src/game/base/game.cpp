// game.cpp
#include "game.h"
#include <assert.h>
#include <game/objects/actor.h>
#include <game/controllers/player_controller.h>
#include "engine/assets/resource_manager.h"
#include "engine/scene/world_view.h"
#include "engine/input/input.h"
#include "engine/scene/scene.h"

namespace StevensDev
{

namespace mgb
{

// MEMBER FUNCTIONS
void Game::initialize( const std::string& name )
{
    assert( !d_isDead );

    d_events.push( STATUS_INITIALIZING );

    d_name = name;

    // load resources
    d_resources.addPackageResources( "res/res.db", 0 );

    // load resource information
    sgda::ResourceManager::init( &d_resources );

    // set up game
    sgds::WorldView::initialize( sgds::RectangleBounds( 0, 0, 800, 600 ),
                                 sgds::RectangleBounds( 0, 0, 800, 600 ),
                                 sgds::RectangleBounds( 0, 0, 800, 600 ),
                                 1.0f );
    sgdi::Input& input = sgdi::Input::inst();
    sgds::Scene& scene = sgds::Scene::inst();
    sgds::WorldView& world = sgds::WorldView::inst();

    scene.setRenderer( &d_renderer );

    d_events.push( STATUS_INITIALIZED );
}

void Game::startup()
{
    d_renderer.setupWindow( 800, 600 );

    sgds::WorldView::initialize( sgds::RectangleBounds( 0, 0, 800, 600 ),
                                 sgds::RectangleBounds( 0, 0, 800, 600 ),
                                 sgds::RectangleBounds( 0, 0, 800, 600 ),
                                 1.0f );
    sgdi::Input& input = sgdi::Input::inst();
    sgds::Scene& scene = sgds::Scene::inst();
    sgds::WorldView& world = sgds::WorldView::inst();

    d_isRunning = true;
    d_events.push( STATUS_STARTED );
}

void Game::update()
{
    if ( d_renderer.isActive() )
    {
        sgds::Scene::inst().tick();
    }
    else
    {
        d_isRunning = false;
    }
}

Game::EngineEvent Game::poll()
{
    if ( d_events.size() > 0 )
    {
        return ( EngineEvent )d_events.popFront();
    }

    return NO_EVENT;
}

void Game::shutdown()
{
    d_isDead = true;
}

}  // End nspc mgb

}  // End nspc StevensDev