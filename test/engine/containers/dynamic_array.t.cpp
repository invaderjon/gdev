// dynamic_array.t.cpp
#include "engine/containers/dynamic_array.h"
#include "engine/memory/counting_allocator.h"
#include <gtest/gtest.h>

TEST( DynamicArrayTest, ConstructionAndAssignment )
{
    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    DefaultAllocator<int> alloc;

    DynamicArray<int> array( &alloc );
    DynamicArray<int> copy( array );
    DynamicArray<int> capacity( &alloc, ( unsigned int )100 );
    DynamicArray<int> def;

    def = copy;
}

TEST( DynamicArrayTest, PushAndPop )
{
    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    int i;
    int tmp;

    DefaultAllocator<int> alloc;
    DynamicArray<int> array( &alloc );

    // push
    array.push( 0 );
    EXPECT_EQ( 0,  array[0] );

    array.push( 32 );
    EXPECT_EQ( 32, array[1] );

    for ( i = 0; i < 32; ++i )
    {
        array.push( i + 12 );
        ASSERT_EQ( i + 12, array[i + 2] );
    }

    for ( i = 0; i < 32; ++i )
    {
        array.pushFront( i + 69 );
        ASSERT_EQ( i + 69,  array[0] );
    }

    EXPECT_EQ( 66, array.size() );

    // pop
    while ( array.size() > 0 )
    {
        i = array.size();

        tmp = array[array.size() - 1];
        ASSERT_EQ( tmp, array.pop() );

        tmp = array[0];
        ASSERT_EQ( tmp, array.popFront() );

        ASSERT_EQ( i - 2, array.size() );
    }

    // mix
    for ( i = 0; i < 35; ++i )
    {
        array.pushFront( i + 69 );
        ASSERT_EQ( i + 69, array[0] );
    }
    array.popFront();
    array.push( 10 );
    EXPECT_EQ( 10,  array[array.size() - 1] );
}

TEST( DynamicArrayTest, At )
{
    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    int i;

    DefaultAllocator<int> alloc;
    DynamicArray<int> array( &alloc );

    for ( i = 0; i < 64; ++i )
    {
        array.push( i );
    }

    for ( i = 0; i < 64; ++i )
    {
        ASSERT_EQ( i, array.at( i ) );
    }

    EXPECT_THROW( array.at( 65 ), std::runtime_error );
}

TEST( DynamicArrayTest, InsertAndRemove )
{
    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    unsigned int i;
    unsigned int tmp;

    DefaultAllocator<unsigned int> alloc;
    DynamicArray<unsigned int> array( &alloc );

    // force wrap
    array.push( 0 );
    array.popFront();
    array.push( 0 );

    for ( i = 0; i < 128; ++i )
    {
        tmp = ( i * 2 ) % ( array.size() + 1 );
        array.insertAt( tmp, i );
        ASSERT_EQ( i, array.at( tmp ) );
    }

    for ( i = 0; i < 128; ++i)
    {
        tmp = array[( i * 2 ) % array.size()];
        ASSERT_EQ( tmp, array.removeAt( ( i * 2 ) % array.size() ) );
    }
}

TEST( DynamicArrayTest, Print )
{
    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    CountingAllocator<std::string> alloc;

    DynamicArray<std::string> arr( &alloc );
    arr.push("value");

    std::ostringstream oss;
    ASSERT_NO_FATAL_FAILURE( oss << arr );
}