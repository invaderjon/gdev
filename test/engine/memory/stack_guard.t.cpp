// stack_guard.t.cpp
#include "engine/memory/stack_guard.h"
#include <gtest/gtest.h>
#include <engine/memory/counting_allocator.h>

TEST( StackGuardTest, Construction )
{
    using namespace StevensDev::sgdm;

    CountingAllocator<std::string> alloc;

    ASSERT_NO_FATAL_FAILURE (
        StackGuard<std::string> guard( &alloc, alloc.get( 1 ) ) );
}

TEST( StackGuardTest, Operators )
{
    using namespace StevensDev::sgdm;

    CountingAllocator<std::string> alloc;

    StackGuard<std::string> guard( &alloc, alloc.get( 1 ) );
    alloc.construct( &( *guard ), "test" );

    ASSERT_TRUE( guard != nullptr );
    ASSERT_FALSE( guard == nullptr );
    ASSERT_FALSE( !guard );
    ASSERT_NO_THROW( guard->c_str() );
    ASSERT_NO_THROW( *guard );

    ASSERT_STREQ( "test", guard->c_str() );
}

TEST( StackGuardTest, Print )
{
    using namespace StevensDev::sgdm;

    CountingAllocator<std::string> alloc;

    StackGuard<std::string> guard( &alloc, alloc.get( 1 ) );
    alloc.construct( &( *guard ), "test" );

    std::ostringstream oss;

    oss << guard;

    ASSERT_STREQ( "{ \"guarding\": \"test\" }", oss.str().c_str() );
}