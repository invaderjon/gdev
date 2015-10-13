// itickable.h
#ifndef INCLUDED_ITICKABLE
#define INCLUDED_ITICKABLE

namespace StevensDev
{

namespace sgds
{

class ITickable
{
  public:
    // CONSTRUCTORS
    virtual ~ITickable() = 0;
      // Destructs the tickable.

    // MEMBER FUNCTIONS
    virtual void preTick() = 0;
      // Prepares for the next tick cycle.

    virtual void tick( float dtS ) = 0;
      // Executes the tick cycle using a given delta time.

    virtual void postTick() = 0;
      // Cleans up after the tick cycle.
};

inline
ITickable::~ITickable()
{
}

} // End nspc sgds

} // End nspc StevensDev

#endif
