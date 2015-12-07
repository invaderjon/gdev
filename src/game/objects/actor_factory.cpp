// actor_factory.cpp
#include <engine/assets/resource_manager.h>
#include <engine/util/string_utils.h>
#include "actor_factory.h"
#include "token_factory.h"

namespace StevensDev
{

namespace mgo
{

namespace
{

// ACTOR CLASSES
static const unsigned int PLAYER = sgdu::StringUtils::hash( "Player" );
static const unsigned int BLINKY = sgdu::StringUtils::hash( "Blinky" );
static const unsigned int PINKY = sgdu::StringUtils::hash( "Pinky" );
static const unsigned int INKY = sgdu::StringUtils::hash( "Inky" );
static const unsigned int CLYDE = sgdu::StringUtils::hash( "Clyde" );
static const unsigned int GEN_GHOST =
    sgdu::StringUtils::hash( "GenericGhost" );

} // End nspc anonymous

// GLOBALS
ActorFactory ActorFactory::d_instance  = ActorFactory();

// MEMBER FUNCTIONS
Actor* ActorFactory::get( const sgda::ResourceID& actor )
{
    using namespace sgda;
    using namespace sgdd;

    ResourceManager& mgr = ResourceManager::inst();
    Handle<ConfigTag> configHandle = mgr.getConfigHandle( actor );
    mgr.loadConfig( actor );

    const JsonEntity& config = mgr.get( configHandle );

    const std::string& cls = config["class"].asString();

    // construct based on hash
    Actor* inst;
    switch ( sgdu::StringUtils::hash( cls ) )
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
