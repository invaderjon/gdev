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

} // End nspc mgo

} // End nspc StevensDev