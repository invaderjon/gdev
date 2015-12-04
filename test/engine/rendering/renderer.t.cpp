// renderer.t.cpp
#include <engine/rendering/renderer.h>
#include <gtest/gtest.h>

TEST( RendererTest, Construction )
{
    using namespace StevensDev::sgdr;

    Renderer renderer;
    Renderer copy( renderer );
    Renderer moved( std::move( copy ) );

    renderer = moved;
}

TEST( RendererTest, SpriteManagement )
{
    using namespace StevensDev::sgdr;

    Renderer renderer;
    RenderableSprite sprite;

    EXPECT_NO_FATAL_FAILURE( renderer.addSprite( &sprite ) );
    EXPECT_NO_FATAL_FAILURE( renderer.removeSprite( &sprite ) );
}

TEST( RendererTest, WindowCreationAndDrawing )
{
    using namespace StevensDev::sgdr;

    Renderer renderer;

    EXPECT_FALSE( renderer.isActive() );

    EXPECT_NO_FATAL_FAILURE( renderer.setupWindow( 200, 200 ) );

    EXPECT_TRUE( renderer.isActive() );

    EXPECT_NO_FATAL_FAILURE( renderer.draw() );
}

TEST( RendererTest, TextureLoading )
{
    using namespace StevensDev::sgdr;

    Renderer renderer;

    EXPECT_FALSE( renderer.loadTexture( "null", "null" ) );
    EXPECT_TRUE( renderer.loadTexture( "block", "res/texture/actor/block/block.png" ) );
}

TEST( RendererTest, Print )
{
    using namespace StevensDev::sgdr;

    Renderer renderer;

    std::ostringstream oss;

    oss << renderer;

    EXPECT_STREQ( "{ \"isActive\": false }", oss.str().c_str() );
}