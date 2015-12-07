// controller_factory.h
#ifndef INCLUDED_CONTROLLER_FACTORY
#define INCLUDED_CONTROLLER_FACTORY

#include <engine/scene/itickable.h>
#include <game/objects/actor.h>
#include <game/world/level.h>

namespace StevensDev
{

namespace mgc
{

class ControllerFactory
{
  private:
    // GLOBALS
    static ControllerFactory d_instance;
      // The factory instance.

  public:
    // GLOBAL FUNCTIONS
    static ControllerFactory& inst();
      // Gets an instance of the controller factory.

    ControllerFactory();
      // Constructs a new controller factory.

    ~ControllerFactory();
      // Destructs a controller factory.

    // MEMBER FUNCTIONS
    sgds::ITickable* get( mgo::Actor* actor, const sgdd::JsonEntity& config );
      // Obtains the specified controller for the given actor.

    sgds::ITickable* get( mgw::Level* level, const sgdd::JsonEntity& config );
      // Obtains the specified controller for the given level.

    void release( sgds::ITickable* controller );
      // Releases the controller.
};

// CONSTRUCTORS
inline
ControllerFactory::ControllerFactory()
{
}

inline
ControllerFactory::~ControllerFactory()
{
}

// GLOBAL FUNCTIONS
inline
ControllerFactory& ControllerFactory::inst()
{
    return d_instance;
}

// MEMBER FUNCTIONS
inline
void ControllerFactory::release( sgds::ITickable* controller )
{
    delete controller;
}


} // End nspc mgc

} // End nspc StevensDev

#endif
