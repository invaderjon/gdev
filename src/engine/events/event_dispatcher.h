// event_dispatcher.h
#ifndef INCLUDED_EVENT_DISPATCHER
#define INCLUDED_EVENT_DISPATCHER

#include "../containers/dynamic_array.h"
#include "../containers/map.h"
#include "../scene/itickable.h"
#include <functional>
#include "ievent.h"
#include <string>

namespace StevensDev
{

namespace sgde
{

class EventDispatcher : public sgds::ITickable
{
  private:
    // TYPES
    typedef std::function<void( const IEvent* )> Listener;
      // Defines a listener callback (for convenience).

    typedef std::pair<std::string, const Listener*> Pending;
      // Defines an item that is pending for addition or removal.

    // MEMBERS
    sgdc::Map<sgdc::DynamicArray<const Listener*>> d_registered;
      // A map of the registered event listeners to their event types.

    sgdc::DynamicArray<Pending> d_pendingAddition;
      // List of listeners to register during post-tick cycle.

    sgdc::DynamicArray<Pending> d_pendingRemoval;
      // List of listeners to remove during post-tick cycle.

  public:
    // CONSTRUCTORS
    EventDispatcher();
      // Constructs a new event dispatcher.

    EventDispatcher( const EventDispatcher& dispatcher );
      // Constructs a copy of the given event dispatcher.

    ~EventDispatcher();
      // Destructs the event dispatcher.

    // OPERATORS
    EventDispatcher& operator=( const EventDispatcher& dispatcher );

    // MEMBER FUNCTIONS
    void add( const std::string& type,
              const std::function<void( const IEvent* )>* listener );
      // Registers a listener to for the specified event type.

    void remove( const std::string& type,
                 const std::function<void( const IEvent* )>* listener );
      // Stops broadcasting events to the specified listener for the
      // specified event type.

    void dispatch( const IEvent* event );
      // Dispatches an event to relevant registered listeners.

    virtual void preTick();
      // Does nothing.

    virtual void tick( float dtS );
      // Does nothing.

    virtual void postTick();
      // Performs additions and removals.
};

// FREE OPERATORS
static
std::ostream& operator<<( std::ostream& stream,
                          const EventDispatcher& dispatcher )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
EventDispatcher::EventDispatcher() : d_registered(), d_pendingAddition(),
                                     d_pendingRemoval()
{
}

inline
EventDispatcher::EventDispatcher( const EventDispatcher& other )
    : d_registered( other.d_registered ),
      d_pendingAddition( other.d_pendingAddition ),
      d_pendingRemoval( other.d_pendingRemoval )
{
}

inline
EventDispatcher::~EventDispatcher()
{
}

// OPERATORS
inline
EventDispatcher& EventDispatcher::operator=( const EventDispatcher& other )
{
    d_registered = other.d_registered;
    d_pendingAddition = other.d_pendingAddition;
    d_pendingRemoval = other.d_pendingRemoval;

    return *this;
}

// MEMBER FUNCTIONS
inline
void EventDispatcher::add( const std::string& type, const Listener* listener )
{
    assert( listener != nullptr );
    d_pendingAddition.push( std::make_pair( type, listener ) );
}

inline
void EventDispatcher::remove( const std::string& type,
                              const Listener* listener )
{
    assert( listener != nullptr );
    d_pendingRemoval.push( std::make_pair( type, listener ) );
}

} // End nspc sgde

} // End nspc StevensDev

#endif
