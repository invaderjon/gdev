// ghost_controller.h
#ifndef INCLUDED_GHOST_CONTROLLER
#define INCLUDED_GHOST_CONTROLLER

#include <engine/scene/itickable.h>
#include <game/objects/ghost.h>

namespace StevensDev
{

namespace mgc
{

class GhostController : public sgds::ITickable
{
  private:
    // MEMBERS
    mgo::Ghost* d_ghost;
      // The ghost that is being controlled.

  public:
    // CONSTRUCTORS
    GhostController();
      // Constructs a ghost controller with no target.

    GhostController( mgo::Ghost* ghost );
      // Constructs a ghost controller for the given ghost.

    GhostController( const GhostController& controller );
      // Constructs a copy of the given controller.

    ~GhostController();
      // Destructs the controller.

    // OPERATORS
    GhostController& operator=( const GhostController& controller );
      // Makes this a copy of the given controller.

    // MEMBER FUNCTIONS
    virtual void preTick();
      // Updates the

    virtual void tick( float dtS );
      // Does nothing

    virtual void postTick();
      //
};

// CONSTRUCTORS
inline
GhostController::GhostController() : d_ghost( nullptr )
{
}

inline
GhostController::GhostController( mgo::Ghost* ghost ) : d_ghost( ghost )
{
}

inline
GhostController::GhostController( const GhostController& controller )
    : d_ghost( controller.d_ghost )
{
}

inline
GhostController::~GhostController()
{
}

// OPERATORS
inline
GhostController& GhostController::operator=(
    const GhostController& controller )
{
    d_ghost = controller.d_ghost;

    return *this;
}

// MEMBER FUNCTIONS
inline
void GhostController::tick( float dtS )
{
    // does nothing
}

inline
void GhostController::postTick()
{
    // does nothing
}

} // End nspc mgc

} // End nspc StevensDev

#endif