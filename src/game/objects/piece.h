// piece.h
//
// Defines an actor that acts as a game board piece.
//
// Actors are "brought to life" by "feeding" them their associated "token".
#ifndef INCLUDED_PIECE
#define INCLUDED_PIECE

#include "game/objects/actor.h"
#include <gel/math/vec.h>
#include <game/world/board.h>

namespace StevensDev
{

namespace mgo
{

// Game "Token" that spawns a board piece.
struct PieceToken
{
    ActorToken actorToken;
      // Sub-Type information
};

class Piece : public Actor
{
  private:
    sgdc::DynamicArray<gel::math::IVec2> d_motionPath;
      // The piece's motion path.
      //
      // The first is the current space and the rest are target spaces.

  public:
    // CONSTRUCTORS
    Piece();
      // Constructs a new game piece at (0, 0) on the board.

    Piece( const PieceToken& token );
      // Constructs a new game piece and feeds it the token

    Piece( const Piece& piece );
      // Constructs a copy of the given game piece.

    virtual ~Piece();
      // Destructs the game piece.

    // OPERATORS
    Piece& operator=( const Piece& piece );
      // Makes this a copy of the given piece.

    // MEMBER FUNCTIONS
    const gel::math::IVec2& getCurrentSpace() const;
      // Gets the space the piece is currently occupying.

    const gel::math::IVec2& getTargetSpace( unsigned int n ) const;
      // Gets the nth target space for the piece's current motion path.

    unsigned int getTargetCount() const;
      // Gets the number of targets currently part of the motion path.

    void setCurrentSpace( const gel::math::IVec2& current );
      // Sets the current space to the specified location.

    gel::math::IVec2 getDirection( unsigned int n ) const;
      // Gets the direction to the nth target.
      //
      // 0 means the direction to the 1st target from the current path node.
      // 1 means the direction to the 2nd target from the 1st target.
      // ...

    void addTarget( const gel::math::IVec2& target );
      // Adds a new target to the motion path.

    void clearTargets();
      // Clears all targets in the piece's motion path.

    void jumpToNextTarget();
      // Updates the current space to the next target (if available).

    virtual gel::math::Vec2 getWorldPositionForSpace(
            const gel::math::IVec2& space ) const;
      // Gets the world position the piece would have if it were in the
      // specified space.

    virtual gel::math::Vec2 getWorldPositionForSpace(
        const gel::math::IVec2& space, const mgw::Board& board ) const;
    // Gets the world position the piece would have if it were in the
    // specified space.
};

// CONSTRUCTORS
inline
Piece::Piece() : Actor(), d_motionPath()
{
    d_motionPath.push( gel::math::IVec2( 0, 0 ) );
}

inline
Piece::Piece( const PieceToken& token ) : Actor( token.actorToken ),
                                          d_motionPath()
{
    d_motionPath.push( gel::math::IVec2( 0, 0 ) );
}

inline
Piece::Piece( const Piece& piece ) : Actor( piece ),
                                     d_motionPath( piece.d_motionPath )
{
}

inline
Piece::~Piece()
{
}

// OPERATORS
inline
Piece& Piece::operator=( const Piece& piece )
{
    d_motionPath = piece.d_motionPath;
    return *this;
}

// MEMBER FUNCTIONS
inline
const gel::math::IVec2& Piece::getCurrentSpace() const
{
    assert( d_motionPath.size() > 0 );
    return d_motionPath[0];
}

inline
const gel::math::IVec2& Piece::getTargetSpace( unsigned int n ) const
{
    assert( n + 1 < d_motionPath.size() );
    return d_motionPath[n + 1];
}

inline
unsigned int Piece::getTargetCount() const
{
    return d_motionPath.size() - 1;
}

inline
void Piece::setCurrentSpace( const gel::math::IVec2& current )
{
    assert( d_motionPath.size() > 0 );
    d_motionPath[0] = current;
}

inline
void Piece::addTarget( const gel::math::IVec2& target )
{
    d_motionPath.push( target );
}

inline
void Piece::jumpToNextTarget()
{
    if ( d_motionPath.size() < 2 )
    {
        return;
    }

    d_motionPath.popFront();
}

inline
gel::math::IVec2 Piece::getDirection( unsigned int n ) const
{
    assert( n + 1 < d_motionPath.size() );
    return d_motionPath[n + 1] - d_motionPath[n];
}

} // End nspc mgo

} // End nspc StevensDev

#endif
