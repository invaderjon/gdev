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
    manager.addFile( "rid1", "path1", sizeof( "value1" ) );
    manager.addFile( "rid2", "path2", sizeof( "value2" ) );
    manager.addFile( "rid3", "path3", sizeof( "value3" ) );

    // acquisition tests
    Handle<Tag> h1 = manager.acquire( "rid1" );
    Handle<Tag> h2 = manager.acquire( "rid2" );
    Handle<Tag> h3 = manager.acquire( "rid3" );

    ASSERT_NE( Handle<Tag>::NULL_HANDLE, h1 );
    ASSERT_NE( Handle<Tag>::NULL_HANDLE, h2 );
    ASSERT_NE( Handle<Tag>::NULL_HANDLE, h3 );

    // loading tests
    manager.load( "rid1" );
    manager.load( "rid2", true );
    manager.load( "rid3", false );

    ASSERT_TRUE( factory.isLoaded( "value1" ) );
    ASSERT_TRUE( factory.isLoaded( "value2" ) );
    ASSERT_TRUE( factory.isLoaded( "value3" ) );
    ASSERT_TRUE( manager.isLoaded( "rid1" ) );
    ASSERT_TRUE( manager.isLoaded( "rid2" ) );
    ASSERT_TRUE( manager.isLoaded( "rid3" ) );

    ASSERT_STREQ( "value1", manager.dereference( h1 ).c_str() );
    ASSERT_STREQ( "value2", manager.dereference( h2 ).c_str() );
    ASSERT_STREQ( "value3", manager.dereference( h3 ).c_str() );

    // unloading tsts
    manager.unload( "rid2" );

    ASSERT_TRUE( factory.isLoaded( "value1" ) );
    ASSERT_FALSE( factory.isLoaded( "value2" ) );
    ASSERT_TRUE( factory.isLoaded( "value3" ) );
    ASSERT_TRUE( manager.isLoaded( "rid1" ) );
    ASSERT_FALSE( manager.isLoaded( "rid2" ) );
    ASSERT_TRUE( manager.isLoaded( "rid3" ) );

    ASSERT_STREQ( "value1", manager.dereference( h1 ).c_str() );
    ASSERT_STREQ( "value3", manager.dereference( h3 ).c_str() );

    manager.unload( "rid1" );
    manager.unload( "rid3" );

    manager.clean();

    ASSERT_TRUE( manager.hasHandle( "rid1" ) );
    ASSERT_TRUE( manager.hasHandle( "rid2" ) );
    ASSERT_TRUE( manager.hasHandle( "rid3" ) );

    ASSERT_FALSE( factory.isLoaded( "value1" ) );
    ASSERT_FALSE( factory.isLoaded( "value2" ) );
    ASSERT_FALSE( factory.isLoaded( "value3" ) );
    ASSERT_FALSE( manager.isLoaded( "rid1" ) );
    ASSERT_FALSE( manager.isLoaded( "rid2" ) );
    ASSERT_FALSE( manager.isLoaded( "rid3" ) );

    // releasing tests
    ASSERT_TRUE( manager.release( h1 ) );
    ASSERT_TRUE( manager.release( h2 ) );
    ASSERT_TRUE( manager.release( h3 ) );
    ASSERT_FALSE( manager.release( h1 ) );
    ASSERT_FALSE( manager.release( h2 ) );
    ASSERT_FALSE( manager.release( h3 ) );

    manager.clean();

    ASSERT_FALSE( manager.hasHandle( "rid1" ) );
    ASSERT_TRUE( manager.hasHandle( "rid2" ) );
    ASSERT_FALSE( manager.hasHandle( "rid3" ) );

}