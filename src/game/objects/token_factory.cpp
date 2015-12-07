// token_factory.cpp
#include <engine/assets/resource_manager.h>
#include "token_factory.h"

namespace StevensDev
{

namespace mgo
{

void TokenFactory::getActorToken( const sgdd::JsonEntity& info,
                                  ActorToken* token )
{
    token->name = info["name"].asString();
    token->properties = info["properties"];
    token->flags = sgds::Collision::ACTOR_COLLISION;
    token->bounds = sgds::RectangleBounds(
        ( float )info["bounds"]["x"].asDouble(),
        ( float )info["bounds"]["y"].asDouble(),
        ( float )info["bounds"]["width"].asDouble(),
        ( float )info["bounds"]["height"].asDouble());

    std::string imageID = info["sprite"]["image"].asString();

    sgda::ResourceManager& mgr = sgda::ResourceManager::inst();
    if ( !mgr.isTextureLoaded( imageID ) )
    {
        mgr.loadTexture( imageID );
    }
    sgda::Handle<sgda::TextureTag> handle = mgr.getTextureHandle( imageID );
    token->sprite = handle;
    token->spriteSize = sgds::RectangleBounds(
        0.0f, 0.0f,
        ( float )info["sprite"]["width"].asDouble(),
        ( float )info["sprite"]["height"].asDouble());
}

void TokenFactory::getGhostToken( const sgdd::JsonEntity& info,
                                  GhostToken* token )
{
    getPieceToken( info, &token->pieceToken );

    token->personality = info["personality"].asString();
}

} // End nspc mgo

} // End nspc StevensDev
