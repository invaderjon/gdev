// default_allocator.h
#ifndef INCLUDED_DEFAULT_ALLOCATOR
#define INCLUDED_DEFAULT_ALLOCATOR

#include <assert.h>
#include "iallocator.h"
#include <iostream>

namespace StevensDev
{

namespace sgdm
{

template<typename T>
class DefaultAllocator : public IAllocator<T>
{
  public:
    // CONSTRUCTORS
    DefaultAllocator();
      // Constructs a new default allocator that wraps the new and delete
      // functions.

    DefaultAllocator( const DefaultAllocator<T>& other );
      // Constructs a copy of the given default allocator.

    virtual ~DefaultAllocator();
      // The virtual destructor.

    // OPERATORS
    DefaultAllocator<T>& operator=( const DefaultAllocator<T>& other );
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
      // out of memory
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
      // pointer does not reference an allocated block of memory.

    virtual void construct( T* pointer, const T& copy );
      // Constructs the object in place using the copy constructor.
      //
      // Requirements:
      // pointer is not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid

    virtual void construct( T* pointer, T&& copy );
      // Constructs the object in place using the move constructor.
      //
      // Requirements:
      // pointer is not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid


    virtual void destruct( T* pointer );
      // Call the destructor on an object.
      //
      // Requirements:
      // pointer is not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // T is void
};

// FREE OPERATORS
template<typename T>
inline
std::ostream &operator<<( std::ostream &stream,
                          const DefaultAllocator<T> &allocator )
{
    return stream << "{ }";
}

// CONSTRUCTORS
template<typename T>
inline
DefaultAllocator<T>::DefaultAllocator()
{
}

template<typename T>
inline
DefaultAllocator<T>::DefaultAllocator( const DefaultAllocator<T> &other )
{
}

template<typename T>
inline
DefaultAllocator<T>::~DefaultAllocator()
{
}

// OPERATORS
template<typename T>
inline
DefaultAllocator<T>& DefaultAllocator<T>::operator=(
    const DefaultAllocator<T> &other )
{
    return *this;
}

// MEMBER FUNCTIONS
template<typename T>
inline
T* DefaultAllocator<T>::get( int count )
{
    assert( count > 0 );

    return new T[count];
}

template<typename T>
inline
void DefaultAllocator<T>::release( T* ptr, int count )
{
    assert( ptr != nullptr );
    assert( count > 0 );

    delete[] ptr;
}

template<typename T>
inline
void DefaultAllocator<T>::construct( T* ptr, const T& copy )
{
    assert( ptr != nullptr );

    new ( ptr ) T( copy );
}

template<typename T>
void DefaultAllocator<T>::construct( T* ptr, T&& copy )
{
    assert( ptr != nullptr );

    new ( ptr ) T( std::move( copy ) );
}

template<typename T>
inline
void DefaultAllocator<T>::destruct( T* ptr )
{
    assert( ptr != nullptr );

    ptr->~T();
}

} // End nspc sgdm

} // End nspc StevensDev

#endif
