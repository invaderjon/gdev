// string_utils.t.cpp
#include "engine/util/string_utils.h"
#include <gtest/gtest.h>

TEST( StringUtilsTest, AreEqual )
{
    using namespace StevensDev::sgdu;

    EXPECT_TRUE( StringUtils::areEqual( "XJ-9", "XJ-9" ) );

    EXPECT_FALSE( StringUtils::areEqual( "Jenny", "Brad" ) );
}

TEST( StringUtilsTest, Compare )
{
    using namespace StevensDev::sgdu;

    EXPECT_TRUE( StringUtils::compare( "dog", "cat" ) > 0 );

    EXPECT_TRUE( StringUtils::compare( "loose", "win" ) < 0 );

    EXPECT_EQ( 0, StringUtils::compare( "Johnny Bravo", "Johnny Bravo" ));
}