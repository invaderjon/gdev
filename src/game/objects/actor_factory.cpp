// actor_factory.cpp
#include <engine/assets/resource_manager.h>
#include <engine/util/hasher.h>
#include "actor_factory.h"
#include "token_factory.h"

namespace StevensDev
{

namespace mgo
{

// GLOBALS
ActorFactory ActorFactory::d_instance  = ActorFactory();

// MEMBER FUNCTIONS
Actor* ActorFactory::get( const sgda::ResourceID& actor )
{
    using namespace sgda;
    using namespace sgdd;
    using namespace sgdu;

    ResourceManager& mgr = ResourceManager::inst();
    Handle<ConfigTag> configHandle = mgr.getConfigHandle( actor );
    mgr.loadConfig( actor );

    const JsonEntity& config = mgr.get( configHandle );

    const std::string& cls = config["class"].asString();

    // construct based on hash
    Actor* inst;
    switch ( Hasher<std::string>::hash( cls ) )
    {
        case chash( "Piece" ):
        {
            PieceToken pieceToken;
            TokenFactory::getPieceToken( config, &pieceToken );
            inst = new Piece( pieceToken );
        } break;

        case chash( "Ghost" ):
        {
            GhostToken ghostToken;
            TokenFactory::getGhostToken( config, &ghostToken );
            inst = new Ghost( ghostToken );
        } break;

        default:
        {
            inst = nullptr;
        } break;
    }

    return inst;
}

} // End nspc mgo

} // End nspc StevensDev
