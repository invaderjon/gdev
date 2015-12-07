// actor_factory.h
#ifndef INCLUDED_ACTOR_FACTORY
#define INCLUDED_ACTOR_FACTORY

#include <engine/assets/resource_database.h>
#include "game/objects/actor.h"
#include "piece.h"

namespace StevensDev
{

namespace mgo
{

class ActorFactory
{
  private:
    // GLOBALS
    static ActorFactory d_instance;
      // Gets the actor factory instance.

    // CONSTRUCTORS
    ActorFactory();
      // Constructs a new actor factory.

    ActorFactory( const ActorFactory& factory );
      // Constructs a copy of a factory.

    // OPERATORS
    ActorFactory& operator=( const ActorFactory& factory );
      // Makes this a copy of the given factory.a

  public:
    // GLOBAL FUNCTIONS
    static ActorFactory& inst();
      // Gets the singleton factory instance.

    // CONSTRUCTORS
    ~ActorFactory();
      // Destructs the actor factory.

    // MEMBER FUNCTIONS
    Actor* get( const sgda::ResourceID& actor );
      // Creates a new actor of the given type.

    void release( mgo::Actor* actor );
      //  Releases the given actor.
};

// GLOBAL FUNCTIONS
inline
ActorFactory& ActorFactory::inst()
{
    return d_instance;
}

// CONSTRUCTORS
inline
ActorFactory::ActorFactory()
{
}

inline
ActorFactory::ActorFactory( const ActorFactory& factory )
{
}

inline
ActorFactory::~ActorFactory()
{
}

// OPERATORS
inline
ActorFactory& ActorFactory::operator=( const ActorFactory& factory )
{
}

// MEMBER FUNCTIONS
inline
void ActorFactory::release( mgo::Actor* actor )
{
    delete actor;
}

} // End nspc mgo

} // End nspc StevensDev

#endif