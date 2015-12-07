// token_factory.h
#ifndef INCLUDED_TOKEN_FACTORY
#define INCLUDED_TOKEN_FACTORY

#include "actor.h"
#include "piece.h"
#include "ghost.h"

namespace StevensDev
{

namespace mgo
{

struct TokenFactory
{
    static void getActorToken( const sgdd::JsonEntity& info,
                               ActorToken* token );
      // Gets an actor token using the information.

    static void getPieceToken( const sgdd::JsonEntity& info,
                               PieceToken* token );
      // Gets a piece token using the information.

    static void getGhostToken( const sgdd::JsonEntity& info,
                               GhostToken* token );
      // Gets a ghost token using the information.
};

// FUNCTIONS
inline
void TokenFactory::getPieceToken( const sgdd::JsonEntity& info,
                                  PieceToken* token )
{
    getActorToken( info, &token->actorToken );
}

} // End nspc mgo

} // End nspc StevensDev

#endif //GDEV_TOKEN_FACTORY_H
