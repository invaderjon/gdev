// event_bus.h
#ifndef INCLUDED_EVENT_BUS
#define INCLUDED_EVENT_BUS

#include "event_dispatcher.h"
#include "ievent.h"

namespace StevensDev
{

namespace sgde
{

class EventBus
{
  private:
    // GLOBALS
    static EventDispatcher d_instance;
      // The singleton instance of the dispatcher.

    // CONSTRUCTORS
    EventBus();
      // Constructs a new event bus.

    EventBus( const EventBus& other );
      // Constructs a copy of an event bus.

    // OPERATORS
    EventBus& operator=( const EventBus& other );
      // Makes this a copy of another event bus.

  public:
    // CONSTRUCTORS
    ~EventBus();
      // Deconstructs the event bus.

    // GLOBAL FUNCTIONS
    static EventDispatcher& dispatcher();
      // Gets the event dispatcher acting as the event bus.
};

// FREE OPERATORS
static
std::ostream& operator<<( std::ostream& stream, const EventBus& bus )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
EventBus::EventBus()
{
}

inline
EventBus::EventBus( const EventBus& bus )
{
    // does nothing
}

inline
EventBus::~EventBus()
{
}

// OPERATORS
inline
EventBus& EventBus::operator=( const EventBus& other )
{
    // does nothing
    return *this;
}

// GLOBAL FUNCTIONS
inline
EventDispatcher& EventBus::dispatcher()
{
    return d_instance;
}

//inline
//void EventBus::add( const std::string& type,
//                    const std::function<void( const IEvent* )>* listener )
//{
//    d_instance.add( type, listener );
//}
//
//inline
//void EventBus::remove( const std::string& type,
//                       const std::function<void( const IEvent* )>* listener )
//{
//    d_instance.remove( type, listener );
//}
//
//inline
//void EventBus::dispatch( const IEvent* event )
//{
//    d_instance.dispatch( event );
//}

} // End nspc StevensDev

} // End nspc sgde


#endif
