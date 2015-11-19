// test_collider.h
#ifndef INCLUDED_TEST_COLLIDER
#define INCLUDED_TEST_COLLIDER

#include <engine/scene/icollider.h>

namespace StevensDev
{

namespace sgdt
{

class TestCollider : public sgds::ICollider
{
  private:
    sgds::RectangleBounds d_bounds;
      // The collision bounds.

    unsigned short d_flags;
      // The collision flags.

  public:
    TestCollider();
      // Constructs a new test collider.

    TestCollider( const sgds::RectangleBounds& bounds );
      // Constructs a new collider with the given bounds.

    TestCollider( const sgds::RectangleBounds& bounds, unsigned short flags );
      // Constructs a new collider with the given bounds and flags.

    TestCollider( const TestCollider& collider );
      // Constructs a copy of a test collider.

    ~TestCollider();
      // Destructs the test collider.

    virtual const sgds::RectangleBounds& bounds() const;
      // Gets the collision bounds.

    virtual unsigned short flags() const;
      // Gets the collision flags.

    virtual void setFlags( unsigned short flags );
      // Sets the collider flags.

    virtual bool canCollide( unsigned short flags ) const;
      // Checks if this can collide with an object that has the given flags.

    virtual bool doesCollide( const sgds::RectangleBounds& candidate ) const;
      // Checks if this does collide with another set of bounds.
};

// FREE OPERATORS
static
std::ostream& operator<<( std::ostream& stream, const TestCollider& collider )
{
    sgdd::JsonPrinter p( stream );

    p.open().print( "bounds", collider.bounds() )
            .print( "flags", collider.flags() )
            .close();

    return stream;
}

// CONSTRUCTORS
inline
TestCollider::TestCollider() : d_bounds(), d_flags( 0 )
{
}

inline
TestCollider::TestCollider( const sgds::RectangleBounds& bounds )
    : d_bounds( bounds ), d_flags( 0 )
{
}

inline
TestCollider::TestCollider( const sgds::RectangleBounds& bounds,
                            unsigned short flags ) : d_bounds( bounds ),
                                                     d_flags( flags )
{
}

inline
TestCollider::TestCollider( const TestCollider& collider )
    : d_bounds( collider.d_bounds ), d_flags( collider.d_flags )
{
}

inline
TestCollider::~TestCollider()
{
}

// ACCESSOR FUNCTIONS
inline
const sgds::RectangleBounds& TestCollider::bounds() const
{
    return d_bounds;
}

inline
unsigned short TestCollider::flags() const
{
    return d_flags;
}

// MUTATOR FUNCTIONS
inline
void TestCollider::setFlags( unsigned short flags )
{
    d_flags = flags;
}

// MEMBER FUNCTIONS
inline
bool TestCollider::canCollide( unsigned short flags ) const
{
    // only if flags are the same
    return flags == ( flags & d_flags );
}

inline
bool TestCollider::doesCollide( const sgds::RectangleBounds& candidate ) const
{
    return d_bounds.doesCollide( candidate );
}

} // End nspc sgdt

} // End nspc StevensDev

#endif
