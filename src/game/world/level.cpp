// level.cpp
#include "level.h"
#include "game/objects/actor_factory.h"
#include "game/controllers/controller_factory.h"

namespace StevensDev
{

namespace mgw
{

// MEMBER FUNCTIONS
void Level::close()
{
    sgda::ResourceManager& mgr = sgda::ResourceManager::inst();
    mgo::ActorFactory& af = mgo::ActorFactory::inst();
    mgc::ControllerFactory& cf = mgc::ControllerFactory::inst();
    sgds::Scene& s = sgds::Scene::inst();

    s.removeTickable( this );

    // remove map
    while ( d_mapSprites.size() > 0 )
    {
        s.removeRenderable( &d_mapSprites[0] );
        d_mapSprites.popFront();
    }
    mgr.release( d_mapTexHandle );

    // remove actors
    mgo::Actor* actor;
    while ( d_actors.size() > 0 )
    {
        actor = d_actors.pop();
        s.removeRenderable( &actor->sprite() );
        af.release( actor );
    }

    // remove controllers
    sgds::ITickable* controller;
    while ( d_controllers.size() > 0 )
    {
        controller = d_controllers.pop();
        s.removeTickable( controller );
        cf.release( controller );
    }

    // remove pending actors
    while ( d_newActors.size() > 0 )
    {
        actor = d_newActors.pop();
        af.release( actor );
    }

    // remove pending controllers
    while ( d_newControllers.size() > 0 )
    {
        controller = d_newControllers.pop();
        cf.release( controller );
    }
}

void Level::open()
{
    sgds::Scene& s = sgds::Scene::inst();

    s.addTickable( this );

    // add drawable map
    buildMap();
    unsigned int i;
    for ( i = 0; i < d_mapSprites.size(); ++i )
    {
        s.addRenderable( &d_mapSprites[i] );
    }

    // add actors
    mgo::Actor* actor;
    while ( d_newActors.size() > 0 )
    {
        actor = d_newActors.popFront();
        s.addRenderable( &actor->sprite() );
        d_actors.push( actor );
    }

    // add controllers
    sgds::ITickable* controller;
    while ( d_newControllers.size() > 0 )
    {
        controller = d_newControllers.popFront();
        s.addTickable( controller );
        d_controllers.push( controller );
    }

    mgw::Board::set( d_board );
}

void Level::postTick()
{
    sgds::Scene& s = sgds::Scene::inst();

    mgo::Actor* actor;
    while ( d_newActors.size() > 0 )
    {
        actor = d_newActors.popFront();
        s.addRenderable( &actor->sprite() );
        d_actors.push( actor );
    }

    sgds::ITickable* controller;
    while ( d_newControllers.size() > 0 )
    {
        controller = d_newControllers.popFront();
        s.addTickable( controller );
        d_controllers.push( controller );
    }
}

// HELPER FUNCTIONS
void Level::buildMap()
{
    sgda::ResourceManager& mgr = sgda::ResourceManager::inst();

    mgr.loadTexture( "R.texture.world.traversable", true );

    d_mapTexHandle = mgr.getTextureHandle( "R.texture.world.traversable" );

    sf::Texture& worldTex = mgr.get( d_mapTexHandle );

    unsigned int col;
    unsigned int row;
    for ( col = 0; col < d_board.width(); ++col )
    {
        for ( row = 0; row < d_board.height(); ++row )
        {
            if ( d_board.isTraversable( col, row ) )
            {
                sgdr::RenderableSprite sprite( worldTex );

                sprite.setPosition( col * d_board.spaceWidth(),
                                    row * d_board.spaceHeight() );

                d_mapSprites.push( sprite );
            }
        }
    }
}

} // End nspc mgw

} // End nspc StevensDev