// res.g.h
#ifndef INCLUDED_RES
#define INCLUDED_RES

#include <engine/assets/resource_database.h>

namespace StevensDev
{

namespace mgr
{

typedef sgda::ResourceID ResourceID;

struct R
{
    // CONSTANTS
    static const ResourceID TEST_RES;
    static const ResourceID TEXTURE_ACTOR_BLOCK_BLOCK;
    static const ResourceID FONT_UBUNTU_MONO_R;
    static const ResourceID CONFIG_ACTOR_BLOCK_PLAYER;
    static const ResourceID CONFIG_ACTOR_BLOCK_ENEMY;
};

} // End nspc mgr

} // End nspc StevensDev

#endif