// dynamic_array.h
#ifndef INCLUDED_DYNAMIC_ARRAY
#define INCLUDED_DYNAMIC_ARRAY

#include "../data/json_printer.h"
#include "../memory/allocator_guard.h"
#include "../memory/iallocator.h"
#include "../memory/mem.h"
#include <assert.h>
#include <stdexcept>

namespace StevensDev
{

namespace sgdc
{

template<typename T>
class DynamicArray
{
  private:
    sgdm::AllocatorGuard<T> d_allocator;
      // The allocator used when growing the array.

    T* d_array;
      // The internal array.

    unsigned int d_first;
      // The index of the first element in the array.

    unsigned int d_size;
      // The size of the array.

    unsigned int d_capacity;
      // The capacity of the array.

    // HELPER FUNCTIONS
    void grow();
      // Grows the array to twice the current size.
      // Doubles the size each time room is needed to avoid a large number of
      // re-allocations. If it is known ahead of time that a capacity of
      // greater than 32 will be needed then the pre-size constructor should
      // be used.
      // This reallocates the entire block when growing to avoid memory
      // fragmentation. This is to abide by the expectation that an array
      // be contiguous in memory. This reduces the number of cache clears
      // during iteration to at most one which is due to the wrapped nature of
      // the array.

    unsigned int wrap( int index ) const;
      // Wraps the index inside of the circular bounds.

    void shiftForward( unsigned int start, unsigned int count );
      // Shifts the given number of items forward one spot starting at the
      // given index.

    void shiftBack( unsigned int start, unsigned int count );
      // Shifts the given number of items back one spot starting at the given
      // index.

  public:
    // CONSTRUCTORS
    DynamicArray();
      // Constructs a new dynamic array using the default allocator.

    DynamicArray( unsigned int capacity );
      // Constructs a new dynamic array with a given initial capacity using
      // The default allocator.

    DynamicArray( sgdm::IAllocator<T>* allocator );
      // Constructs a new dynamic array using the given allocator.

    DynamicArray( sgdm::IAllocator<T>* allocation, unsigned int capacity );
      // Constructs a new dynamic array using the given allocator with the
      // given initial capacity.

    DynamicArray( const DynamicArray<T>& other );
      // Constructs a copy of the other dynamic array.

    DynamicArray( DynamicArray<T>&& source );
      // Moves the resources from the source to this instance.

    virtual ~DynamicArray();
      // Releases the dynamic array and all of its resources.

    // OPERATORS
    DynamicArray<T>& operator=( const DynamicArray<T>& other );
      // Makes this a copy of the other dynamic array.
      //
      // Requirements:
      // other is not this

    DynamicArray<T>& operator=( DynamicArray<T>&& source );
      // Moves the resources from the source to this instance.

    const T& operator[]( int index ) const;
      // Gets the element at the given index.
      //
      // Behavior is undefined when:
      // index is out of bounds

    T& operator[]( int index );
      // Sets the element at the given index.
      //
      // Behavior is undefined when:
      // index is out of bounds

    // MEMBER FUNCTIONS
    void push( const T& element );
      // Adds the element to the end of the collection and grows if necessary.

    void pushFront( const T& element );
      // Adds the element to the front of the collection and grows if
      // necessary.

    T pop();
      // Removes and retrieves the last element.
      //
      // Requirements:
      // array is not empty

    T popFront();
      // Removes and retrieves the first element.
      //
      // Requirements:
      // array is not empty

    T& at( unsigned int index ) const;
      // Gets the element at the given index.
      //
      // Throws runtime_error when:
      // index is out of bounds

    bool remove( const T& value );
      // Removes the first instance of the value from the array and returns
      // if it was found.

    T removeAt( unsigned int index );
      // Removes and retrieves the element at the given index.
      //
      // Throws runtime_error when:
      // index is out of bounds

    void insertAt( unsigned int index, const T& elem );
      // Inserts an element at the given index.
      //
      // Throws runtime_error when:
      // index is out of bounds

    bool doesContain( const T& value ) const;
      // Checks if the array contains the given value.

