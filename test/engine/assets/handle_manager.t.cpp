// handle_manager.t.cpp
#include <engine/assets/handle_manager.h>
#include <engine/memory/allocator_guard.h>
#include <gtest/gtest.h>

struct Tag
{
};

TEST( HandleManagerTest, Construction )
{
    using namespace StevensDev::sgda;

    HandleManager<Tag, std::string> manager;
    HandleManager<Tag, std::string> copy( manager );

    copy = manager;
}

TEST( HandleManagerTest, HandleAcquisition )
{
    using namespace StevensDev::sgda;

    HandleManager<Tag, std::string> manager;

    // get first handle
    Handle<Tag> handle = manager.acquire();

    ASSERT_EQ( 0, handle.index() );
    ASSERT_EQ( 0, handle.checkValue() );

    // get second handle
    Handle<Tag> handle2 = manager.acquire();

    ASSERT_EQ( 1, handle2.index() );
    ASSERT_EQ( 0, handle2.checkValue() );

    // re-acquire first
    manager.release( handle );
    handle = manager.acquire();

    ASSERT_EQ( 0, handle.index() );
    ASSERT_EQ( 1, handle.checkValue() );

    // re-acquire both but reverse order
    manager.release( handle );
    manager.release( handle2 );
    handle2 = manager.acquire();
    handle = manager.acquire();

    ASSERT_EQ( 0, handle2.index() );
    ASSERT_EQ( 2, handle2.checkValue() );
    ASSERT_EQ( 1, handle.index() );
    ASSERT_EQ( 1, handle.checkValue() );

    ASSERT_TRUE( manager.has( handle ) );
    ASSERT_TRUE( manager.has( handle2 ) );

    manager.release( handle );
    manager.release( handle2 );

    ASSERT_FALSE( manager.has( handle ) );
    ASSERT_FALSE( manager.has( handle2 ) );
}

TEST( HandleManagerTest, DataManagement )
{
    using namespace StevensDev::sgda;
    using namespace StevensDev::sgdm;

    AllocatorGuard<std::string> alloc;
    HandleManager<Tag, std::string> manager;

    Handle<Tag> handle = manager.acquire();
    std::string* data = alloc.get( 1 );

    manager.set( handle, data );

    ASSERT_TRUE( manager.hasData( handle ) );

    ASSERT_EQ( data, manager.get( handle ) );

    ASSERT_EQ( data, manager.release( handle ) );

    ASSERT_FALSE( manager.hasData( handle ) );

    ASSERT_EQ( nullptr, manager.get( handle ) );

    alloc.release( data, 1 );
}