// hash_utils.t.cpp
#include <engine/util/hash_utils.h>
#include <gtest/gtest.h>

TEST( HashUtilsTest, CompileTime )
{
    using namespace StevensDev::sgdu;

    EXPECT_EQ( HashUtils::fnv1a( "testing" ), chash( "testing" ) );
}
