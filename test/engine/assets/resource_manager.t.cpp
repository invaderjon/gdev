// resource_manager.t.cpp
#include <engine/assets/resource_manager.h>
#include <game/res.g.h>
#include <gtest/gtest.h>

TEST( ResourceManagerTest, Images )
{
    using namespace StevensDev::sgda;
    using namespace StevensDev::mgr;

    ResourceManager& manager = ResourceManager::inst();

    Handle<ImageTag> handle =
        manager.getImageHandle( R::TEXTURE_ACTOR_BLOCK_BLOCK );

    manager.loadImage( R::TEXTURE_ACTOR_BLOCK_BLOCK );

    sf::Image& image = manager.get( handle );
}

TEST( ResourceManagerTest, Configs )
{
    using namespace StevensDev::sgda;
    using namespace StevensDev::sgdd;
    using namespace StevensDev::mgr;

    ResourceManager& manager = ResourceManager::inst();

    Handle<ConfigTag> handle =
        manager.getConfigHandle( R::CONFIG_ACTOR_BLOCK_ENEMY );

    manager.loadConfig( R::CONFIG_ACTOR_BLOCK_ENEMY );

    JsonEntity& config = manager.get( handle );
}