    unsigned int size() const;
      // Gets the size of the array.
};

// FREE OPERATORS
template<typename T>
inline
std::ostream& operator<<( std::ostream& stream,
                          const DynamicArray<T>& array )
{
    sgdd::JsonPrinter p( stream );
    p.open();
    p.print( "size", array.size() );
    p.printArr( "values", array, array.size() );
    p.close();
    return stream;
}

// CONSTRUCTORS
template<typename T>
DynamicArray<T>::DynamicArray() : d_allocator( nullptr ), d_array( nullptr ),
                                  d_first( 0 ), d_size( 0 ), d_capacity( 32 )
{
    d_array = d_allocator.get( d_capacity );
}

template <typename T>
DynamicArray<T>::DynamicArray( unsigned int capacity )
    : d_allocator( nullptr ), d_array( nullptr ),
      d_first( 0 ), d_size( 0 ), d_capacity( capacity )
{
    d_array = d_allocator.get( d_capacity );
}

template<typename T>
DynamicArray<T>::DynamicArray( sgdm::IAllocator<T>* allocator )
    : d_allocator( allocator ), d_array( nullptr ), d_first( 0 ), d_size( 0 )
    , d_capacity( 32 )
{
    d_array = d_allocator.get( d_capacity );
}

template<typename T>
DynamicArray<T>::DynamicArray( sgdm::IAllocator<T>* allocator,
                               unsigned int capacity )
    : d_allocator( allocator ), d_array( nullptr ), d_first( 0 ), d_size( 0 ),
      d_capacity( capacity )
{
    d_array = d_allocator.get( d_capacity );
}

template<typename T>
DynamicArray<T>::DynamicArray( const DynamicArray<T>& other )
    : d_allocator( other.d_allocator ), d_array( nullptr ),
      d_first( other.d_first ), d_size( other.d_size ),
      d_capacity( other.d_capacity )
{
    if ( other.d_array != nullptr )
    {
        // create copy of source array.
        d_array = d_allocator.get( d_capacity );
        sgdm::Mem::copy<T>( d_allocator.allocator(), d_array, other.d_array,
                            d_capacity );
    }
}

template<typename T>
DynamicArray<T>::DynamicArray( DynamicArray<T>&& source )
    : d_allocator( source.d_allocator ),
      d_array( source.d_array ), d_first( source.d_first ),
      d_size( source.d_size ), d_capacity( source.d_capacity )
{
    source.d_array = nullptr;
    source.d_first = 0;
    source.d_size = 0;
    source.d_capacity = 0;
}

template<typename T>
DynamicArray<T>::~DynamicArray()
{
    if ( d_array != nullptr )
    {
        d_allocator.release( d_array, d_capacity );
    }

    d_first = 0;
    d_size = 0;
    d_capacity = 0;
}

// OPERATORS
template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=( const DynamicArray<T>& other )
{
    assert( &other != this );

    if ( d_array != nullptr )
    {
        d_allocator.release( d_array, d_capacity );
    }

    d_allocator = other.d_allocator;
    d_first = other.d_first;
    d_size = other.d_size;
    d_capacity = other.d_capacity;

    if ( other.d_array != nullptr )
    {
        // create copy of source array
        d_array = d_allocator.get( d_capacity );
        sgdm::Mem::copy<>( d_array, other.d_array, d_capacity );
    }
    else
    {
        d_array = nullptr;
    }

    return *this;
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=( DynamicArray<T>&& source )
{
    if ( d_array != nullptr )
    {
        d_allocator.release( d_array, d_capacity );
    }

    d_allocator = source.d_allocator;
    d_array = source.d_array;
    d_first = source.d_first;
    d_size = source.d_size;
    d_capacity = source.d_capacity;

    source.d_array = nullptr;
    source.d_first = 0;
    source.d_size = 0;
    source.d_capacity = 0;

    return *this;
}

template<typename T>
const T& DynamicArray<T>::operator[]( int index ) const
{
    return d_array[wrap( ( unsigned int )index )];
}

template<typename T>
T& DynamicArray<T>::operator[]( int index )
{
    return d_array[wrap( ( unsigned int )index )];
}

// MEMBER FUNCTIONS
template<typename T>
void DynamicArray<T>::push( const T& element )
{
    if ( d_size >= d_capacity )
    {
        grow();
    }

    // assign next item in the array and increment size
    d_allocator.construct( d_array + wrap( d_size++ ),  element );
}

template<typename T>
void DynamicArray<T>::pushFront( const T& element )
{
    if ( d_size >= d_capacity )
    {
        grow();
    }

    // account for wrapping
    unsigned int prev = ( d_first == 0 ) ?
                        d_capacity - 1 :
                        d_first - 1;

    d_allocator.construct( d_array + prev, element );
    d_first = prev;
    ++d_size;
}

template<typename T>
T DynamicArray<T>::pop()
{
    assert( d_size > 0 );

    // get last and reduce size
    return d_array[wrap( --d_size )];
}

template<typename T>
T DynamicArray<T>::popFront()
{
    assert( d_size > 0 );

    int front = d_first;

    d_first = ( d_first + 1 ) % d_capacity;

    --d_size;

    return d_array[front];
}

template<typename T>
T& DynamicArray<T>::at( unsigned int index ) const
{
    if ( index >= d_size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    return d_array[wrap( index )];
}

template<typename T>
bool DynamicArray<T>::remove( const T& value )
{
    unsigned int i;
    for ( i = 0; i < d_size && d_array[i] != value; ++i )
    {
        // do nothing
    }

    if ( i >= d_size )
    {
        return false;
    }

    removeAt( i );

    return true;
}

template<typename T>
T DynamicArray<T>::removeAt( unsigned int index )
{
    if ( index >= d_size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    T elem = d_array[wrap( index )];
    shiftBack( index, d_size - index );
    --d_size;

    return elem;
}

template<typename T>
void DynamicArray<T>::insertAt( unsigned int index, const T& elem )
{
    if ( index > d_size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    if ( d_size >= d_capacity )
    {
        grow();
    }

    shiftForward( index, d_size - index );
    d_allocator.construct( d_array + wrap( index ), elem );
    ++d_size;
}

template<typename T>
inline
unsigned int DynamicArray<T>::size() const
{
    return d_size;
}

template <typename T>
bool DynamicArray<T>::doesContain( const T& value ) const
{
    unsigned int i;
    bool found;
    for ( i = 0, found = false; !found && i < d_size; ++i )
    {
        if ( ( *this )[i] == value )
        {
            found = true;
        }
    }

    return found;
}

// HELPER FUNCTIONS
template<typename T>
void DynamicArray<T>::grow()
{
    T* expanded = d_allocator.get( d_capacity * 2 );
    sgdm::Mem::clear<>( expanded, d_capacity * 2 );

    // account for wrapping
    if ( d_first + d_size > d_capacity )
    {
        unsigned int firstSize = ( d_capacity - d_first );
        unsigned int wrappedSize = ( d_size - firstSize );

        sgdm::Mem::copy<>( expanded, d_array + d_first, firstSize );
        sgdm::Mem::copy<>( expanded + firstSize, d_array, wrappedSize );
    }
    else
    {
        sgdm::Mem::copy<>( expanded, d_array + d_first, d_size );
    }

    d_allocator.release( d_array, d_capacity );

    d_first = 0;
    d_array = expanded;
    d_capacity *= 2;
}

template<typename T>
inline
unsigned int DynamicArray<T>::wrap( int index ) const
{
    return ( d_first + index ) % d_capacity;
}

template<typename T>
void DynamicArray<T>::shiftForward( unsigned int start, unsigned int count )
{
    int i;
    for ( i = count - 1; i >= 0; --i )
    {
        d_array[wrap( start + i + 1 )] = d_array[wrap( start + i )];
    }
}

template<typename T>
void DynamicArray<T>::shiftBack( unsigned int start, unsigned int count )
{
    unsigned  int i;
    for ( i = 0; i < count; ++i )
    {
        d_array[wrap( start + i )] = d_array[ wrap( start + i + 1 )];
    }
}

} // End nspc sgdc

} // End nspc StevensDev

#endif
