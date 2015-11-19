// scene.t.cpp
#include <engine/scene/scene.h>
#include <engine/rendering/renderer.h>
#include <engine/scene/test_tickable.h>
#include <gtest/gtest.h>

TEST( SceneTest, Singleton )
{
    using namespace StevensDev::sgds;

    Scene& scene = Scene::inst();
}

TEST( SceneTest, Ticking )
{
    using namespace StevensDev::sgds;
    using namespace StevensDev::sgdt;

    Scene& scene = Scene::inst();

    TestTickable tickable;

    scene.addTickable( &tickable );

    scene.tick();

    EXPECT_TRUE( tickable.wasPreTicked() );
    EXPECT_TRUE( tickable.wasTicked() );
    EXPECT_TRUE( tickable.wasPostTicked() );

    scene.removeTickable( &tickable );

    tickable.reset();

    scene.tick();

    EXPECT_FALSE( tickable.wasPreTicked() );
    EXPECT_FALSE( tickable.wasTicked() );
    EXPECT_FALSE( tickable.wasPostTicked() );
}

TEST( SceneTest, Print )
{
    using namespace StevensDev::sgds;

    Scene& scene = Scene::inst();

    std::ostringstream oss;

    oss << scene;

    EXPECT_STREQ( "{  }", oss.str().c_str() );
}