// allocator_guard.h
#ifndef INCLUDED_ALLOCATOR_GUARD
#define INCLUDED_ALLOCATOR_GUARD

#include "default_allocator.h"
#include "iallocator.h"

namespace StevensDev
{

namespace sgdm
{

template<typename T>
class AllocatorGuard : public IAllocator<T>
{
  private:
    DefaultAllocator<T> d_default;
      // The default allocator that is used if none is provided.

    IAllocator<T>* d_allocator;
      // The underlying allocator.

  public:
    // CONSTRUCTORS
    AllocatorGuard();
      // Constructs a wrapper that uses the default allocator.

    AllocatorGuard( IAllocator<T>* allocator );
      // Constructs a wrapper that uses the given allocator.

    AllocatorGuard( const AllocatorGuard<T>& other );
      // Constructs a copy of the given wrapper.

    ~AllocatorGuard();
      // Deconstructs the wrapper.

    // OPERATORS
    AllocatorGuard<T>& operator=( const AllocatorGuard<T>& other );
      // Sets this to be a copy of the other wrapper.

    // MEMBER FUNCTIONS
    virtual T* get( int count );
      // Gets a memory allocation for count instances of the given type.
      //
      // Behavior is undefined when:
      // T is void
      // count is less than or equal to zero
      // out of memory

    virtual void release( T* pointer, int count );
      // Releases the memory allocation at the given pointer location.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is
      // equal to nullptr (zero) or is invalid
      // count is less than or equal to zero

    virtual void construct( T* pointer, const T& copy );
      // Constructs the object in place using the copy constructor.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid

    virtual void construct( T* pointer, T&& copy );
      // Constructs the object in place using the move constructor.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid

    virtual void destruct( T* pointer );
      // Call the destructor on an object.
      //
      // Behavior is undefined when:
      // T is void
      // pointer is equal to nullptr (zero) or is invalid

    IAllocator<T>* allocator();
      // Gets the allocator used by the guard.
      // This pointer is only valid while the guard instance exists.
};

// CONSTRUCTORS
template<typename T>
inline
AllocatorGuard<T>::AllocatorGuard() : d_default(), d_allocator( &d_default )
{
}

template<typename T>
inline
AllocatorGuard<T>::AllocatorGuard( IAllocator<T>* allocator )
{
    if ( allocator != nullptr )
    {
        d_allocator = allocator;
    }
    else
    {
        d_allocator = &d_default;
    }
}

template<typename T>
inline
AllocatorGuard<T>::AllocatorGuard( const sgdm::AllocatorGuard<T>& other )
    : d_default()
{
    if ( other.d_allocator != &other.d_default )
    {
        d_allocator = other.d_allocator;
    }
    else
    {
        d_allocator = &d_default;
    }
}

template<typename T>
inline
AllocatorGuard<T>::~AllocatorGuard()
{
}

// OPERATORS
template<typename T>
inline
AllocatorGuard<T>& AllocatorGuard<T>::operator=(
    const sgdm::AllocatorGuard<T>& other )
{
    if ( other.d_allocator != &other.d_default )
    {
        d_allocator = other.d_allocator;
    }
    else
    {
        d_allocator = &d_default;
    }
}

// MEMBER FUNCTIONS
template<typename T>
T* AllocatorGuard<T>::get( int count )
{
    return d_allocator->get( count );
}

template<typename T>
void AllocatorGuard<T>::release( T* pointer, int count )
{
    d_allocator->release( pointer, count );
}

template<typename T>
void AllocatorGuard<T>::construct( T* pointer, const T& copy )
{
    d_allocator->construct( pointer, copy );
}

template<typename T>
void AllocatorGuard<T>::construct( T* pointer, T&& copy )
{
    d_allocator->construct( pointer, copy );
}

template<typename T>
void AllocatorGuard<T>::destruct( T* pointer )
{
    d_allocator->destruct( pointer );
}

template<typename T>
IAllocator<T>* AllocatorGuard<T>::allocator()
{
    return d_allocator;
}

} // End nspc sgdm

} // End nspc StevensDev

#endif
