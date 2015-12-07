// board.cpp
#include "board.h"

namespace StevensDev
{

namespace mgw
{

// GLOBALS
Board Board::d_board = Board();

// CONSTRUCTORS
Board::Board( const sgdd::JsonEntity& spaces,
              unsigned int columns, unsigned int rows,
              float spaceWidth, float spaceHeight )
    : d_width( columns ), d_height( rows ), d_spaceWidth( spaceWidth ),
      d_spaceHeight( spaceHeight )
{
    unsigned int space;
    for ( space = 0; space < d_width * d_height; ++space )
    {
        d_spaces.push( parseSpace( spaces[space] ) );
    }
}

} // End nspc mgw

} // End nspc StevensDev