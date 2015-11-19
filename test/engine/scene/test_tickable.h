// test_tickable.h
#ifndef INCLUDED_TEST_TICKABLE
#define INCLUDED_TEST_TICKABLE

#include <engine/scene/itickable.h>
#include <iostream>

namespace StevensDev
{

namespace sgdt
{

class TestTickable : public sgds::ITickable
{
  private:
    bool d_wasPreTicked;
      // If it was pre-ticked.

    bool d_wasTicked;
      // If it was post-ticked.

    bool d_wasPostTicked;
      // If it was post-ticked.

  public:
    // CONSTRUCTORS
    TestTickable();
      // Constructs a new test tickable.

    TestTickable( const TestTickable& tickable );
      // Constructs a copy of the tickable.

    ~TestTickable();
      // Destructs the tickable.

    // MEMBER FUNCTIONS
    virtual void preTick();
      // Sets the pre-ticked flag.

    virtual void tick( float dtS );
      // Sets the ticked flag.

    virtual void postTick();
      // Sets the post-ticked flag.

    bool wasPreTicked() const;
      // Checks if the item was pre-ticked.

    bool wasTicked() const;
      // Checks if the item was ticked.

    bool wasPostTicked() const;
      // Checks if the item was post-ticked.

    void reset();
      // Resets the ticked flags.
};

// FREE OPERATORS
static
std::ostream& operator<<( std::ostream& stream, const TestTickable& collider )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
TestTickable::TestTickable() : d_wasPreTicked( false ), d_wasTicked( false ),
                               d_wasPostTicked( false )
{
}

inline
TestTickable::TestTickable( const TestTickable& tickable )
    : d_wasPreTicked( tickable.d_wasPreTicked ),
      d_wasTicked( tickable.d_wasTicked ),
      d_wasPostTicked( tickable.d_wasPostTicked )
{
}

inline
TestTickable::~TestTickable()
{
}

// MEMBER FUNCTIONS
inline
void TestTickable::preTick()
{
    d_wasPreTicked = true;
}

inline
void TestTickable::tick( float dtS )
{
    d_wasTicked = true;
}

inline
void TestTickable::postTick()
{
    d_wasPostTicked = true;
}

inline
bool TestTickable::wasPreTicked() const
{
    return d_wasPreTicked;
}

inline
bool TestTickable::wasTicked() const
{
    return d_wasTicked;
}

inline
bool TestTickable::wasPostTicked() const
{
    return d_wasPostTicked;
}

inline
void TestTickable::reset()
{
    d_wasPreTicked = false;
    d_wasTicked = false;
    d_wasPostTicked = false;
}

} // End nspc sgdt

} // End nspc StevensDev

#endif
