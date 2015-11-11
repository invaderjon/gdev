// test_event.h
#ifndef INCLUDED_TEST_EVENT
#define INCLUDED_TEST_EVENT

#include <string>
#include <engine/data/json_printer.h>
#include "engine/events/ievent.h"

namespace StevensDev
{

namespace sgdt
{

class TestEvent : public sgde::IEvent
{
  private:
    std::string d_type;
      // The test event type.

  public:
    // CONSTANTS
    static const std::string DEFAULT_TYPE;

    // CONSTRUCTORS
    TestEvent();
      // Creates a test event of the default type.

    TestEvent( const std::string& type );
      // Creates a test event of the specified type.

    TestEvent( const TestEvent& event );
      // Creates a copy of the specified event.

    ~TestEvent();

    // OPERATORS
    TestEvent& operator=( const TestEvent& event );
      // Makes this a copy of another test event.

    // MEMBER FUNCTIONS
    virtual const std::string& type() const;
      // Gets the test event type.
};

// FREE OPERATORS
static
std::ostream& operator<<( std::ostream& stream, const TestEvent& event )
{
    sgdd::JsonPrinter p( stream );

    p.open().print( "type", event.type() ).close();

    return stream;
}

// CONSTRUCTORS
inline
TestEvent::TestEvent() : d_type( DEFAULT_TYPE )
{
}

inline
TestEvent::TestEvent( const std::string& type ) : d_type( type )
{
}

inline
TestEvent::TestEvent( const TestEvent& event ) : d_type( event.d_type )
{
}

inline
TestEvent::~TestEvent()
{
}

// OPERATORS
inline
TestEvent& TestEvent::operator=( const TestEvent& event )
{
    d_type = event.d_type;
    return *this;
}

// MEMBER FUNCTIONS
inline
const std::string& TestEvent::type() const
{
    return d_type;
}

} // End nspc sgdt

} // End nspc Stevens Dev

#endif