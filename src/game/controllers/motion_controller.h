// motion_controller.h
//
// This controller linearly interpolates "board piece" motion between "board spaces".
// The end effect is smooth motion that is locked to a large grid.
#ifndef INCLUDED_MOTION_CONTROLLER
#define INCLUDED_MOTION_CONTROLLER

#include <engine/scene/itickable.h>
#include <game/objects/piece.h>

namespace StevensDev
{

namespace mgc
{

class MotionController : public sgds::ITickable
{
  private:
    // MEMBERS
    mgo::Piece* d_piece;
      // The game piece being controlled.

    float d_speed;
      // The speed of the piece in WU/sec.

    // HELPER FUNCTIONS
    gel::math::Vec2 getDir() const;
      // Gets the current direction vector of the piece.

  public:
    // CONSTRUCTORS
    MotionController();
      // Constructs a new motion controller without a piece.

    MotionController( mgo::Piece* piece, float speed );
      // Constructs a new motion controller that moves the given piece
      // at the specified speed.

    MotionController( const MotionController& controller );
      // Constructs a copy of the controller.

    ~MotionController();
      // Destructs the motion controller.

    // OPERATORS
    MotionController& operator=( const MotionController& controller );
      // Makes this a copy of the given controller.

    // MEMBER FUNCTIONS
    virtual void preTick();
      // Does nothing.

    virtual void tick( float dtS );
      // Moves the piece along its specified motion path..

    virtual void postTick();
      // Does nothing.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const MotionController& controller )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
MotionController::MotionController() : d_piece( nullptr ), d_speed( 1.0f )
{
}

inline
MotionController::MotionController( mgo::Piece* piece, float speed )
    : d_piece( piece ), d_speed( speed )
{
}

inline
MotionController::MotionController( const MotionController& controller )
    : d_piece( controller.d_piece ), d_speed( controller.d_speed )
{
}

inline
MotionController::~MotionController()
{
}

// OPERATORS
inline
MotionController& MotionController::operator=(
                const MotionController& controller )
{
    d_piece = controller.d_piece;
    d_speed = controller.d_speed;

    return *this;
}

// MEMBER FUNCTIONS
inline
void MotionController::preTick()
{
    // does nothing
}

inline
void MotionController::postTick()
{
    // does nothing
}

} // End nspc mgc

} // End nspc StevensDev

#endif
