// handle.t.cpp
#include <engine/assets/handle.h>
#include <gtest/gtest.h>

struct Tag
{
};

TEST( HandleTest, Construction )
{
    using namespace StevensDev::sgda;

    Handle<Tag> handle;

    ASSERT_EQ( Handle<Tag>::NULL_HANDLE, handle );

    handle = Handle<Tag>( 1, 1 );

    ASSERT_NE( Handle<Tag>::NULL_HANDLE, handle );
}

TEST( HandleTest, Accessors )
{
    using namespace StevensDev::sgda;

    Handle<Tag> handle( 1, 2 );

    ASSERT_EQ( 1, handle.index() );
    ASSERT_EQ( 2, handle.checkValue() );
}