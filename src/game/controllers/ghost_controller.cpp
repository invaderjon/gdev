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

inline
sgdu::HashCode hash( const gel::math::IVec2& value )
{
    using namespace sgdu;
    return ( ( ( HashUtils::FNV_OFFSET_32 ^ value.x ) *
            HashUtils::FNV_PRIME_32 ) ^ value.y ) * HashUtils::FNV_PRIME_32;
}


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
            x = std::rand() % mgw::Board::get().width();
            y = std::rand() % mgw::Board::get().height();
        }

        navigateTo( IVec2( x, y ) );
        //d_ghost->addTarget( IVec2( x, y ) );//navigateTo( IVec2( x, y ) );
    }
}

// HELPER FUNCTIONS
void GhostController::navigateTo( const gel::math::IVec2& tar )
{
    using namespace gel::math;
    using namespace StevensDev::sgdc;
    const mgw::Board& board = mgw::Board::get();

    Set<IVec2> closed( &hash );
    Set<IVec2> open( &hash );
    Map<IVec2, IVec2> cameFrom( &hash );
    Map<float, IVec2> gScores;
    Map<float, IVec2> fScores;

    IVec2 cur = ( d_ghost -> getTargetCount() > 0 ) ?
                d_ghost->getTargetSpace( d_ghost->getTargetCount() - 1 ) :
                d_ghost->getCurrentSpace();

    open.add( cur );
    gScores[cur] = 0.0f;
    fScores[cur] = getManhattanDist( cur, tar );

    IVec2 neigh = cur;
    IVec2 dir;
    unsigned i;
    float tentativeGScore;
    bool wasSuccessful = false;
    while ( !open.isEmpty() )
    {
        cur = getNextBest( open, fScores );
        if ( cur == tar )
        {
            wasSuccessful = true;
            break;
        }

        open.remove( cur );
        closed.add( cur );
        for ( i = 0; i < 4; ++i )
        {
            neigh = cur + DIRS[i];

            // check that it hasn't been checked and is valid
            if ( closed.has( neigh ) ||
                 !board.isTraversable(
                         static_cast<unsigned int>( neigh.x ),
                         static_cast<unsigned int>( neigh.y ) ) )
            {
                continue;
            }

            tentativeGScore =
                    ( gScores.has( cur ) ) ?
                    gScores[cur] + Vec::length( Vec2( cur - neigh ) ) :
                    std::numeric_limits<float>::infinity();

            // add it to open or skip it if it's not better
            if ( !open.has( neigh ) )
            {
                open.add( neigh );
            }
            else if ( tentativeGScore >= gScores[neigh] )
            {
                continue;
            }

            cameFrom[neigh] = cur;
            gScores[neigh] = tentativeGScore;
            fScores[neigh] = gScores[neigh] + getManhattanDist( neigh, tar );
        }
    }

    if ( !wasSuccessful )
    {
        return;
    }

    DynamicArray<IVec2> path( cameFrom.keys().size() );
    cur = tar;
    path.push( cur );
    while ( cameFrom.has( cur ) )
    {
        cur = cameFrom[cur];
        path.pushFront( cur );
    }

    while ( path.size() > 0 )
    {
        d_ghost->addTarget( path[0] );
        path.popFront();
    }
}

float GhostController::getManhattanDist(
        const gel::math::IVec2& source, const gel::math::IVec2& target ) const
{
    using namespace gel::math;
    float dx = Math::abs( source.x - target.x );
    float dy = Math::abs( source.y - target.y );
    return dx * dy;
}

gel::math::IVec2 GhostController::getNextBest(
        const sgdc::Set<gel::math::IVec2>& items,
        const sgdc::Map<float, gel::math::IVec2>& fScores )
{
    using namespace gel::math;

    if ( items.size() <= 0 )
    {
        return gel::math::IVec2();
    }

    IVec2 best;
    float minScore;
    unsigned int i;
    for ( i = 1, best = items[0], minScore = fScores[best];
          i < items.size(); ++i )
    {
        if ( fScores[items[i]] < minScore )
        {
            best = items[i];
            minScore = fScores[best];
        }
    }

    return best;
}

} // End nspc mgc

} // End nspc StevensDev