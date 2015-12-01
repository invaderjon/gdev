// plain_text_file_reader.t.cpp
#include <engine/data/plain_text_file_reader.h>
#include <gtest/gtest.h>

TEST( PlainTextFileReaderTest, Construction )
{
    using namespace StevensDev::sgdd;

    PlainTextFileReader r;
    PlainTextFileReader r2( "res/config/actor/block/player.conf" );
}

TEST( PlainTextFileREaderTest, Reading )
{
    using namespace StevensDev::sgdd;

    PlainTextFileReader r( "res/config/actor/block/player.conf" );

    ASSERT_EQ( '{', r.get() );
    ASSERT_STREQ( "{\n", r.get( 2 ).c_str() );
    ASSERT_STRNE( "{\n", r.get( 15 ).c_str() );
    ASSERT_NO_FATAL_FAILURE( r.get( 3000000 ) );

    while( !r.isEnd() )
    {
        ASSERT_NO_FATAL_FAILURE( r.advance( 7 ) );
    }
}

TEST( PlainTextFileReaderTest, Print )
{
    using namespace StevensDev::sgdd;

    PlainTextFileReader r;

    std::ostringstream oss;
    oss << r;

    ASSERT_STREQ( "{ \"position\": 0 }", oss.str().c_str() );
}