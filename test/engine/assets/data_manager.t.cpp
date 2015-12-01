// data_manager.t.cpp
#include <engine/assets/data_manager.h>
#include <engine/assets/test_data_factory.h>
#include <gtest/gtest.h>

struct Tag
{
};

TEST( DataManagerTest, Construction )
{
    using namespace StevensDev::sgda;
    using namespace StevensDev::sgdt;

    TestDataFactory factory;
    DataManager<Tag, std::string> null;
    DataManager<Tag, std::string> manager( &factory );
    DataManager<Tag, std::string> copy( manager );
    manager = null;
}

TEST( DataMangerTest, Handles )
{
    using namespace StevensDev::sgda;
    using namespace StevensDev::sgdt;

    // prepare factory
    TestDataFactory factory;
    factory.addTest( "path1", "value1" );
    factory.addTest( "path2", "value2" );
    factory.addTest( "path3", "value3" );

    ASSERT_FALSE( factory.isLoaded( "value1" ) );
    ASSERT_FALSE( factory.isLoaded( "value2" ) );
    ASSERT_FALSE( factory.isLoaded( "value3" ) );

    // prepare manager
    DataManager<Tag, std::string> manager( &factory );
    manager.addFile( 0, "path1", sizeof( "value1" ) );
    manager.addFile( 1, "path2", sizeof( "value2" ) );
    manager.addFile( 2, "path3", sizeof( "value3" ) );

    // acquisition tests
    Handle<Tag> h1 = manager.acquire( 0 );
    Handle<Tag> h2 = manager.acquire( 1 );
    Handle<Tag> h3 = manager.acquire( 2 );

    ASSERT_NE( Handle<Tag>::NULL_HANDLE, h1 );
    ASSERT_NE( Handle<Tag>::NULL_HANDLE, h2 );
    ASSERT_NE( Handle<Tag>::NULL_HANDLE, h3 );

    // loading tests
    manager.load( 0 );
    manager.load( 1, true );
    manager.load( 2, false );

    ASSERT_TRUE( factory.isLoaded( "value1" ) );
    ASSERT_TRUE( factory.isLoaded( "value2" ) );
    ASSERT_TRUE( factory.isLoaded( "value3" ) );
    ASSERT_TRUE( manager.isLoaded( 0 ) );
    ASSERT_TRUE( manager.isLoaded( 1 ) );
    ASSERT_TRUE( manager.isLoaded( 2 ) );

    ASSERT_STREQ( "value1", manager.dereference( h1 ).c_str() );
    ASSERT_STREQ( "value2", manager.dereference( h2 ).c_str() );
    ASSERT_STREQ( "value3", manager.dereference( h3 ).c_str() );

    // unloading tsts
    manager.unload( 1 );

    ASSERT_TRUE( factory.isLoaded( "value1" ) );
    ASSERT_FALSE( factory.isLoaded( "value2" ) );
    ASSERT_TRUE( factory.isLoaded( "value3" ) );
    ASSERT_TRUE( manager.isLoaded( 0 ) );
    ASSERT_FALSE( manager.isLoaded( 1 ) );
    ASSERT_TRUE( manager.isLoaded( 2 ) );

    ASSERT_STREQ( "value1", manager.dereference( h1 ).c_str() );
    ASSERT_STREQ( "value3", manager.dereference( h3 ).c_str() );

    manager.unload( 0 );
    manager.unload( 2 );

    manager.clean();

    ASSERT_TRUE( manager.hasHandle( 0 ) );
    ASSERT_TRUE( manager.hasHandle( 1 ) );
    ASSERT_TRUE( manager.hasHandle( 2 ) );

    ASSERT_FALSE( factory.isLoaded( "value1" ) );
    ASSERT_FALSE( factory.isLoaded( "value2" ) );
    ASSERT_FALSE( factory.isLoaded( "value3" ) );
    ASSERT_FALSE( manager.isLoaded( 0 ) );
    ASSERT_FALSE( manager.isLoaded( 1 ) );
    ASSERT_FALSE( manager.isLoaded( 2 ) );

    // releasing tests
    ASSERT_TRUE( manager.release( h1 ) );
    ASSERT_TRUE( manager.release( h2 ) );
    ASSERT_TRUE( manager.release( h3 ) );
    ASSERT_FALSE( manager.release( h1 ) );
    ASSERT_FALSE( manager.release( h2 ) );
    ASSERT_FALSE( manager.release( h3 ) );

    manager.clean();

    ASSERT_FALSE( manager.hasHandle( 0 ) );
    ASSERT_TRUE( manager.hasHandle( 1 ) );
    ASSERT_FALSE( manager.hasHandle( 2 ) );
}