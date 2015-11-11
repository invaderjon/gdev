// test_input_controller.cpp
#include "test_input_controller.h"
#include <engine/input/input.h>

namespace StevensDev
{

namespace sgdt
{

namespace
{
    const float VELOCITY = 250.0f;

} // End nspc anonymous

void TestInputController::tick( float dtS )
{
    using namespace sgdi;

    Input& input = Input::inst();

    float sx = input.isDown( Input::KEY_LEFT ) ? -1 :
               input.isDown( Input::KEY_RIGHT ) ? 1 :
               0;

    float sy = input.isDown( Input::KEY_UP ) ? -1 :
               input.isDown( Input::KEY_DOWN ) ? 1 :
               0;

    d_sprite->move( dtS * VELOCITY * sx, dtS * VELOCITY * sy );
}

} // End nspc sgdt

} // End nspc StevensDev
