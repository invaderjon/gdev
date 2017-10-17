// collision_controller.h
#ifndef INCLUDED_COLLISION_CONTROLLER
#define INCLUDED_COLLISION_CONTROLLER

#include "game/objects/actor.h"

namespace StevensDev
{

namespace mgc
{

class CollisionController : public sgds::ITickable
{
  private:
    // MEMBERS
    mgo::Actor* d_actor;
      // The piece being controlled.

  public:
    // CONSTRUCTORS
    CollisionController();
      // Constructs a collision controller.

    CollisionController( mgo::Actor* actor );
      // Constructs a collision controller for the given actor.

    CollisionController( const CollisionController& controller );
      // Constructs a copy of the given controller.

    ~CollisionController();
      // Destructs the controller.

    // MEMBER FUNCTIONS
    virtual void preTick();
      // Does nothing.

    virtual void tick( float dtS );
      // Detects collision.

    virtual void postTick();
      // Does nothing
};

// CONSTRUCTORS
inline
CollisionController::CollisionController() : d_actor( nullptr )
{
}

inline
CollisionController::CollisionController( mgo::Actor* actor )
    : d_actor( actor )
{
}

inline
CollisionController::CollisionController(
    const CollisionController& controller ) : d_actor( controller.d_actor )
{
}

inline
CollisionController::~CollisionController()
{
}

// MEMBER FUNCTIONS
inline
void CollisionController::preTick()
{
}

inline
void CollisionController::postTick()
{
}

} // End nspc mgc

} // End nspc StevensDev

#endif
