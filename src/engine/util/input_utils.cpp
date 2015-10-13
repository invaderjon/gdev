// input_utils.cpp
#include "input_utils.h"
#include <sstream>

namespace StevensDev
{

namespace sgdu
{

std::string InputUtils::getInputTypeName( sgdi::Input::InputType type )
{
    using namespace sgdi;
    using sgdi::Input;
    std::ostringstream oss;
    std::string ret;

    switch ( type )
    {
        // KEYBOARD
        case Input::KEY_A:
        case Input::KEY_B:
        case Input::KEY_C:
        case Input::KEY_D:
        case Input::KEY_E:
        case Input::KEY_F:
        case Input::KEY_G:
        case Input::KEY_H:
        case Input::KEY_I:
        case Input::KEY_J:
        case Input::KEY_K:
        case Input::KEY_L:
        case Input::KEY_M:
        case Input::KEY_N:
        case Input::KEY_O:
        case Input::KEY_P:
        case Input::KEY_Q:
        case Input::KEY_R:
        case Input::KEY_S:
        case Input::KEY_T:
        case Input::KEY_U:
        case Input::KEY_V:
        case Input::KEY_W:
        case Input::KEY_X:
        case Input::KEY_Y:
        case Input::KEY_Z:
            oss << static_cast<char>(
                static_cast<int>( 'a' ) +
                static_cast<int>( type ) -
                static_cast<int>( Input::KEY_A ) );
            ret = oss.str();
            break;

        case Input::KEY_0:
        case Input::KEY_1:
        case Input::KEY_2:
        case Input::KEY_3:
        case Input::KEY_4:
        case Input::KEY_5:
        case Input::KEY_6:
        case Input::KEY_7:
        case Input::KEY_8:
        case Input::KEY_9:
            oss << static_cast<char>(
                static_cast<int>( '0' ) +
                static_cast<int>( type ) -
                static_cast<int>( Input::KEY_0 ) );
            ret = oss.str();
            break;

        case Input::KEY_ESCAPE:
            ret = "escape";
            break;

        case Input::KEY_CNTRL_LEFT:
            ret = "left control";
            break;

        case Input::KEY_SHIFT_LEFT:
            ret = "left shift";
            break;

        case Input::KEY_ALT_LEFT:
            ret = "left alt";
            break;

        case Input::KEY_SUPER_LEFT:
            ret = "left super";
            break;

        case Input::KEY_CNTRL_RIGHT:
            ret = "right control";
            break;

        case Input::KEY_SHIFT_RIGHT:
            ret = "right shift";
            break;

        case Input::KEY_ALT_RIGHT:
            ret = "right alt";
            break;

        case Input::KEY_SUPER_RIGHT:
            ret = "right system";
            break;

        case Input::KEY_MENU:
            ret = "menu";
            break;

        case Input::KEY_LEFT_RACKET:
            ret = "left bracket";
            break;

        case Input::KEY_RIGHT_BRACKET:
            ret = "right bracket";
            break;

        case Input::KEY_SEMICOLON:
            ret = "semicolon";
            break;

        case Input::KEY_COMMA:
            ret = "comma";
            break;

        case Input::KEY_PERIOD:
            ret = "period";
            break;

        case Input::KEY_QUOTE:
            ret = "quote";
            break;

        case Input::KEY_SLASH:
            ret = "slash";
            break;

        case Input::KEY_BACKSLASH:
            ret = "backslash";
            break;

        case Input::KEY_TILDE:
            ret = "tilde";
            break;

        case Input::KEY_EQUAL:
            ret = "equal";
            break;

        case Input::KEY_DASH:
            ret = "dash";
            break;

        case Input::KEY_SPACE:
            ret = "space";
            break;

        case Input::KEY_ENTER:
            ret = "enter";
            break;

        case Input::KEY_BACKSPACE:
            ret = "backspace";
            break;

        case Input::KEY_TAB:
            ret = "tab";
            break;

        case Input::KEY_PAGE_UP:
            ret = "page up";
            break;

        case Input::KEY_PAGE_DOWN:
            ret = "page down";
            break;

        case Input::KEY_END:
            ret = "end";
            break;

        case Input::KEY_HOME:
            ret = "home";
            break;

        case Input::KEY_INSERT:
            ret = "insert";
            break;

        case Input::KEY_DELETE:
            ret = "delete";
            break;

        case Input::KEY_NUMPAD_ADD:
            ret = "add";
            break;

        case Input::KEY_NUMPAD_SUBTRACT:
            ret = "subtract";
            break;

        case Input::KEY_NUMPAD_MULTIPLY:
            ret = "multiply";
            break;

        case Input::KEY_NUMPAD_DIVIDE:
            ret = "divide";
            break;

        case Input::KEY_LEFT:
            ret = "left";
            break;

        case Input::KEY_RIGHT:
            ret = "right";
            break;

        case Input::KEY_UP:
            ret = "up";
            break;

        case Input::KEY_DOWN:
            ret = "down";
            break;

        case Input::KEY_NUMPAD_0:
        case Input::KEY_NUMPAD_1:
        case Input::KEY_NUMPAD_2:
        case Input::KEY_NUMPAD_3:
        case Input::KEY_NUMPAD_4:
        case Input::KEY_NUMPAD_5:
        case Input::KEY_NUMPAD_6:
        case Input::KEY_NUMPAD_7:
        case Input::KEY_NUMPAD_8:
        case Input::KEY_NUMPAD_9:
            oss << static_cast<char>(
                static_cast<int>( '0' ) +
                static_cast<int>( type ) -
                static_cast<int>( Input::KEY_NUMPAD_0 ) );
            ret = oss.str();
            break;

        case Input::KEY_F1:
        case Input::KEY_F2:
        case Input::KEY_F3:
        case Input::KEY_F4:
        case Input::KEY_F5:
        case Input::KEY_F6:
        case Input::KEY_F7:
        case Input::KEY_F8:
        case Input::KEY_F9:
        case Input::KEY_F10:
        case Input::KEY_F11:
        case Input::KEY_F12:
        case Input::KEY_F13:
        case Input::KEY_F14:
        case Input::KEY_F15:
            oss << "F" << ( 1 + static_cast<int>( type ) -
                            static_cast<int>( Input::KEY_F1 ) );
            ret = oss.str();
            break;

        case Input::KEY_PAUSE:
            ret = "pause";
            break;

            // case Input::MOUSE
        case Input::MOUSE_LEFT_BUTTON:
            ret = "left mouse button";
            break;

        case Input::MOUSE_RIGHT_BUTTON:
            ret = "right mouse button";
            break;

        case Input::MOUSE_MIDDLE_BUTTON:
            ret = "middle mouse button";
            break;

        case Input::MOUSE_XBUTTON1:
            ret = "extra mouse button 1";
            break;

        case Input::MOUSE_XBUTTON2:
            ret = "extra mouse button 2";
            break;

            // case Input::GAMEPAD
        case Input::GAMEPAD_BUTTON_1 :
        case Input::GAMEPAD_BUTTON_2:
        case Input::GAMEPAD_BUTTON_3:
        case Input::GAMEPAD_BUTTON_4:
        case Input::GAMEPAD_BUTTON_5:
        case Input::GAMEPAD_BUTTON_6:
        case Input::GAMEPAD_BUTTON_7:
        case Input::GAMEPAD_BUTTON_8:
        case Input::GAMEPAD_BUTTON_9:
        case Input::GAMEPAD_BUTTON_10:
        case Input::GAMEPAD_BUTTON_11:
        case Input::GAMEPAD_BUTTON_12:
        case Input::GAMEPAD_BUTTON_13:
        case Input::GAMEPAD_BUTTON_14:
        case Input::GAMEPAD_BUTTON_15:
        case Input::GAMEPAD_BUTTON_16:
        case Input::GAMEPAD_BUTTON_17:
        case Input::GAMEPAD_BUTTON_18:
        case Input::GAMEPAD_BUTTON_19:
        case Input::GAMEPAD_BUTTON_20:
        case Input::GAMEPAD_BUTTON_21:
        case Input::GAMEPAD_BUTTON_22:
        case Input::GAMEPAD_BUTTON_23:
        case Input::GAMEPAD_BUTTON_24:
        case Input::GAMEPAD_BUTTON_25:
        case Input::GAMEPAD_BUTTON_26:
        case Input::GAMEPAD_BUTTON_27:
        case Input::GAMEPAD_BUTTON_28:
        case Input::GAMEPAD_BUTTON_29:
        case Input::GAMEPAD_BUTTON_30:
        case Input::GAMEPAD_BUTTON_31:
        case Input::GAMEPAD_BUTTON_32:
            oss << "gamepad button " <<
            ( 1 + static_cast<int>( type ) -
              static_cast<int>( Input::GAMEPAD_BUTTON_1 ) );
            ret = oss.str();
            break;

        default:
            ret = "unknown";
            break;
    }

    return ret;
}

} // End nspc sgdu

} // End nspc StevensDev