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

typedef unsigned int EventHandle;
  // Defines a listener unique handle that is used to identify registered
  // callbacks.

typedef std::function<void( const IEvent* )> EventListener;
  // Defines a listener callback..

class EventDispatcher : public sgds::ITickable
{
  private:
    struct RegisteredListener
    {
        EventHandle handle;
        EventListener listener;
    };

    // TYPES
    typedef std::pair<std::string, RegisteredListener> PendingAddition;
      // Defines an item that is pending for addition.

    typedef std::pair<std::string, EventHandle> PendingRemoval;

    // MEMBERS
    sgdc::Map<sgdc::DynamicArray<RegisteredListener>> d_registered;
      // A map of the registered event listeners to their event types.

    sgdc::DynamicArray<PendingAddition> d_pendingAddition;
      // List of listeners to register during post-tick cycle.

    sgdc::DynamicArray<PendingRemoval> d_pendingRemoval;
      // List of listeners to remove during post-tick cycle.

    EventHandle d_nextHandle;

    // HELPER FUNCTIONS
    int findListener( const std::string& item, EventHandle handle );
      // Gets the index of the listener with the specified handle.

    int findListener( const sgdc::DynamicArray<RegisteredListener>& listeners,
                      EventHandle handle, unsigned int start,
                      unsigned int end );
      // Find the listener with the given handle using a binary search.
      // A binary search can be used because items are always inserted
      // chronologically from when they are created so ID's are always in
      // ascending order.

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
    EventHandle add( const std::string& type,
                     const EventListener& listener );
      // Registers a listener to for the specified event type and
      // returns a handle that can be later used to unregister the
      // listener.

    void remove( const std::string& type, EventHandle handle );
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
                                     d_pendingRemoval(), d_nextHandle( 0 )
{
}

inline
EventDispatcher::EventDispatcher( const EventDispatcher& other )
    : d_registered( other.d_registered ),
      d_pendingAddition( other.d_pendingAddition ),
      d_pendingRemoval( other.d_pendingRemoval ),
      d_nextHandle( other.d_nextHandle )
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
    d_nextHandle = other.d_nextHandle;

    return *this;
}

// MEMBER FUNCTIONS
inline
EventHandle EventDispatcher::add( const std::string& type,
                                  const EventListener& listener )
{
    EventHandle handle = d_nextHandle++;

    RegisteredListener registered;
    registered.listener = std::move( listener );
    registered.handle = handle;

    d_pendingAddition.push( std::make_pair( type, std::move( registered ) ) );

    return handle;
}

inline
void EventDispatcher::remove( const std::string& type,
                              EventHandle handle )
{
    d_pendingRemoval.push( std::make_pair( type, handle ) );
}

} // End nspc sgde

} // End nspc StevensDev

#endif
