// spawn_controller.cpp
#include "spawn_controller.h"
#include "game/controllers/controller_factory.h"
#include "game/objects/actor_factory.h"

namespace StevensDev
{

namespace mgc
{

// MEMBER FUNCTIONS
void SpawnController::spawn()
{
    using namespace mgo;
    using namespace gel::math;

    ActorFactory& af = ActorFactory::inst();
    ControllerFactory& cf = ControllerFactory::inst();

    Actor* actor = af.get( d_entity["actor"].asString() );
    Piece* piece = dynamic_cast<Piece*>( actor );

    if ( piece == nullptr )
    {
        af.release( actor );
        return;
    }

    IVec2 space = IVec2( d_column, d_row );
    Vec2 pos = piece->getWorldPositionForSpace( space );

    piece->setCurrentSpace( IVec2( d_column, d_row ) );
    piece->setPosition( pos.x, pos.y );

    d_level->addActor( piece );

    unsigned int i;
    for ( i = 0; i < d_entity["controllers"].asArray().size(); ++i )
    {
       d_level->addController( cf.get( piece, d_entity["controllers"][i] ) );
    }
}

} // End nspc mgc

} // End nspc StevensDev