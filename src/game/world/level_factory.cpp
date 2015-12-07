// level_factory.cpp
#include "level_factory.h"
#include "game/objects/actor_factory.h"
#include "game/controllers/controller_factory.h"

namespace StevensDev
{

namespace mgw
{

Level* LevelFactory::get( const sgda::Handle<sgda::LevelDataTag>& handle )
{
    using namespace sgdd;
    sgda::ResourceManager& mgr = sgda::ResourceManager::inst();

    const JsonEntity& data = mgr.get( handle );

    Level* level = d_alloc.get( 1 );
    d_alloc.construct( level, Level() );

    assert( data.has( "board" ) );

    buildMap( level, data["board"] );

    if ( data.has( "entities" ) )
    {
        buildEntities( level, data["entities"] );
    }

    if ( data.has( "controllers" ) )
    {
        buildControllers( level, data[ "controllers" ] );
    }

    return level;
}

void LevelFactory::release( Level* level )
{
    d_alloc.release( level, 1 );
}

// HELPER METHODS
void LevelFactory::buildMap( Level* level, const sgdd::JsonEntity& map )
{
    sgds::WorldView& wv = sgds::WorldView::inst();

    float spaceWidth = wv.width() / map["width"].asInt();
    float spaceHeight = wv.width() / map["height"].asInt();

    Board board( map["spaces"], ( unsigned int )map["width"].asInt(),
                 ( unsigned int )map["height"].asInt(), spaceWidth,
                 spaceHeight );

    level->setBoard( board );
}

void LevelFactory::buildEntities( Level* level,
                                  const sgdd::JsonEntity& entities )
{
    mgo::ActorFactory& aFactory = mgo::ActorFactory::inst();
    mgc::ControllerFactory& cFactory = mgc::ControllerFactory::inst();

    unsigned int i;
    unsigned int c;

    for ( i = 0; i < entities.asArray().size(); ++i )
    {
        mgo::Actor* actor = aFactory.get(
            entities[i]["actor"].asString() );

        level->addActor( actor );

        // add controllers if they are specified
        if ( entities[i].has( "controllers" ) )
        {
            for ( c = 0; c < entities[i]["controllers"].asArray().size(); ++c )
            {
                level->addController(
                    cFactory.get( actor, entities[i]["controllers"][c] ) );
            }
        }
    }
}

void LevelFactory::buildControllers( Level* level,
                                     const sgdd::JsonEntity& controllers )
{
    mgc::ControllerFactory& cFactory = mgc::ControllerFactory::inst();

    unsigned int c;
    for ( c = 0; c < controllers.asArray().size(); ++c )
    {
        const sgdd::JsonEntity& ctrlr = controllers[c];
        level->addController(
            cFactory.get( level, controllers[c] ) );
    }
}

} // End nspc mgw

} // End nspc StevensDev