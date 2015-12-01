// player_controller.h
//
// REFERENCE:
// WU stands for world units
// 1 WU = the width of the world
//
// OU stands for object unit
// 1 OU = the length the minimal width of the bounding box
//
// The minimal width is defined as min( width, height )
//
#ifndef INCLUDED_PLAYER_CONTROLLER
#define INCLUDED_PLAYER_CONTROLLER

#include <game/objects/actor.h>
#include <engine/scene/itickable.h>
#include <engine/input/input.h>
#include <gel/math/vec.h>

namespace StevensDev
{

namespace mgc
{

class PlayerController : public sgds::ITickable
{
  private:
    // CONSTANTS
    static constexpr float MAX_VELOCITY = 100000.0f;
      // The actor's maximum allowed velocity (WU/sec).

    static constexpr float MIN_VELOCITY = 50.0f;
      // The minimum required velocity to be considered in motion (WU/sec).

    static constexpr float ACCELERATION = 17500.0f;
      // The actor's applied acceleration (WU/sec^2).

    static constexpr float KINETIC_FRICTION = 6000.0f;
      // The actor's observed kinetic friction deceleration (WU/sec^2).

    static constexpr float STATIC_FRICTION = 7500.0f;
      // The actor's observed static friction (WU/sec^2).

    // MEMBERS
    gel::math::Vec2 d_acceleration;
      // The actor's current acceleration vector (OU/sec^2).

    gel::math::Vec2 d_velocity;
      // The actor's velocity vector (OU/sec).

    mgo::Actor* d_subject;
      // The controller's subject (or in this case player).

  public:
    // CONSTRUCTORS
    PlayerController();
      // Constructs a new player controller without a subject.

    PlayerController( mgo::Actor* subject );
      // Constructs a new player controller using the given actor as it's
      // subject.

    PlayerController( mgo::Actor* subject,
                      const gel::math::Vec2& acceleration,
                      const gel::math::Vec2& velocity );
      // Constructs a new player controller for the given subject using
      // the initial speeds.

    PlayerController( const PlayerController& controller );
      // Constructs a copy of another player controller.

    ~PlayerController();
      // Destructs the controller.

    // OPERATORS
    PlayerController& operator=( const PlayerController& controller );
      // Makes this a copy of the other controller.

    // ACCESSOR FUNCTIONS
    mgo::Actor* subject() const;
      // Gets a reference to the subject.

    const gel::math::Vec2& acceleration() const;
      // Gets the player's current acceleration.

    const gel::math::Vec2& velocity() const;
      // Gets the player's current velocity.

    // MEMBER FUNCTIONS
    virtual void preTick();
      // Prepares for the update cycle.

    virtual void tick( float dt );
      // Updates the player's position.

    virtual void postTick();
      // Prepares for the next update cycle.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const PlayerController& controller )
{
    sgdd::JsonPrinter p( stream );
    p.open().print( "subject_id", controller.subject()->id() ).close();
    return stream;
}

// CONSTRUCTORS
inline
PlayerController::PlayerController() : d_acceleration(), d_velocity(),
                                       d_subject( nullptr )
{
}

inline
PlayerController::PlayerController( mgo::Actor* subject )
    : d_acceleration(), d_velocity(), d_subject( subject )
{
}

inline
PlayerController::PlayerController( mgo::Actor* subject,
                                    const gel::math::Vec2& acceleration,
                                    const gel::math::Vec2& velocity )
    : d_acceleration( acceleration ), d_velocity( velocity ),
      d_subject( subject )
{
}

inline
PlayerController::PlayerController( const PlayerController& controller )
    : d_acceleration( controller.d_acceleration ),
      d_velocity( controller.d_velocity ),
      d_subject( controller.d_subject )
{
}

inline
PlayerController::~PlayerController()
{
}

// OPERATORS
inline
PlayerController& PlayerController::operator=(
    const PlayerController& controller )
{
    d_acceleration = controller.d_acceleration;
    d_velocity = controller.d_velocity;
    d_subject = controller.d_subject;

    return *this;
}

inline
mgo::Actor* PlayerController::subject() const
{
    return d_subject;
}

inline
const gel::math::Vec2& PlayerController::acceleration() const
{
    return d_acceleration;
}

inline
const gel::math::Vec2& PlayerController::velocity() const
{
    return d_velocity;
}

} // End nspc mgc

} // End nspc StevensDev

#endif