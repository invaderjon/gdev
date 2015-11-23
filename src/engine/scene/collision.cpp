// collision.cpp
#include "collision.h"

namespace StevensDev
{

namespace sgds
{

bool Collision::canCollide( Flags a, Flags b )
{
    return !( ( a ^ b ) & NO_COLLISION );
}

} // End nspc sgds

} // End nspc StevensDev