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
    mgo::ActorFactory& af = mgo::ActorFactory::inst();
    mgc::ControllerFactory& cf = mgc::ControllerFactory::inst();
    sgds::Scene& s = sgds::Scene::inst();

    mgo::Actor* actor;
    while ( d_actors.size() > 0 )
    {
        actor = d_actors.pop();
        s.removeRenderable( &actor->sprite() );
        af.release( actor );
    }

    sgds::ITickable* controller;
    while ( d_controllers.size() > 0 )
    {
        controller = d_controllers.pop();
        s.removeTickable( controller );
        cf.release( controller );
    }

    while ( d_newActors.size() > 0 )
    {
        actor = d_newActors.pop();
        af.release( actor );
    }

    while ( d_newControllers.size() > 0 )
    {
        controller = d_newControllers.pop();
        cf.release( controller );
    }
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

} // End nspc mgw

} // End nspc StevensDev