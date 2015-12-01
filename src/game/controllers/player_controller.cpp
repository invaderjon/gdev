// player_controller.cpp
#include "player_controller.h"

namespace StevensDev
{

namespace mgc
{

// CONSTANT DECLARATIONS
constexpr float PlayerController::MAX_VELOCITY;
constexpr float PlayerController::MIN_VELOCITY;
constexpr float PlayerController::ACCELERATION;
constexpr float PlayerController::KINETIC_FRICTION;
constexpr float PlayerController::STATIC_FRICTION;

// MEMBER FUNCTIONS
void PlayerController::preTick()
{
    using namespace sgdi;
    using namespace gel::math;

    Input& in = Input::inst();

    float vertical = 0.0f;
    float horizontal = 0.0f;

    if ( in.isDown( Input::KEY_W ) )
    {
        vertical -= 1.0f;
    }

    if ( in.isDown( Input::KEY_S ) )
    {
        vertical += 1.0f;
    }

    if ( in.isDown( Input::KEY_A ) )
    {
        horizontal -= 1.0f;
    }

    if ( in.isDown( Input::KEY_D ) )
    {
        horizontal += 1.0f;
    }

    // pre-calculate acceleration
    if ( horizontal != 0.0f || vertical != 0.0f )
    {
        d_acceleration = ACCELERATION *
                         Vec::normalize( Vec2( horizontal, vertical ));
    }
    else
    {
        d_acceleration = Vec2( 0.0f, 0.0f );
    }
}

void PlayerController::tick( float dt )
{
    using namespace sgds;
    using namespace gel::math;

    const sgds::RectangleBounds& b = d_subject->bounds();
    World& w = World::inst();

    // check if it meets the minimum velocity for movement
    bool hasMinVelocity = Vec::length( d_velocity ) > MIN_VELOCITY;

    // is it moving faster than cutoff speed?
    // or is the acceleration greater than the static friction?
    if ( hasMinVelocity ||
         Vec::length( d_acceleration ) > STATIC_FRICTION )
    {
        // calculate the applied friction
        Vec2 friction;
        if ( hasMinVelocity )
        {
            friction = KINETIC_FRICTION * Vec::normalize( -d_velocity );
        }
        else
        {
            friction = Vec2( 0.0f );
        }

        // calculate change in velocity due to friction
        Vec2 dVf = friction * dt;

        // prevent dVf from being greater than the velocity
        if ( Vec::length( dVf ) > Vec::length( d_velocity ) )
        {
            dVf = -d_velocity;
        }

        // apply forces to velocity
        d_velocity += d_acceleration * dt + dVf;
    }

    // check if new velocity meets cutoff
    if ( Vec::length( d_velocity ) > MIN_VELOCITY )
    {
        const float maxDisplacement = MAX_VELOCITY * dt;

        // calculate displacement
        Vec2 dP = d_velocity * dt;

        // clamp displacement to one bounds offset
        if ( Vec::length( dP ) > maxDisplacement )
        {
            dP = Vec::normalize( dP ) * maxDisplacement;
        }

        // clamp movement to inside of the world
        float nl = b.left() + dP.x;
        float nt = b.top() + dP.y;
        float nr = b.right() + dP.x;
        float nb = b.bottom() + dP.y;

        if ( nl < w.left() )
        {
            dP.x = b.left() - w.left();
            d_velocity.x *= 0.05f;
        }
        else if ( nr > w.right() )
        {
            dP.x = w.right() - b.right();
            d_velocity.x *= 0.05f;
        }

        if ( nt < w.top() )
        {
            dP.y = b.top() - w.top();
            d_velocity.y *= 0.05f;
        }
        else if ( nb > w.bottom() )
        {
            dP.y = w.bottom() - b.bottom();
            d_velocity.y *= 0.05f;
        }

        // move subject
        d_subject->move( dP.x, dP.y );
    }
}

void PlayerController::postTick()
{
}

} // End nspc mgc

} // End nspc StevensDev