// collision.h
#ifndef INCLUDED_COLLISION
#define INCLUDED_COLLISION

namespace StevensDev
{

namespace sgds
{

struct Collision
{
    typedef unsigned short Flags;
      // Defines a collision flag or set of flags.
      //
      // Defined as a type so the size can easily be increased as needed.

    static const Flags NO_COLLISION = 0x8000;
      // No collision flag.

    static bool canCollide( Flags a, Flags b );
      // Checks if two sets of collision flags can collide with each other.

};

} // End nspc sgds

} // End nspc StevensDev

#endif
