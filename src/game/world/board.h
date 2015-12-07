// board.h
//
// The game board is oriented as follows:
//
//             top
//
//           column
//       0 ___1___2___ ...
//        |   |   |   |
// l   r 1|___|___|___|_
// e   o  |   |   |   |
// f   w 2|___|___|___|_
// t      |   |   |   |
//     ...|___|___|___|_
//        |   |   |   |
//
// ( x, y ) = ( column, row )
//
#ifndef INCLUDED_BOARD
#define INCLUDED_BOARD

#include <engine/data/json_entity.h>
#include <engine/data/iserializable.h>

namespace StevensDev
{

namespace mgw
{

class Board : public sgdd::ISerializable
{
  private:
    // TYPES
    struct Space
    {
        bool isTraversable;
          // If pieces can exist inside of the space.
    };
    // Defines a space.
    //
    // Uses a structure so that at a later date other data may be added.

    // GLOBALS
    static Board d_board;
      // The current board.

    // MEMBERS
    sgdc::DynamicArray<Space> d_spaces;
      // The board as a list of spaces.

    unsigned int d_width;
      // The board width.

    unsigned int d_height;
      // The board height.

    float d_spaceWidth;
      // The witdth of a space (WU).

    float d_spaceHeight;
      // The height of a space (WU).

    // OPERATORS
    friend std::ostream& operator<<( std::ostream& stream,
                                     const Space& space );
      // Outputs board space information.

    // HELPER FUNCTIONS
    Space parseSpace( const sgdd::JsonEntity& space ) const;
      // Parses the json representation of the board space.

  public:
    // GLOBAL FUNCTIONS
    static void set( const Board& board );
      // Changes to the give board.

    static Board& get();
      // Gets the current board.

    // CONSTRUCTORS
    Board();
      // Constructs an empty board with no spaces.

    Board( const sgdd::JsonEntity& spaces,
           unsigned int columns, unsigned int rows,
           float spaceWidth, float spaceHeight );
      // Constructs a new board with the given configuration.

    Board( const Board& board );
      // Constructs a copy of the given board.

    ~Board();
      // Destroys the board.

    // OPERATORS
    Board& operator=( const Board& board );
      // Makes this a copy of the given board.

    // ACCESSOR FUNCTIONS
    unsigned int width() const;
      // Gets the board's width in spaces.

    unsigned int height() const;
      // Gets the board's height in spaces.

    float spaceWidth() const;
      // Gets the width of a space on the board (WU).

    float spaceHeight() const;
      // Gets the height of a space on the board (WU).

    // MEMBER FUNCTIONS
    bool isTraversable( unsigned int column, unsigned int row ) const;
      // Checks if the space at the specified length is traversable.
      //
      // Traversability refers to the ability of Piece actors to occupy that
      // location.
      //
      // The column is the x coordinate and the row is the y coordinate from
      // the the top left of the board.

    virtual std::ostream& serialize( std::ostream& out ) const;
      // Serializes the board to the stream.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream, const Board& board )
{
    return board.serialize( stream );
}

inline
std::ostream& operator<<( std::ostream& stream, const Board::Space& space )
{
    return stream << space.isTraversable;
}

// GLOBAL FUNCTIONS
inline
void Board::set( const Board& board )
{
    d_board = board;
}

inline
Board& Board::get()
{
    return d_board;
}

// CONSTRUCTORS
inline
Board::Board() : d_spaces(), d_width( 0 ), d_height( 0 ), d_spaceWidth( 1.0f ),
                 d_spaceHeight( 1.0f )
{
}

inline
Board::Board( const Board& board )
    : d_spaces( board.d_spaces ), d_width( board.d_width ),
      d_height( board.d_height ), d_spaceWidth( board.d_spaceWidth ),
      d_spaceHeight( board.d_spaceHeight )
{
}

inline
Board::~Board()
{
}

// OPERATORS
inline
Board& Board::operator=( const Board& board )
{
    d_spaces = board.d_spaces;
    d_width = board.d_width;
    d_height = board.d_height;
    d_spaceWidth = board.d_spaceWidth;
    d_spaceHeight = board.d_spaceHeight;

    return *this;
}

// ACCESSOR FUNCTIONS
inline
unsigned int Board::width() const
{
    return d_width;
}

inline
unsigned int Board::height() const
{
    return d_height;
}

inline
float Board::spaceWidth() const
{
    return d_spaceWidth;
}

inline
float Board::spaceHeight() const
{
    return d_spaceHeight;
}

// MEMBER FUNCTIONS
inline
bool Board::isTraversable( unsigned int column, unsigned int row ) const
{
    if ( row >= d_height || column >= d_width )
    {
        return false;
    }

    return d_spaces[column * d_height + row].isTraversable;
}

inline
std::ostream& Board::serialize( std::ostream& out ) const
{
    sgdd::JsonPrinter p( out );
    p.open().print( "width", d_width )
            .print( "height", d_height )
            .printArr( "spaces", d_spaces, d_spaces.size() )
            .close();
    return out;
}

// HELPER FUNCTIONS
inline
Board::Space Board::parseSpace( const sgdd::JsonEntity& space ) const
{
    Space sp;
    sp.isTraversable = space.asBoolean();
    return sp;
}

} // End nspc mgw

} // End nspc StevensDev

#endif
