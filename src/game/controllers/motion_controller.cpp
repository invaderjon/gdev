// motion_controller.cpp
#include "motion_controller.h"

namespace StevensDev
{

namespace mgc
{

// MEMBER FUNCTIONS
void MotionController::tick( float dtS )
{
    using namespace gel::math;

    if ( d_piece->getTargetCount() <= 0 )
    {
        return;
    }

    Vec2 dir = getDir();

    assert( dir != Vec2( 0, 0 ) );

    Vec2 trans = Vec::normalize( dir ) * d_speed * dtS;

    float dirLen = Vec::length( dir );
    float transLen = Vec::length( trans );

    // passing through node?
    if ( transLen > dirLen )
    {
        d_piece->move( dir.x, dir.y );
        d_piece->jumpToNextTarget();
        dtS *= dirLen / transLen;
        tick( dtS );
        return;
    }

    d_piece->move( trans.x, trans.y );
}

// HELPER FUNCTIONS
gel::math::Vec2 MotionController::getDir() const
{
    using namespace gel::math;

    if ( d_piece->getTargetCount() <= 0 )
    {
        return gel::math::Vec2( 0 );
    }

    IVec2 space = d_piece->getTargetSpace( 0 );

    Vec2 pos( d_piece->getPositionX(), d_piece->getPositionY() );
    Vec2 tar = d_piece->getWorldPositionForSpace( space );

    return gel::math::Vec2( tar - pos );
}

} // End nspc mgc

} // End nspc StevensDev