// piece.cpp
#include "piece.h"

namespace StevensDev
{

namespace mgo
{

void Piece::clearTargets()
{
    while ( d_motionPath.size() > 1 )
    {
        d_motionPath.pop();
    }
}

gel::math::Vec2 Piece::getWorldPositionForSpace(
    const gel::math::IVec2& space ) const
{
    const mgw::Board& board = mgw::Board::get();
    const sgds::RectangleBounds& obj = bounds();

    // center the sprite in the space
    float x = space.x * board.spaceWidth() +
        ( board.spaceWidth() - obj.width() ) / 2.0f + 10.0f;

    float y = space.y * board.spaceHeight() +
        ( board.spaceHeight() - obj.height() ) / 2.0f + 10.0f;

    return gel::math::Vec2( x, y );
}

gel::math::Vec2 Piece::getWorldPositionForSpace(
    const gel::math::IVec2& space, const mgw::Board& board ) const
{
    const sgds::RectangleBounds& obj = bounds();

    // center the sprite in the space
    float x = space.x * board.spaceWidth() +
        ( board.spaceWidth() - obj.width() ) / 2.0f + 10.0f;

    float y = space.y * board.spaceHeight() +
        ( board.spaceHeight() - obj.height() ) / 2.0f + 10.0f;

    return gel::math::Vec2( x, y );
}

} // End nspc mgo

} // End nspc StevensDev