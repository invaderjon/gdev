// stack_guard.h
#ifndef INCLUDED_STACK_GUARD
#define INCLUDED_STACK_GUARD

#include "../data/json_printer.h"
#include "allocator_guard.h"
#include <assert.h>
#include "iallocator.h"

namespace StevensDev
{

namespace sgdm
{

template<typename T>
class StackGuard
{
  private:
    AllocatorGuard<T> d_allocator;
      // The allocator to release the memory to

    T* d_guarded;
      // The pointer to the guarded memory allocation.

    unsigned int d_count;
      // The number of items that are being guarded.

    // CONSTRUCTORS
    StackGuard( const StackGuard<T>& guard );
      // Constructs a copy of the given guard.

    // OPERATORS
    StackGuard<T>& operator=( const StackGuard<T>& guard );
      // Makes this guard a copy of the other guard.

  public:
    // CONSTRUCTORS
    StackGuard();
      // Constructs an invalid stack guard.

    StackGuard( IAllocator<T>* alloc );
      // Constructs a stack guard for the given allocator.

    StackGuard( IAllocator<T>* alloc, T* guarded );
      // Constructs a stack guard for the given memory allocation.

    StackGuard( IAllocator<T>* alloc, T* guarded, unsigned int count );
      // Constructs a stack guard for the given number of items at given
      // memory allocation.

    StackGuard( StackGuard<T>&& guard );
      // Moves the given guard to the new object.

    ~StackGuard();
      // Destructs the stack guard and releases the guarded
      // memory.

    // OPERATORS
    StackGuard<T>& operator=( StackGuard<T>&& guard );
      // Moves the given guard to here.
      //
      // Requirements:
      // Not currently guarding another allocation

    bool operator==( bool rhs );
      // Checks if the right hand side is equal to the underlying pointer.

    bool operator!=( bool rhs );
      // Checks if the right hand side is not equal to the underyling pointer.

    bool operator!();
      // Performs the not operator on the underlying pointer.

    T* operator->() const;
      // Dereferences the guarded content.
      //
      // Behavior is undefined when:
      // There is no guarded allocation.

    T& operator*() const;
      // Dereferences the guarded content.
      //
      // Behavior is undefined when:
      // There is no guarded allocation.
};

// FREE OPERATORS
template<typename T>
inline
std::ostream& operator<<( std::ostream& stream,
                          const StackGuard<T>& guard )
{
    sgdd::JSONPrinter p( stream );

    p.open();
    p.print( "guarding", *guard );
    p.close();

    return stream;
}

// CONSTRUCTORS
template<typename T>
inline
StackGuard<T>::StackGuard() : d_allocator( nullptr ), d_guarded( nullptr ),
                              d_count( 0 )
{
}

template<typename T>
StackGuard<T>::StackGuard( IAllocator<T>* alloc ) : d_allocator( alloc )
{
}

template<typename T>
StackGuard<T>::StackGuard( IAllocator<T>* alloc, T* guarded )
    : d_allocator( alloc ), d_guarded( guarded ), d_count( 1 )
{

}

template<typename T>
inline
StackGuard<T>::StackGuard( IAllocator<T>* allocator, T* guarded,
                           unsigned int count )
    : d_allocator( allocator ), d_guarded( guarded ), d_count( count )
{
}

template<typename T>
inline
StackGuard<T>::StackGuard( const sgdm::StackGuard<T>& guard )
    : d_guarded( guard.d_guarded )
{
}

template<typename T>
inline
StackGuard<T>::StackGuard( sgdm::StackGuard<T>&& guard )
    : d_allocator( guard.d_allocator ), d_guarded( guard.d_guarded )
{
    guard.d_guarded = nullptr;
}

template<typename T>
StackGuard<T>::~StackGuard()
{
    if ( d_guarded == nullptr )
    {
        return;
    }

    assert( d_count > 0 );

    d_allocator.release( d_guarded, d_count );

    d_guarded = nullptr;
}

// OPERATORS
template<typename T>
inline
StackGuard<T>& StackGuard<T>::operator=( const sgdm::StackGuard<T>& guard )
{
    assert( d_guarded == nullptr );
    d_guarded = guard.d_guarded;

    return *this;
}

template<typename T>
inline
StackGuard<T>& StackGuard<T>::operator=( sgdm::StackGuard<T>&& guard )
{
    assert( d_guarded == nullptr );
    d_guarded = guard.d_guarded;

    guard.d_guarded = nullptr;

    return *this;
}

template<typename T>
inline
bool StackGuard<T>::operator==( bool rhs )
{
    return bool( d_guarded ) == rhs;
}

template<typename T>
inline
bool StackGuard<T>::operator!=( bool rhs )
{
    return bool( d_guarded ) != rhs;
}

template<typename T>
inline
bool StackGuard<T>::operator!()
{
    return d_guarded == nullptr;
}

template <typename T>
inline
T* StackGuard<T>::operator->() const
{

    return d_guarded;
}

template<typename T>
inline
T& StackGuard<T>::operator*() const
{
    return *d_guarded;
}

} // End nspc sgdm

} // End nspc StevensDev

#endif
