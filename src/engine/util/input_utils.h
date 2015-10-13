// input_utils.h
#ifndef INCLUDED_INPUT_UTILS
#define INCLUDED_INPUT_UTILS
#include "../input/input.h"

namespace StevensDev
{

namespace sgdu
{

struct InputUtils
{
    static std::string getInputTypeName( sgdi::Input::InputType type );
      // Gets the name of the input type.
};

} // End nspc sgdu

} // End nspc StevensDev

#endif
