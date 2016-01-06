// set.h
#ifndef INCLUDED_SET
#define INCLUDED_SET

#include "engine/containers/map.h"

namespace StevensDev
{

namespace sgdc
{

template <typename T>
class Set
{
  private:
    // MEMBERS
    Map<T, T> d_map;

  public:
    // CONSTRUCTORS
    Set();
      // Constructs a new set using the default allocator.
      // This will use the default Hasher specialization as its hash function.

    Set( const typename Map<T, T>::HashFunc& hashFunc );
      // Constructs a new set set using the default allocator and given hash
      // function.

    Set( unsigned int capacity );
      // Constructs a new set with the given capacity and default allocator.
      // This will use the default Hasher specialization as its hash function.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Set( unsigned int capacity, const typename Map<T, T>::HashFunc& hashFunc );
      // Constructs a new set with the given capacity and hash function using
      // the default allocator.

    Set( sgdm::IAllocator<T>* allocator );
      // Constructs a new set using the given allocator.
      // This will use the default Hasher specialization as its hash function.

    Set( sgdm::IAllocator<T>* valueAlloc,
         sgdm::IAllocator<Bin>* binAlloc,
         sgdm::IAllocator<sgdu::HashCode>* hashAlloc );
      // Constructs a new set using the given allocators.
      // This will use the default Hasher specialization as its hash function.

    Set( sgdm::IAllocator<T>* allocator,
         const typename Map<T, T>::HashFunc& hashFunc );
      // Constructs a new set using the given allocator and hash function.

    Set( sgdm::IAllocator<T>* valueAlloc,
         sgdm::IAllocator<Bin>* binAlloc,
         sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
         const typename Map<T, T>::HashFunc& hashFunc );
      // Constructs a new set using the given allocators and hash function.

    Set( sgdm::IAllocator<T>* allocator, unsigned int capacity );
      // Constructs a new set with the given capacity and allocator.
      // This will use the default Hasher specialization as its hash function.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Set( sgdm::IAllocator<T>* valueAlloc,
         sgdm::IAllocator<Bin>* binAlloc,
         sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
         unsigned int capacity );
      // Constructs a new set with the given capacity and allocators.
      // This will use the default Hasher specialization as its hash function.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Set( sgdm::IAllocator<T>* allocator, unsigned int capacity,
         const typename Map<T, T>::HashFunc& hashFunc );
      // Constructs a new set with the given capacity, hash function,
      // and allocator.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Set( sgdm::IAllocator<T>* valueAlloc,
         sgdm::IAllocator<Bin>* binAlloc,
         sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
         unsigned int capacity, const typename Map<T, T>::HashFunc& hashFunc );
      // Constructs a new set with the given capacity, hash function,
      // and allocators.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Set( const Set<T>& other );
      // Constructs a copy of the other set.

    Set( Set<T>&& source );
      // Moves the resources from the source to this instance.

    virtual ~Set();
      // Releases the set and all of its resources.

    // OPERATORS
    Set<T>& operator=( const Set<T>& other );
      // Makes this a copy of the other set.

    Set<T>& operator=( Set<T>&& source );
      // Moves the resources from the source to this instance.

    const T& operator[]( unsigned int index ) const;
      // Gets the value at the given index.
      //
      // Note:
      // There is no correlation between values and their position. This
      // should only be used when iterating through the entire set.
      //
      // Requirements:
      // The index is in bounds.

    // MEMBER FUNCTIONS
    void add( const T& value );
      // Adds the value to the set.
      // If the value is already in the set it'll be re-written.

    T remove( const T& value );
      // Removes the value from the set.
      //
      // Requirements:
      // The value is a part of the set.

    bool has( const T& value ) const;
      // Checks if the value is a part of the set.

    unsigned int size() const;
      // Gets the size of the set.

    bool isEmpty() const;
      // Checks if the set is empty.

    const DynamicArray<T>& values() const;
      // Gets the values as an array.
};

// FREE OPERATORS
template <typename T>
inline
std::ostream& operator<<( std::ostream& stream, const Set<T>& set )
{
    return stream << set.values();
}

// CONSTRUCTORS
template <typename T>
inline
Set<T>::Set() : d_map()
{
}

