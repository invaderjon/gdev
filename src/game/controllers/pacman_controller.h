// pacman_controller.h
#ifndef GDEV_PACMAN_CONTROLLER_H
#define GDEV_PACMAN_CONTROLLER_H

#include <game/objects/piece.h>

namespace StevensDev
{

namespace mgc
{

class PacmanController : public sgds::ITickable
{
  private:
    mgo::Piece* d_piece;
      // The piece to control.

    // HELPER FUNCTIONS
    gel::math::IVec2 getDesiredDir() const;
      // Calculates the direction the player is trying to move in.

    gel::math::IVec2 getCurrentDir() const;
      // Calculates the direction the player is currently moving in.

    gel::math::IVec2 getInputDir() const;
      // Gets the current input direction.

    gel::math::IVec2 getProjection() const;
      // Projects the actor's current direction.
  public:
    // CONSTRUCTORS
    PacmanController();
      // Constructs a pacman controller without a target.

    PacmanController( mgo::Piece* piece );
      // Constructs a pacman controller for the given target.

    PacmanController( const PacmanController& controller );
      // Constructs a copy of the given controller.

    ~PacmanController();
      // Destructs the pacman controller.

    // OPERATORS
    PacmanController& operator=( const PacmanController& controller );
      // Makes this a copy of the given controller.

    // MEMBER FUNCTIONS
    virtual void preTick();
      // Updates pacman's position.

    virtual void tick( float dtS );
      // Does nothing.

    virtual void postTick();
      // Does nothing.
};

// CONSTRUCTORS
inline
PacmanController::PacmanController()
{
}

inline
PacmanController::PacmanController( mgo::Piece* piece ) : d_piece( piece )
{
}

inline
PacmanController::PacmanController( const PacmanController& controller )
    : d_piece( controller.d_piece )
{
}

inline
PacmanController::~PacmanController()
{
}

// OPERATORS
inline
PacmanController& PacmanController::operator=(
    const PacmanController& controller )
{
    d_piece = controller.d_piece;

    return *this;
}

// MEMBER FUNCTIONS
inline
void PacmanController::tick( float dtS )
{

}

inline
void PacmanController::postTick()
{
    // does nothing
}

} // End nspc mgc

} // End nspc PacmanController

#endif
