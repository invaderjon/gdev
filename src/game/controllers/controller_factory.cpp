// controller_factory.cpp
#include "controller_factory.h"
#include "engine/util/hasher.h"
#include "ghost_controller.h"
#include "motion_controller.h"
#include "pacman_controller.h"
#include "player_controller.h"
#include "spawn_controller.h"

namespace StevensDev
{

namespace mgc
{

// GLOBALS
ControllerFactory ControllerFactory::d_instance = ControllerFactory();

// MEMBER FUNCTIONS
sgds::ITickable* ControllerFactory::get( mgo::Actor* actor,
                                         const sgdd::JsonEntity& config )
{
    using namespace sgdu;

    sgds::ITickable* controller;
    std::string conf = config["class"].asString();
    switch( Hasher<std::string>::hash( config["class"].asString() ) )
    {
        case chash( "PlayerController" ):
            controller = new PlayerController( actor );
            break;

        case chash( "MotionController" ):
            controller = new MotionController(
                dynamic_cast<mgo::Piece*>( actor ),
                static_cast<float>( config["speed"].asDouble() ) );
            break;

        case chash( "GhostController" ):
            controller = new GhostController(
                dynamic_cast<mgo::Ghost*>( actor ) );
            break;

        case chash( "PacmanController" ):
            controller = new PacmanController(
                dynamic_cast<mgo::Piece*>( actor ) );
            break;

        default:
            controller = nullptr;
            break;
    }

    return controller;
}

sgds::ITickable* ControllerFactory::get( mgw::Level* level,
                                         const sgdd::JsonEntity& config )
{
    using namespace sgdu;

    sgds::ITickable* controller;
    switch ( Hasher<std::string>::hash( config["class"].asString() ) )
    {
        case chash( "SpawnController" ):
            controller = new SpawnController(
                level, config["entity"],
                static_cast<unsigned int>( config["column"].asInt() ),
                static_cast<unsigned int>( config["row"].asInt() ) );
            break;

        default:
            controller = nullptr;
            break;
    }

    return controller;
}

} // End nspc mgc

} // End nspc StevensDev