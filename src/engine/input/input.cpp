// input.cpp
#include "input.h"
#include <SFML/Window.hpp>

namespace StevensDev
{

namespace sgdi
{

namespace
{

// HELPER FUNCTIONS
Input::InputType getInputType( sf::Keyboard::Key key )
{
    return static_cast<Input::InputType>( key );
}

Input::InputType getInputType( sf::Mouse::Button button )
{
    return static_cast<Input::InputType>(
        static_cast<int>( button ) +
        static_cast<int>( sf::Keyboard::KeyCount ) );
}

Input::InputType getInputType( unsigned int button )
{
    return static_cast<Input::InputType>(
        static_cast<int>( button ) +
        static_cast<int>( sf::Keyboard::KeyCount ) +
        static_cast<int>( sf::Mouse::ButtonCount ) );
}

} // End nspc anonymous

// GLOBALS
Input Input::d_instance = Input();

// MEMBER FUNCTIONS
bool Input::isDown( Input::InputType type )
{
    return d_states[type].isDown;
}

bool Input::isUp( Input::InputType type )
{
    return !d_states[type].isDown;
}

bool Input::wasPressed( Input::InputType type )
{
    return d_states[type].wasDown;
}

void Input::preTick()
{
    int i;
    InputType type;

    // check keyboard keys
    sf::Keyboard::Key key;
    for ( i = 0, key = static_cast<sf::Keyboard::Key>( i );
          i < static_cast<int>( sf::Keyboard::KeyCount );
          ++i, key = static_cast<sf::Keyboard::Key>( i ) )
    {
        type = getInputType( key );
        d_states[type].wasDown = d_states[type].isDown;
        d_states[type].isDown = sf::Keyboard::isKeyPressed( key );
        d_states[type].wasDown &= !d_states[type].isDown;
    }

    // check mouse buttons
    sf::Mouse::Button mouse;
    for ( i = 0, mouse = static_cast<sf::Mouse::Button>( i );
          i < static_cast<int>( sf::Mouse::ButtonCount );
          ++i, mouse = static_cast<sf::Mouse::Button>( i ) )
    {
        type = getInputType( mouse );
        d_states[type].wasDown = d_states[type].isDown;
        d_states[type].isDown = sf::Mouse::isButtonPressed( mouse );
        d_states[type].wasDown &= !d_states[type].isDown;
    }

    // check game pad buttons (only one game pad is supported)
    if ( sf::Joystick::isConnected( 0 ) )
    {
        unsigned int gpButton;
        for ( gpButton = 0;
              gpButton < sf::Joystick::getButtonCount( 0 );
              ++gpButton )
        {
            type = getInputType( gpButton );
            d_states[type].wasDown = d_states[type].isDown;
            d_states[type].isDown =
                sf::Joystick::isButtonPressed( 0, gpButton );
            d_states[type].wasDown &= !d_states[type].isDown;
        }
    }
}

void Input::tick( float dtS )
{

}

void Input::postTick()
{
}

} // End nspc sgdi

} // End nspc StevensDev