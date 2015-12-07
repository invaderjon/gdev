// ghost_controller.cpp
#include "ghost_controller.h"

namespace StevensDev
{

namespace mgc
{

namespace
{

static const gel::math::IVec2 LEFT = gel::math::IVec2( -1, 0 );
static const gel::math::IVec2 UP = gel::math::IVec2( 0, -1 );
static const gel::math::IVec2 RIGHT = gel::math::IVec2( 1, 0 );
static const gel::math::IVec2 DOWN = gel::math::IVec2( 0, 1 );
static const gel::math::IVec2 DIRS[4] = { LEFT, UP, RIGHT, DOWN };

} // End nspc anonymous

// MEMBER FUNCTIONS
void GhostController::preTick()
{
    using namespace gel::math;

    unsigned int x;
    unsigned int y;
    IVec2 prev;
    if ( d_ghost->getTargetCount() <= 1 )
    {
        x = 0;
        y = 0;
        while ( !mgw::Board::get().isTraversable( x, y ) )
        {
            x = std::rand() % mgw::Board::get().height();
            y = std::rand() % mgw::Board::get().height();
        }

        d_ghost->addTarget( IVec2( x, y ) );//navigateTo( IVec2( x, y ) );
    }
}

// HELPER FUNCTIONS
void GhostController::navigateTo( const gel::math::IVec2& tar )
{
    using namespace gel::math;
    const mgw::Board& board = mgw::Board::get();
    IVec2 cur = d_ghost->getTargetSpace( d_ghost->getTargetCount() - 1 );
    IVec2 next = cur;
    IVec2 dir;
    unsigned int i = 0;
    bool isGood;

    while ( tar != next && i != 4 )
    {
        cur = d_ghost->getTargetSpace( d_ghost->getTargetCount() - 1 );

        for ( i = 0, isGood = false; !isGood && i < 4; ++i )
        {
            dir = DIRS[i];
            next = cur + dir;
            if ( !d_ghost->hasTarget( next ) &&
                 board.isTraversable(
                    static_cast<unsigned int>( next.x ),
                    static_cast<unsigned int>( next.y ) ) &&
                    ( abs( tar.x - next.x ) < abs( tar.x - cur.x ) ||
                        abs( tar.y - next.y ) < abs( tar.y - cur.y ) ) )
            {
                next = cur + dir;
                isGood = true;
            }
        }

        d_ghost->addTarget( next );
    }
}

} // End nspc mgc

} // End nspc StevensDev