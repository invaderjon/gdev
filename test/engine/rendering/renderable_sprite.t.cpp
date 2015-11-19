// renderable_sprite.t.cpp
#include <engine/rendering/renderable_sprite.h>
#include <engine/rendering/renderer.h>
#include <gtest/gtest.h>

TEST( RenderableSpriteTest, Construction )
{
    using namespace StevensDev::sgdr;

    RenderableSprite blank;
    RenderableSprite copy( blank );

    Renderer renderer;

    // can't continue if can't find the texture
    ASSERT_TRUE( renderer.loadTexture( "test", "res/texture/block.png" ) );

    RenderableSprite texture( renderer.getTexture( "test" ) );
    RenderableSprite sprite( sf::Sprite( renderer.getTexture( "test" ) ) );

    texture = sprite;
}

TEST( RenderableSpriteTest, Position )
{
    using namespace StevensDev::sgdr;

    Renderer renderer;

    // can't continue if can't find the texture
    ASSERT_TRUE( renderer.loadTexture( "test", "res/texture/block.png" ) );

    RenderableSprite sprite( renderer.getTexture( "test" ) );

    sprite.setPosition( 25.0f, 25.0f );

    EXPECT_EQ( 25.0f, sprite.getPositionX() );
    EXPECT_EQ( 25.0f, sprite.getPositionY() );

    sprite.move( -25.0f, -25.0f );

    EXPECT_EQ( 0.0f, sprite.getPositionX() );
    EXPECT_EQ( 0.0f, sprite.getPositionY() );
}

TEST( RenderableSpriteTest, Print )
{
    using namespace StevensDev::sgdr;

    Renderer renderer;

    // can't continue if can't find the texture
    ASSERT_TRUE( renderer.loadTexture( "test", "res/texture/block.png" ) );

    RenderableSprite sprite( renderer.getTexture( "test" ) );
    sprite.setPosition( 0.0f, 0.0f );

    std::ostringstream oss;

    oss << sprite;

    EXPECT_STREQ( "{ \"x\": \"0.0\", \"y\": \"0.0\" }", oss.str().c_str() );
}