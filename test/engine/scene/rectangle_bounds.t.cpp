// rectangle_bounds.t.cpp
#include <engine/scene/rectangle_bounds.h>
#include <gtest/gtest.h>

TEST( RectangleBoundsTest, Construction )
{
    using namespace StevensDev::sgds;

    RectangleBounds bounds;
    RectangleBounds copy( bounds );

    bounds = copy;
}

TEST( RectangleBoundsTest, Properties )
{
    using namespace StevensDev::sgds;

    RectangleBounds bounds( 0.0f, 1.0f, 2.0f, 3.0f );

    EXPECT_EQ( 0.0f, bounds.x() );
    EXPECT_EQ( 1.0f, bounds.y() );
    EXPECT_EQ( 2.0f, bounds.width() );
    EXPECT_EQ( 3.0f, bounds.height() );
    EXPECT_EQ( 0.0f, bounds.left() );
    EXPECT_EQ( 1.0f, bounds.top() );
    EXPECT_EQ( 2.0f, bounds.right() );
    EXPECT_EQ( 4.0f, bounds.bottom() );
}

TEST( RectangleBoundsTest, CollisionDetection )
{
    using namespace StevensDev::sgds;

    RectangleBounds a( 0.0f, 0.0f, 1.0f, 1.0f );
    RectangleBounds b( 0.5f, 0.5f, 2.0f, 2.0f );
    RectangleBounds c( 1.0f, 0.0f, 1.0f, 1.0f );
    RectangleBounds d( 2.0f, 2.0f, 1.0f, 1.0f );

    EXPECT_TRUE( a.doesCollide( a ) );
    EXPECT_TRUE( a.doesCollide( b ) );
    EXPECT_TRUE( a.doesCollide( c ) );
    EXPECT_FALSE( a.doesCollide( d ) );

    EXPECT_TRUE( b.doesCollide( b ) );
    EXPECT_TRUE( b.doesCollide( a ) );
    EXPECT_TRUE( b.doesCollide( c ) );
    EXPECT_TRUE( b.doesCollide( d ) );

    EXPECT_TRUE( c.doesCollide( c ) );
    EXPECT_TRUE( c.doesCollide( a ) );
    EXPECT_TRUE( c.doesCollide( b ) );
    EXPECT_FALSE( c.doesCollide( d ) );

    EXPECT_TRUE( d.doesCollide( d ) );
    EXPECT_FALSE( d.doesCollide( a ) );
    EXPECT_TRUE( d.doesCollide( b ) );
    EXPECT_FALSE( d.doesCollide( c ) );
}

TEST( RectangleBoundsTest, Print )
{
    using namespace StevensDev::sgds;

    RectangleBounds bounds( 0.0f, 0.0f, 5.0f, 5.0f );

    std::ostringstream oss;
    oss << bounds;

    EXPECT_STREQ(
        "{ \"x\": \"0.0\", \"y\": \"0.0\", \"width\": \"5.0\", \"height\": \"5.0\" }",
        oss.str().c_str() );
}