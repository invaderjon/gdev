// ievent.h
#ifndef INCLUDED_IEVENT
#define INCLUDED_IEVENT

#include <string>

namespace StevensDev
{

namespace sgde
{

class IEvent
{
  public:
    // CONSTRUCTORS
    virtual ~IEvent() = 0;
      // Destructs the event.

    // ACCESSOR FUNCTIONS
    virtual const std::string& type() const = 0;
      // Gets the event type.
};

// CONSTRUCTORS
inline
IEvent::~IEvent()
{
}

} // End nspc sgde

} // End nspc StevensDev

#endif
