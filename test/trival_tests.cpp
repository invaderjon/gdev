// trivial_tests.cpp
#include <gtest/gtest.h>

TEST( TrivialTest, Addition )
{
    int x = 5 + 4;

    EXPECT_EQ( 9, x );
    EXPECT_EQ( 12, x + 3 );
    EXPECT_EQ( 5, x + -4 );
}

TEST( TrivialTest, Subtraction )
{
    int x = 5 - 4;

    EXPECT_EQ( 1, x );
    EXPECT_EQ( -1, x - 2 );
    EXPECT_EQ( 4, x - -3 );
}

TEST( TrivialTest, Multiplication )
{
    int x = 5 * 4;

    EXPECT_EQ( 20, x );
    EXPECT_EQ( 100, x * 5 );
}