// resource_manager.t.cpp
#include <engine/assets/resource_manager.h>
#include <game/res.h>
#include <gtest/gtest.h>

TEST( ResourceManagerTest, Images )
{
    using namespace StevensDev::sgda;
    using namespace StevensDev::mgr;

    ResourceDatabase rdb;
    rdb.addPackageResources( "res/res.db" );

    ResourceManager::init( &rdb );
    ResourceManager& manager = ResourceManager::inst();

    Handle<TextureTag> handle =
        manager.getTextureHandle( R::TEXTURE_ACTOR_BLOCK_BLOCK );

    manager.loadTexture( R::TEXTURE_ACTOR_BLOCK_BLOCK );

    sf::Texture& tex = manager.get( handle );
}

TEST( ResourceManagerTest, Configs )
{
    using namespace StevensDev::sgda;
    using namespace StevensDev::sgdd;
    using namespace StevensDev::mgr;

    ResourceDatabase rdb;
    rdb.addPackageResources( "res/res.db" );

    ResourceManager::init( &rdb );
    ResourceManager& manager = ResourceManager::inst();

    Handle<ConfigTag> handle =
        manager.getConfigHandle( R::CONFIG_ACTOR_BLOCK_ENEMY );

    manager.loadConfig( R::CONFIG_ACTOR_BLOCK_ENEMY );

    JsonEntity& config = manager.get( handle );
}