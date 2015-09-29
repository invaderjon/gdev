// mem.t.cpp
#include "engine/memory/mem.h"
#include <gtest/gtest.h>

TEST( MemTest, Full )
{
    using namespace StevensDev::sgdm;

    char dst[5];

    Mem::copy( &dst[0], ( char* )"my", 3 );

    EXPECT_STREQ( "my", dst );

    // move is known to be broken at the moment

    Mem::set( &dst[0], '\0', 1 );

    EXPECT_STREQ( "", dst );

    Mem::copy( &dst[0], ( char* )"name", 5 );

    EXPECT_STREQ( "name", dst );

    Mem::clear( &dst[0], 5 );

    int i;
    for ( i = 0; i < 5; ++i )
    {
        EXPECT_EQ( '\0', dst[i] );
    }
}