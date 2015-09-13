// counting_allocator.t.cpp
#include "engine/memory/counting_allocator.h"
#include <gtest/gtest.h>

TEST( CountingAllocatorTest, Construction )
{
    using namespace StevensDev::sgdm;

    // primitive template
    CountingAllocator<int> allocator;
    CountingAllocator<int> copy( allocator );

    // complex template
    CountingAllocator<std::string> complexAllocator;
    CountingAllocator<std::string> complexOther( complexAllocator );
}

TEST( CountingAllocatorTest, Assignment )
{
    using namespace StevensDev::sgdm;

    CountingAllocator<int> allocator;
    CountingAllocator<int> other;

    allocator = other;
}

TEST( CountingAllocatorTest, Alocation )
{
    using namespace StevensDev::sgdm;

    CountingAllocator<int> allocator;

    const int count = 500;
    int* ptr = allocator.get( count );

    EXPECT_NO_FATAL_FAILURE( ptr[0] = 1024 );
    EXPECT_NO_FATAL_FAILURE( ptr[499] = 1024 );

    EXPECT_EQ( allocator.getTotalAllocationCount(), count );
    EXPECT_EQ( allocator.getOutstandingCount(), count );

    allocator.release( ptr, count );

    EXPECT_EQ( allocator.getTotalReleaseCount(), count );
    EXPECT_EQ( allocator.getOutstandingCount(), 0 );
}