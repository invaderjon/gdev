// counting_allocator.h
#ifndef INCLUDED_COUNTING_ALLOCATOR
#define INCLUDED_COUNTING_ALLOCATOR

#include "../data/json_printer.h"
#include <assert.h>
#include "default_allocator.h"
#include <iostream>

namespace StevensDev
{

namespace sgdm
{

template<typename T>
class CountingAllocator : public DefaultAllocator<T>
{
  private:
    static int d_totalAllocationCount;
      // The total number of allocations across all instances of this class.

    static int d_totalReleaseCount;
      // The total number of allocations across all instances of this class.

  public:
    // CONSTRUCTORS
    CountingAllocator();
      // Constructs a new allocator that tracks the number of allocators.

    CountingAllocator( const CountingAllocator<T>& other );
      // Constructs a copy of the given counting allocator.

    virtual ~CountingAllocator();
      // The virtual destructor.

    // OPERATORS
    CountingAllocator<T>& operator=( const CountingAllocator<T>& other );
      // Makes this a copy of the other allocator.

    // MEMBER FUNCTIONS
    virtual T* get( int count );
      // Gets a memory allocation for count instances of the given type using
      // the new[] function.
      //
      // Requirements:
      // count is greater than zero
      //
      // Behavior is undefined when:
      // Out of memory
      // T is void

    virtual void release( T* pointer, int count );
      // Releases the memory allocation at the given pointer location using
      // the release[] function.
      //
      // Requirements:
      // pointer is not equal to nullptr (zero)
      // count is greater than zero
      //
      // Behavior is undefined when:
      // T is void
      // pointer is otherwise invalid

    // GLOBAL FUNCTIONS
    static int getTotalAllocationCount();
      // Gets the total number of allocations for all instances of this class.

    static int getTotalReleaseCount();
      // Gets the total number of released allocations across all instances of
      // this class.

    static int getOutstandingCount();
      // Gets the number of unreleased allocations across all instances of
      // this class.
};

// GLOBALS
template<typename T>
int CountingAllocator<T>::d_totalAllocationCount = 0;

template<typename T>
int CountingAllocator<T>::d_totalReleaseCount = 0;

// FREE OPERATORS
template<typename T>
inline
std::ostream& operator<<( std::ostream& stream,
                          const CountingAllocator<T>& allocator )
{
    return stream << "{ }";
}

// CONSTRUCTORS
template<typename T>
inline
CountingAllocator<T>::CountingAllocator()
{
}

template<typename T>
inline
CountingAllocator<T>::CountingAllocator( const CountingAllocator<T>& other )
{
}

template<typename T>
inline
CountingAllocator<T>::~CountingAllocator()
{
}

// MEMBER OPERATORS
template<typename T>
inline
CountingAllocator<T>& CountingAllocator<T>::operator=(
    const CountingAllocator<T>& other )
{
    return *this;
}

// MEMBER FUNCTIONS
template<typename T>
inline
T* CountingAllocator<T>::get( int count )
{
    assert( count > 0 );

    CountingAllocator<T>::d_totalAllocationCount += count;

    return new T[count];
}

template<typename T>
inline
void CountingAllocator<T>::release( T* ptr, int count )
{
    assert( ptr != nullptr );
    assert( count > 0 );

    CountingAllocator<T>::d_totalReleaseCount += count;

    delete[] ptr;
}

// GLOBAL FUNCTIONS
template<typename T>
inline
int CountingAllocator<T>::getTotalAllocationCount()
{
    return d_totalAllocationCount;
}

template<typename T>
inline
int CountingAllocator<T>::getTotalReleaseCount()
{
    return d_totalReleaseCount;
}

template<typename T>
inline
int CountingAllocator<T>::getOutstandingCount()
{
    return d_totalAllocationCount - d_totalReleaseCount;
}

} // End nspc sgdm

} // End nspc StevensDev

#endif
