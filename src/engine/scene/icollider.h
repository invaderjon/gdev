// icollider.h
#ifndef INCLUDED_ICOLLIDER
#define INCLUDED_ICOLLIDER

#include "rectangle_bounds.h"

namespace StevensDev
{

namespace sgds
{

class ICollider
{
  public:
    // CONSTRUCTORS
    virtual ~ICollider() = 0;
      // Destructs the collider.

    // ACCESSOR FUNCTIONS
    virtual const RectangleBounds& bounds() const = 0;
      // Gets the collider bounds.

    virtual unsigned short flags() const = 0;
      // Gets the collision flags.

    // MUTATOR FUNCTIONS
    virtual void setFlags( unsigned short flags ) = 0;
      // Sets the collision flags.

    // MEMBER FUNCTIONS
    virtual bool canCollide( unsigned short flags ) const = 0;
      // Checks if the item can collide with the given flags.

    virtual bool doesCollide( const RectangleBounds& candidate ) const = 0;
      // Checks if the item does collide with the given bounds.
};

inline
ICollider::~ICollider()
{
}

} // End nspc sgds

} // End nspc StevensDev

#endif
