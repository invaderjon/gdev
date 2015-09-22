// default_allocator.t.cpp
#include "engine/memory/default_allocator.h"
#include <gtest/gtest.h>

TEST( DefaultAllocatorTest, Construction )
{
    using namespace StevensDev::sgdm;

    // primitive template
    DefaultAllocator<int> alloc;
    DefaultAllocator<int> copy( alloc );

    DefaultAllocator<std::string> complexAlloc;
    DefaultAllocator<std::string> complexOther( complexAlloc );
}

TEST( DefaultAllocatorTest, Allocation )
{
    using namespace StevensDev::sgdm;

    DefaultAllocator<int> alloc;

    const int count = 500;
    int* ptr = alloc.get( count );
    alloc.release( ptr, count );
}

TEST( DefaultAllocatorTest, Print )
{
    using namespace StevensDev::sgdm;

    DefaultAllocator<int> alloc;

    std::ostringstream oss;
    ASSERT_NO_FATAL_FAILURE( oss << alloc );
}