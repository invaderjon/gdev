// icontroller.h
#ifndef INCLUDED_TEST_INPUT_CONTROLLER
#define INCLUDED_TEST_INPUT_CONTROLLER
#include "../rendering/renderable_sprite.h"
#include "itickable.h"

namespace StevensDev
{

namespace sgds
{

// This controller provides some simple reactions to user input.
class TestInputController : public ITickable
{
  private:
    sgdr::RenderableSprite* d_sprite;
      // The sprite that is being controlled.

  public:
    // CONSTRUCTORS
    TestInputController();
      // Sets up a new test input controller.

    TestInputController( sgdr::RenderableSprite* sprite );
      // Sets up a new test input controller using the given sprite.

    TestInputController( const TestInputController& other );
      // Creates a copy of another controller.

    ~TestInputController();

    // OPERATORS
    TestInputController& operator=( const TestInputController& other );

    // MEMBER FUNCTIONS
    virtual void preTick();
      // Prepares for the next tick cycle.

    virtual void tick( float dtS );
      // Updates the controller.

    virtual void postTick();
      // Cleans up after the tick cycle.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const TestInputController& controller )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
TestInputController::TestInputController() : d_sprite( nullptr )
{
}

inline
TestInputController::TestInputController( sgdr::RenderableSprite* sprite )
    : d_sprite( sprite )
{
}

inline
TestInputController::TestInputController( const TestInputController& other )
    : d_sprite( other.d_sprite )
{
}

inline
TestInputController::~TestInputController()
{
    d_sprite = nullptr;
}

// OPERATORS
inline
TestInputController& TestInputController::operator=(
    const TestInputController& other )
{
    d_sprite = other.d_sprite;
    return *this;
}

// MEMBER FUNCTIONS
inline
void TestInputController::preTick()
{
}

inline
void TestInputController::postTick()
{
}

} // End nspc sgds

} // End nspc StevensDev

#endif
