// collision_event.h
#ifndef INCLUDED_COLLISION_EVENT
#define INCLUDED_COLLISION_EVENT

#include <engine/events/ievent.h>
#include <engine/scene/icollider.h>

namespace StevensDev
{

namespace mge
{

class CollisionEvent : public sgde::IEvent
{
  private:
    // MEMBERS
    std::string d_firstName;
      // The name of the first collider.

    std::string d_secondName;
      // The name of the second collider.

    unsigned int d_firstId;
      // The id of the first collider.

    unsigned int d_secondId;
      // The id of the second collider.

  public:
    // CONSTRUCTORS
    CollisionEvent();
      // Constructs an empty event.

    CollisionEvent( const std::string& firstName, unsigned int firstId,
                    const std::string& secondName, unsigned int secondId );
      // Constructs a new collision event for the given items.

    CollisionEvent( const CollisionEvent& event );
      // Constructs a copy of the given event.

    ~CollisionEvent();
      // Destructs the event.

    // ACCESSOR FUNCTIONS
    const std::string& firstName() const;
      // Gets the first collider's name.

    const std::string& secondName() const;
      // Gets the second collider's name.

    unsigned int firstId() const;
      // Gets the first collider's id.

    unsigned int secondId() const;
      // Gets the second collider's id.
};

// CONSTRUCTORS
inline
CollisionEvent::CollisionEvent()
{
}

inline
CollisionEvent::CollisionEvent(
    const std::string& firstName, unsigned int firstId,
    const std::string& secondName, unsigned int secondId )
    : d_firstName( firstName ), d_secondName( secondName ),
      d_firstId( firstId ), d_secondId( secondId )
{
}

inline
CollisionEvent::CollisionEvent( const CollisionEvent& event )
    : d_firstName( event.d_firstName ), d_secondName( event.d_secondName ),
      d_firstId( event.d_firstId ), d_secondId( event.d_secondId )
{
}

inline
CollisionEvent::~CollisionEvent()
{
}

// ACCESSOR FUNCTIONS
inline
const std::string& CollisionEvent::firstName() const
{
    return d_firstName;
}

inline
const std::string& CollisionEvent::secondName() const
{
    return d_secondName;
}

inline
unsigned int CollisionEvent::firstId() const
{
    return d_firstId;
}

inline
unsigned int CollisionEvent::secondId() const
{
    return d_secondId;
}

} // End nspc mge

} // End nspc StevensDev

#endif
