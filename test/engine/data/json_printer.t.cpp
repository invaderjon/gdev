// json_printer.t.cpp
#include "engine/data/json_printer.h"
#include <gtest/gtest.h>

TEST( JsonPrinterTest, Construction )
{
    using namespace StevensDev::sgdd;

    std::ostringstream oss;

    JsonPrinter printer;
    JsonPrinter spec( oss );
    JsonPrinter copy( printer );
}

TEST( JsonPrinterTest, Primitive )
{
    using namespace StevensDev::sgdd;

    std::ostringstream oss;
    JsonPrinter p( oss );
    ASSERT_NO_FATAL_FAILURE( p << "test" );
    ASSERT_STREQ( "\"test\"", oss.str().c_str() );

    std::ostringstream oss2;
    p = JsonPrinter( oss2 );
    ASSERT_NO_FATAL_FAILURE( p << 1 );
    ASSERT_STREQ( "\"1\"", oss2.str().c_str() );

    std::ostringstream oss3;
    p = JsonPrinter( oss3 );
    ASSERT_NO_FATAL_FAILURE( p << 439.5 );
    ASSERT_STREQ( "\"439.5\"", oss3.str().c_str() );

    std::ostringstream oss4;
    p = JsonPrinter( oss4 );
    ASSERT_NO_FATAL_FAILURE( p << false );
    ASSERT_STREQ( "\"false\"", oss4.str().c_str() );
}

TEST( JsonPrinterTest, KeyValuePair )
{
    using namespace StevensDev::sgdd;

    std::ostringstream oss;
    JsonPrinter p( oss );
    ASSERT_NO_FATAL_FAILURE( p.print( "k", "v" ) );
    ASSERT_STREQ( "\"k\": \"v\"", oss.str().c_str() );

    std::ostringstream oss2;
    p = JsonPrinter( oss2 );
    ASSERT_NO_FATAL_FAILURE( p.print( "k", 1 ) );
    ASSERT_STREQ( "\"k\": \"1\"", oss2.str().c_str() );

    std::ostringstream oss3;
    p = JsonPrinter( oss3 );
    ASSERT_NO_FATAL_FAILURE( p.print( std::string( "k" ), 1.5f ) );
    ASSERT_STREQ( "\"k\": \"1.5\"", oss3.str().c_str() );
}

TEST( JsonPrinterTest, Array )
{
    using namespace StevensDev::sgdd;

    std::string arr[3] { "x", "y", "z" };

    std::ostringstream oss;
    JsonPrinter p( oss );
    ASSERT_NO_FATAL_FAILURE( p.printArr( "k", &arr[0], 3 ) );
    ASSERT_STREQ( "\"k\": [ \"x\", \"y\", \"z\" ]", oss.str().c_str() );

    std::ostringstream oss2;
    p = JsonPrinter( oss2 );
    ASSERT_NO_FATAL_FAILURE( p.printArr( &arr[0], 3 ) );
    ASSERT_STREQ( "[ \"x\", \"y\", \"z\" ]", oss2.str().c_str() );
}

TEST( JsonPrinterTest, Print )
{
    using namespace StevensDev::sgdd;

    std::ostringstream tmp;
    JsonPrinter p( tmp );

    std::ostringstream oss;
    oss << p;

    ASSERT_STREQ( "{  }", oss.str().c_str() );
}