// basic_string_reader.t.cpp
#include <engine/data/basic_string_reader.h>
#include <gtest/gtest.h>

TEST( BasicStringReaderTest, Construction )
{
    using namespace StevensDev::sgdd;

    BasicStringReader reader( "test" );
    BasicStringReader copy( reader );
    reader = copy;
}

TEST( BasicStringReaderTest, Reading )
{
    using namespace StevensDev::sgdd;

    std::ostringstream oss;

    oss << "Why would I design such an absurdly reusable "
        << "system!? For the glory of satan of course! "
        << "If you don't get the reference look up the "
        << "\"For the glory of satan\" meme; it's pretty funny...";

    std::string full = oss.str();
    BasicStringReader r( full );

    oss = std::ostringstream();

    while ( !r.isEnd() )
    {
        oss << r.get();
        r.advance( 1 );
    }

    ASSERT_STREQ( full.c_str(), oss.str().c_str() );

    oss = std::ostringstream();

    r = BasicStringReader( full );
    while ( !r.isEnd() )
    {
        oss << r.get( 10 );
        r.advance( 10 );
    }

    ASSERT_STREQ( full.c_str(), oss.str().c_str() );
}

TEST( BasicStringReaderTest, Print )
{
    using namespace StevensDev::sgdd;

    BasicStringReader r;

    std::ostringstream oss;
    oss << r;

    ASSERT_STREQ( "{ \"position\": 0 }", oss.str().c_str() );
}