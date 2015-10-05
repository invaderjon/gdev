// resource_manager.t.cpp
#include "engine/game/resource_manager.h"
#include <gtest/gtest.h>

TEST( ResourceManagerTest, Construction )
{
    using namespace StevensDev::sgdg;

    ResourceManager resMgr;
    ResourceManager resMgr2( resMgr );

    resMgr = resMgr2;
}

TEST( ResourceManagerTest, Fonts )
{
    using namespace StevensDev::sgdg;

    ResourceManager resMgr;

    EXPECT_FALSE( resMgr.fastLoadFont( "failure" ) );
    EXPECT_THROW( resMgr.getFont( "failure" ), std::runtime_error );

    EXPECT_TRUE( resMgr.fastLoadFont( "ubuntu-mono-r" ) );
    EXPECT_NO_FATAL_FAILURE( resMgr.getFont( "ubuntu-mono-r" ) );
}