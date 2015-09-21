// mem.h
#ifndef INCLUDED_MEM
#define INCLUDED_MEM

#include <bits/move.h>
#include "iallocator.h"

namespace StevensDev
{

namespace sgdm
{

struct Mem
{
    // GLOBAL HELPER FUNCTIONS
    template<typename T>
    static void copy( T* dst, T* src, unsigned int count );
      // Copies the given number of items from the source to the destination
      // using assignment.
      //
      // Requirements:
      // dst and src are not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // dst or src is invalid
      // count is larger than the array

    template<typename T>
    static void copy( IAllocator<T>* alloc, T* dst, T* src,
                      unsigned int count );
      // Copies the given number of items from the source to the destination
      // using construction.
      //
      // Requirements:
      // alloc, dst, and src are not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // alloc, dst, or src is invalid
      // count is larger than the array

    template<typename T>
    static void move( T* dst, T* src, unsigned int count );
      // Moves the given number of items from the source to the destination
      // using assignment.
      //
      // Requirements:
      // dst and src are not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // dst or src is invalid
      // count is larger than the array

    template<typename T>
    static void move( IAllocator<T>* alloc, T* dst, T* src,
                      unsigned int count );
      // Moves the given number of items from the source to the destination
      // using construction.
      //
      // Requirements:
      // alloc, dst, and src are not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // alloc, dst, or src is invalid
      // count is larger than the array

    template<typename T>
    static void set( T* arr, const T& value, unsigned int count );
      // Sets the elements in the array to the given value using assignment.
      //
      // Requirements:
      // arr is not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // arr is invalid
      // count is larger than the array

    template<typename T>
    static void set( IAllocator<T>* alloc, T* arr, const T& value,
                      unsigned int count );
      // Sets the elements in the array to the given value using construction.
      //
      // Requirements:
      // alloc and arr are not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // alloc or arr is invalid
      // count is larger than the array

    template<typename T>
    static void clear( T* arr, unsigned int count );
      // Clears the elements in the array to the default constructed item
      // using assignment.
      //
      // Requirements:
      // arr is not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // arr is invalid
      // count is larger than the array

    template<typename T>
    static void clear( IAllocator<T>* alloc, T* arr, unsigned int count );
      // Clears the elements in the array to the default constructed item
      // using construction.
      //
      // Requirements:
      // alloc and arr are not equal to nullptr (zero)
      //
      // Behavior is undefined when:
      // alloc or arr is invalid
      // count is larger than the array
};

// GLOBAL HELPER FUNCTIONS
template<typename T>
inline
void Mem::copy( T* dst, T* src, unsigned int count )
{
    while ( count-- )
    {
        dst[count] = src[count];
    }
}

template<typename T>
inline
void Mem::copy( IAllocator<T>* alloc, T* dst, T* src, unsigned int count )
{
    while ( count-- )
    {
        alloc->construct( dst + count, src[count] );
    }
}

template<typename T>
inline
void Mem::move( T* dst, T* src, unsigned int count )
{
    while ( count-- )
    {
        dst[count] = std::move( src[count] );
    }
}

template<typename T>
inline
void Mem::move( IAllocator<T>* alloc, T* dst, T* src, unsigned int count )
{
    while ( count-- )
    {
        alloc->construct( dst + count, std::move( src[count] ) );
    }
}

template<typename T>
inline
void Mem::set( T* arr, const T& value, unsigned int count )
{
    while ( count-- )
    {
        arr[count] = value;
    }
}

template<typename T>
inline
void Mem::set( IAllocator<T>* alloc, T* arr, const T& value,
               unsigned int count )
{
    while ( count-- )
    {
        alloc->construct( arr + count, value );
    }
}

template<typename T>
inline
void Mem::clear( T* arr, unsigned int count )
{
    T t;
    set( arr, t, count );
}

template<typename T>
inline
void Mem::clear( IAllocator<T>* alloc, T* arr, unsigned int count )
{
    T t;
    set( alloc, arr, t, count );
}

} // End nspc sgdm

} // End nspc StevensDev

#endif