// pacman_controller.cpp
#include "pacman_controller.h"
#include <engine/input/input.h>

namespace StevensDev
{

namespace mgc
{

// MEMBER FUNCTIONS
void PacmanController::preTick()
{
    using namespace gel::math;

    // parse input
    IVec2 in = getDesiredDir();
    IVec2 cur = getCurrentDir();

    if ( in != IVec2( 0 ) && in != cur )
    {
        if ( in == -cur ) // reversing (has targets)
        {
            IVec2 src = d_piece->getTargetSpace( 0 );
            IVec2 tar = d_piece->getCurrentSpace();
            d_piece->clearTargets();
            d_piece->setCurrentSpace( src );
            d_piece->addTarget( tar );
        }
        else if ( cur != IVec2( 0, 0 ) ) // turning (has targets)
        {
            IVec2 tar = d_piece->getTargetSpace( 0 );
            IVec2 dst = tar + in;

            d_piece->clearTargets();
            d_piece->addTarget( tar );
            d_piece->addTarget( dst );
        }
        else // moving from stationary
        {
            d_piece->clearTargets();
            d_piece->addTarget( d_piece->getCurrentSpace() + in );
        }

        cur = in;
    }

    // project direction
    IVec2 proj;
    while ( d_piece->getTargetCount() < 5 &&
            ( proj = getProjection() ) != d_piece->getCurrentSpace() )
    {
        d_piece->addTarget( proj );
    }
}

// HELPER FUNCTIONS
gel::math::IVec2 PacmanController::getDesiredDir() const
{
    using namespace gel::math;

    IVec2 cur = getCurrentDir();
    IVec2 in = getInputDir();
    IVec2 dst;

        if ( in == IVec2( 0 ) || in == cur )
    {
        return in;
    }

    // filter out current direction
    in.x = ( in.x == cur.x ) ? 0 : in.x;
    in.y = ( in.y == cur.y ) ? 0 : in.y;

    // reversing takes priority (always possible)
    if ( ( in.x == -cur.x && in.x != 0 ) ||
         ( in.y == -cur.y && in.y != 0 ) ) // reversing
    {
        return -cur;
    }
    else if ( cur != Vec2( 0 ) ) // turning
    {
        dst = d_piece->getTargetSpace( 0 ) + in;
    }
    else // from stationary
    {
        IVec2 src = d_piece->getCurrentSpace();
        dst.x = src.x + in.x;
        dst.y = src.y;
        if ( dst == src )
        {
            dst.x = src.x;
            dst.y = src.y + in.y;
            in.x = 0;
        }
        else
        {
            in.y = 0;
        }
    }

    // ensure new target is traversible
    if ( !mgw::Board::get().isTraversable(
            static_cast<unsigned int>( dst.x ),
            static_cast<unsigned int>( dst.y ) ) )
    {
        return IVec2( 0 );
    }

    return in;
}

gel::math::IVec2 PacmanController::getCurrentDir() const
{
    using namespace gel::math;

    if ( d_piece->getTargetCount() <= 0 )
    {
        return IVec2( 0 );
    }

    return d_piece->getTargetSpace( 0 ) - d_piece->getCurrentSpace();
}

gel::math::IVec2 PacmanController::getProjection() const
{
    using namespace gel::math;

    if ( d_piece->getTargetCount() < 1 )
    {
        return d_piece->getCurrentSpace();
    }

    IVec2 src = d_piece->getTargetSpace( d_piece->getTargetCount() - 1 );
    IVec2 dir = d_piece->getDirection( d_piece->getTargetCount() - 1 );
    IVec2 tar = src + dir;

    if ( !mgw::Board::get().isTraversable(
            static_cast<unsigned int>( tar.x ),
            static_cast<unsigned int>( tar.y ) ) )
    {
        return d_piece->getCurrentSpace();
    }

    return tar;
}

gel::math::IVec2 PacmanController::getInputDir() const
{
    using namespace gel::math;
    using namespace sgdi;
    IVec2 dir;

    Input& in = Input::inst();
    if ( in.isDown( Input::KEY_W ) || in.isDown( Input::KEY_UP ) )
    {
        dir.y -= 1;
    }
    if ( in.isDown( Input::KEY_S ) || in.isDown( Input::KEY_DOWN ) )
    {
        dir.y += 1;
    }
    if ( in.isDown( Input::KEY_A ) || in.isDown( Input::KEY_LEFT ) )
    {
        dir.x -= 1;
    }
    if ( in.isDown( Input::KEY_D ) || in.isDown( Input::KEY_RIGHT ) )
    {
        dir.x += 1;
    }

    return dir;
}

} // End nspc mgc

} // End nspc PacmanController