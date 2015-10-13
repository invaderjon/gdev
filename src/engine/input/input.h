// input.h
#ifndef INCLUDED_INPUT
#define INCLUDED_INPUT
#include "../memory/allocator_guard.h"
#include "../memory/mem.h"
#include "../scene/itickable.h"
#include <string>

namespace StevensDev
{

namespace sgdi
{

class Input : public sgds::ITickable
{
  private:
    struct InputState
    {
        bool isDown;
          // If it is currently down.

        bool wasDown;
          // If it was released.
    };

    // GLOBALS
    static Input d_instance;
      // The singleton instance.

    // MEMBERS
    sgdm::AllocatorGuard<InputState> d_alloc;
      // The state allocator.

    InputState* d_states;
      // Set of keyboard key states.
      // Every input is stored separately in this array so that lookup time
      // is equal to O(1). Furthermore, the state information is just 2 bits
      // (assuming bool is compressed to 1 bit).

    // CONSTRUCTORS
    Input();
      // Constructs a new input manager.

    Input( const Input& input );
      // Constructs a copy of an input manager.

    Input& operator=( const Input& input );
      // Makes this a copy of another input manager.

  public:
    enum InputType
    {
        // KEYBOARD
        KEY_A = 0,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,
        KEY_0,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_ESCAPE,
        KEY_CNTRL_LEFT,
        KEY_SHIFT_LEFT,
        KEY_ALT_LEFT,
        KEY_SUPER_LEFT,
        KEY_CNTRL_RIGHT,
        KEY_SHIFT_RIGHT,
        KEY_ALT_RIGHT,
        KEY_SUPER_RIGHT,
        KEY_MENU,
        KEY_LEFT_RACKET,
        KEY_RIGHT_BRACKET,
        KEY_SEMICOLON,
        KEY_COMMA,
        KEY_PERIOD,
        KEY_QUOTE,
        KEY_SLASH,
        KEY_BACKSLASH,
        KEY_TILDE,
        KEY_EQUAL,
        KEY_DASH,
        KEY_SPACE,
        KEY_ENTER,
        KEY_BACKSPACE,
        KEY_TAB,
        KEY_PAGE_UP,
        KEY_PAGE_DOWN,
        KEY_END,
        KEY_HOME,
        KEY_INSERT,
        KEY_DELETE,
        KEY_NUMPAD_ADD,
        KEY_NUMPAD_SUBTRACT,
        KEY_NUMPAD_MULTIPLY,
        KEY_NUMPAD_DIVIDE,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN,
        KEY_NUMPAD_0,
        KEY_NUMPAD_1,
        KEY_NUMPAD_2,
        KEY_NUMPAD_3,
        KEY_NUMPAD_4,
        KEY_NUMPAD_5,
        KEY_NUMPAD_6,
        KEY_NUMPAD_7,
        KEY_NUMPAD_8,
        KEY_NUMPAD_9,
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,
        KEY_F13,
        KEY_F14,
        KEY_F15,
        KEY_PAUSE,

        // MOUSE
        MOUSE_LEFT_BUTTON,
        MOUSE_RIGHT_BUTTON,
        MOUSE_MIDDLE_BUTTON,
        MOUSE_XBUTTON1,
        MOUSE_XBUTTON2,

        // GAMEPAD
        GAMEPAD_BUTTON_1,
        GAMEPAD_BUTTON_2,
        GAMEPAD_BUTTON_3,
        GAMEPAD_BUTTON_4,
        GAMEPAD_BUTTON_5,
        GAMEPAD_BUTTON_6,
        GAMEPAD_BUTTON_7,
        GAMEPAD_BUTTON_8,
        GAMEPAD_BUTTON_9,
        GAMEPAD_BUTTON_10,
        GAMEPAD_BUTTON_11,
        GAMEPAD_BUTTON_12,
        GAMEPAD_BUTTON_13,
        GAMEPAD_BUTTON_14,
        GAMEPAD_BUTTON_15,
        GAMEPAD_BUTTON_16,
        GAMEPAD_BUTTON_17,
        GAMEPAD_BUTTON_18,
        GAMEPAD_BUTTON_19,
        GAMEPAD_BUTTON_20,
        GAMEPAD_BUTTON_21,
        GAMEPAD_BUTTON_22,
        GAMEPAD_BUTTON_23,
        GAMEPAD_BUTTON_24,
        GAMEPAD_BUTTON_25,
        GAMEPAD_BUTTON_26,
        GAMEPAD_BUTTON_27,
        GAMEPAD_BUTTON_28,
        GAMEPAD_BUTTON_29,
        GAMEPAD_BUTTON_30,
        GAMEPAD_BUTTON_31,
        GAMEPAD_BUTTON_32,

        // NUMBER OF AVAILABLE INPUTS
        INPUT_TYPE_COUNT
    };

    ~Input();
      // Destructs the input manager.

    // MEMBER FUNCTIONS
    bool isDown( InputType type );
      // Checks if a button is down.

    bool isUp( InputType type );
      // Checks if a button is up.

    bool wasPressed( InputType type );
      // Checks if a button was down then up.

    virtual void preTick();
      // Polls input state and stores it internally.

    virtual void tick( float dtS );
      // Does nothing.

    virtual void postTick();
      // Does nothing.

    static Input& inst();
      // Gets an instance of the input manager.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream, const Input& input )
{
    return stream << std::string( "{  }" );
}

// CONSTRUCTORS
inline
Input::Input() : d_alloc()
{
    // clear input state
    InputState nullState;
    nullState.isDown = false;
    nullState.wasDown = false;

    d_states = d_alloc.get( INPUT_TYPE_COUNT );
    sgdm::Mem::set( d_states, nullState, INPUT_TYPE_COUNT );
}

inline
Input::Input( const Input& input )
{
    d_states = d_alloc.allocator()->get( INPUT_TYPE_COUNT );
    sgdm::Mem::copy( d_states, input.d_states, INPUT_TYPE_COUNT );
}

inline
Input& Input::operator=( const Input& input )
{
    sgdm::Mem::copy( d_states, input.d_states, INPUT_TYPE_COUNT );
    return *this;
}

inline
Input::~Input()
{
    d_alloc.release( d_states, INPUT_TYPE_COUNT );
}

// GLOBAL FUNCTIONS
inline
Input& Input::inst()
{
    return d_instance;
}

} // End nspc sgdi

} // End nspc StevensDev

#endif