template <typename T>
inline
Set<T>::Set( const typename Map<T, T>::HashFunc& hashFunc ) : d_map( hashFunc )
{
}

template <typename T>
inline
Set<T>::Set( unsigned int capacity ) : d_map( capacity )
{
}

template <typename T>
inline
Set<T>::Set( unsigned int capacity,
             const typename Map<T, T>::HashFunc& hashFunc )
        : d_map( capacity, hashFunc )
{
}

template <typename T>
inline
Set<T>::Set( sgdm::IAllocator<T>* allocator ) : d_map( allocator )
{
}

template <typename T>
inline
Set<T>::Set( sgdm::IAllocator<T>* valueAlloc,
             sgdm::IAllocator<Bin>* binAlloc,
             sgdm::IAllocator<sgdu::HashCode>* hashAlloc )
        : d_map( valueAlloc, valueAlloc, binAlloc, hashAlloc )
{
}

template <typename T>
inline
Set<T>::Set( sgdm::IAllocator<T>* allocator,
             const typename Map<T, T>::HashFunc& hashFunc )
        : d_map( allocator, hashFunc )
{
}

template <typename T>
inline
Set<T>::Set( sgdm::IAllocator<T>* valueAlloc,
             sgdm::IAllocator<Bin>* binAlloc,
             sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
             const typename Map<T, T>::HashFunc& hashFunc )
        : d_map( valueAlloc, valueAlloc, binAlloc, hashAlloc, hashFunc )
{
}

template <typename T>
inline
Set<T>::Set( sgdm::IAllocator<T>* allocator, unsigned int capacity )
        : d_map( allocator, capacity )
{
}

template <typename T>
inline
Set<T>::Set( sgdm::IAllocator<T>* valueAlloc,
             sgdm::IAllocator<Bin>* binAlloc,
             sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
             unsigned int capacity )
        : d_map( valueAlloc, valueAlloc, binAlloc, hashAlloc, capacity )
{
}

template <typename T>
inline
Set<T>::Set( sgdm::IAllocator<T>* allocator, unsigned int capacity,
             const typename Map<T, T>::HashFunc& hashFunc )
        : d_map( allocator, capacity, hashFunc )
{
}

template <typename T>
inline
Set<T>::Set( sgdm::IAllocator<T>* valueAlloc,
             sgdm::IAllocator<Bin>* binAlloc,
             sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
             unsigned int capacity,
             const typename Map<T, T>::HashFunc& hashFunc )
        : d_map( valueAlloc, valueAlloc, binAlloc, hashAlloc, capacity,
                 hashFunc )
{
}

template <typename T>
inline
Set<T>::Set( const sgdc::Set<T>& other ) : d_map( other.d_map )
{
}

template <typename T>
inline
Set<T>::Set( sgdc::Set<T>&& source ) : d_map( std::move( source.d_map ) )
{
}

template <typename T>
inline
Set<T>::~Set()
{
}

// OPERATORS
template <typename T>
inline
Set<T>& Set<T>::operator=( const sgdc::Set<T>& other )
{
    d_map = other.d_map;

    return *this;
}

template <typename T>
inline
Set<T>& Set<T>::operator=( sgdc::Set<T>&& source )
{
    d_map = std::move( source.d_map );

    return *this;
}

template <typename T>
inline
const T& Set<T>::operator[]( unsigned int index ) const
{
    return d_map.values()[index];
}

// MEMBER FUNCTIONS
template <typename T>
inline
void Set<T>::add( const T& value )
{
    d_map[value] = value;
}

template <typename T>
inline
T Set<T>::remove( const T& value )
{
    return d_map.remove( value );
}

template <typename T>
inline
bool Set<T>::has( const T& value ) const
{
    return d_map.has( value );
}

template <typename T>
inline
unsigned int Set<T>::size() const
{
    return d_map.values().size();
}

template <typename T>
inline
bool Set<T>::isEmpty() const
{
    return d_map.values().size() <= 0;
}

template <typename T>
inline
const DynamicArray<T>& Set<T>::values() const
{
    return d_map.values();
}

} // End nspc sgdc

} // End nspc StevensDev

#endif
