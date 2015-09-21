// map.cpp
#include "engine/containers/map.h"
#include <gtest/gtest.h>

namespace
{

StevensDev::sgdc::DynamicArray<std::string>
getKeys( StevensDev::sgdm::IAllocator<std::string>* allocator )
{
    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    DynamicArray<std::string> arr( allocator );

    int i;
    int j;
    for ( i = 0; i < 128; ++i )
    {
        std::ostringstream oss;
        for ( j = 0; j <= i; ++j )
        {
            oss << std::string( "a" );
        }

        arr.push( oss.str() );
    }

    return arr;
}

} // End nspc anonymous

TEST( MapTest, Construction )
{
    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    CountingAllocator<std::string> alloc;

    Map<std::string> map( &alloc );
    Map<std::string> mapSized( &alloc, 3000 );
    Map<std::string>& copy = map;

    Map<std::string> copyMap( copy );
    Map<std::string> moveMap( std::move( map ) );
}

TEST( MapTest, MoveAndCopyData )
{
    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    CountingAllocator<std::string> alloc;

    Map<std::string> map( &alloc );
    DynamicArray<std::string> keys = getKeys( &alloc );

    int i;
    for ( i = 0; i < keys.size(); ++i )
    {
        map[keys[i]] = keys[i];
    }

    Map<std::string> copy( map );
    for ( i = 0; i < keys.size(); ++i )
    {
        ASSERT_STREQ( keys[i].c_str(), copy[keys[i]].c_str() );
    }

    Map<std::string> moved( std::move( copy ) );
    for ( i = 0; i < keys.size(); ++i )
    {
        ASSERT_STREQ( keys[i].c_str(), moved[keys[i]].c_str() );
    }

    ASSERT_EQ( 0, copy.values().size() );
    ASSERT_NE( 0, map.values().size() );
}

TEST( MapTest, SetAndGet )
{
    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    CountingAllocator<std::string> alloc;

    Map<std::string> map( &alloc );
    DynamicArray<std::string> keys = getKeys( &alloc );

    map["test"] = "value";
    map["test"] = "value";
    ASSERT_STREQ( "value", map["test"].c_str() );
    ASSERT_EQ( 1, map.values().size() );
    ASSERT_STREQ( "value", map.remove("test").c_str() );
    ASSERT_EQ( 0, map.values().size() );

    int i;
    for ( i = 0; i < keys.size(); ++i )
    {
        map[keys[i]] = keys[i];
        ASSERT_STREQ( keys[i].c_str(), map[keys[i]].c_str() );
    }

    for ( i = 0; i < keys.size(); ++i )
    {
        ASSERT_STREQ( keys[i].c_str(), map[keys[i]].c_str() );
    }

    for ( i = 0; i < keys.size(); ++i )
    {
        EXPECT_TRUE( map.has( keys[i] ) );
        map.remove( keys[i] );
        ASSERT_FALSE( map.has( keys[i] ) );
    }
}

TEST( MapTest, KeysAndValues )
{

    using namespace StevensDev::sgdc;
    using namespace StevensDev::sgdm;

    CountingAllocator<std::string> alloc;

    Map<std::string> map( &alloc );
    DynamicArray<std::string> keys = getKeys( &alloc );

    map["test"] = "value";

    ASSERT_EQ( "value", map.values()[0] );
    ASSERT_EQ( "test", map.keys()[0] );
}