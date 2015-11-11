// test_listener.h
#ifndef GDEV_TEST_LISTENER_H
#define GDEV_TEST_LISTENER_H

#include <engine/events/ievent.h>
#include <engine/data/json_printer.h>

namespace StevensDev
{

namespace sgdt
{

class TestListener
{
  private:
    std::string d_type;
    // The type of the received event.

    bool d_wasCalled;
      // If the callback was called.

  public:
    TestListener();
      // Constructs a new test listener.

    TestListener( const TestListener& listener );
      // Constructs a copy of another test listener.

    ~TestListener();
      // Destructs the test listener.

    // ACCESSOR FUNCTIONS
    const std::string& receivedType() const;
      // Get the type of the received event.

    bool wasCalled() const;
    // Checks if the callback was invoked.

    void receive( const sgde::IEvent* event );
    // Receives the event and sets the called flag.

    void reset();
      // Resets the was called flag.
};

// FREE OPERATORS
static
std::ostream& operator<<( std::ostream& stream,
                          const TestListener& listener )
{
    sgdd::JsonPrinter p( stream );
    p.open()
        .print( "wasCalled", listener.wasCalled() )
        .print( "receivedType", listener.receivedType() )
        .close();

    return stream;
}

// CONSTRUCTORS
inline
TestListener::TestListener() : d_type(), d_wasCalled( false )
{
}

inline
TestListener::TestListener( const TestListener& listener )
    : d_type( listener.d_type ), d_wasCalled( listener.d_wasCalled )
{
}

inline
TestListener::~TestListener()
{
}

// ACCESSOR FUNCTIONS
inline
bool TestListener::wasCalled() const
{
    return d_wasCalled;
}

inline
const std::string& TestListener::receivedType() const
{
    return d_type;
}

// MEMBER FUNCTIONS
inline
void TestListener::receive( const sgde::IEvent* event )
{
    d_wasCalled = true;
    d_type = event->type();
}

inline
void TestListener::reset()
{
    d_type = "";
    d_wasCalled = false;
}

} // End nspc sgdt

} // End nspc StevensDev

#endif //GDEV_TEST_LISTENER_H
