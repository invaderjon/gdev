// ghost.h
#ifndef INCLUDED_GHOST
#define INCLUDED_GHOST

#include "game/objects/piece.h"

namespace StevensDev
{

namespace mgo
{

// Game "Token" that spawns a ghost.
struct GhostToken
{
    PieceToken pieceToken;
      // Sub-Type information.

    std::string personality;
      // The ghost's personality.
};

class Ghost : public Piece
{
  private:
    std::string d_personality;
      // The ghost's personality

  public:
    // CONSTRUCTORS
    Ghost();
      // Constructs a default ghost.

    Ghost( const GhostToken& token );
      // Constructs a new ghost and feeds it the token.

    Ghost( const Ghost& ghost );
      // Constructs a ccopy of the given ghost.

    ~Ghost();
      // Destructs the ghost.

    // OPERATORS
    Ghost& operator=( const Ghost& ghost );
      // Makes this a copy of the given ghost.
};

// CONSTRUCTORS
inline
Ghost::Ghost() : Piece(), d_personality( "Random" )
{
}

inline
Ghost::Ghost( const GhostToken& token ) : Piece( token.pieceToken ),
                                          d_personality( token.personality )
{
}

inline
Ghost::Ghost( const Ghost& ghost ) : Piece( ghost ),
                                     d_personality( ghost.d_personality )
{
}

inline
Ghost::~Ghost()
{
}

// OPERATORS
inline
Ghost& Ghost::operator=( const Ghost& ghost )
{
    d_personality = ghost.d_personality;

    return *this;
}

} // End nspc mgo

} // End nspc StevensDev


#endif
