// map.h
#ifndef INCLUDED_MAP
#define INCLUDED_MAP

#include <assert.h>
#include "engine/containers/dynamic_array.h"
#include "engine/data/json_printer.h"
#include "engine/memory/allocator_guard.h"
#include "engine/memory/iallocator.h"
#include "engine/memory/mem.h"
#include "engine/util/hasher.h"
#include <functional>
#include <search.h>
#include <string>

namespace StevensDev
{

namespace sgdc
{

// TYPES
typedef int Bin;
  // Defines a bin. The bin value is the index of the value in the array.
  // Only positive values are valid where-as certain negative values have
  // special meanings.

template <typename T, typename K = std::string>
class Map
{
  public:
    // TYPES
    typedef std::function<sgdu::HashCode( const K& )> HashFunc;
      // Defines a function that computes the hash of a given key.

  private:
    // CONSTANTS
    static constexpr Bin BIN_EMPTY = -1;
      // Defines a bin that is empty.

    static constexpr Bin BIN_DELETED = -2;
      // Defines a bin that was deleted.

    static constexpr Bin BIN_INVALID = -3;
      // Defines a bin that is invalid.

    static constexpr unsigned int GROW_THRESHOLD = 75;
      // Grows once more than 75% full.

    static constexpr unsigned int SHRINK_THRESHOLD = 30;
      // Shrink once less than 25% full.

    static constexpr unsigned int MIN_TRANSFER = 16;
      // The minimum number of items that must be transferred with each
      // progressive copy pass.

    static constexpr unsigned int MIN_BINS = 32;
      // The minimum number of bins.

    // MEMBERS
    sgdm::AllocatorGuard<Bin> d_binAllocator;
      // The allocator used for obtaining new bins.

    DynamicArray<K> d_keys;
      // The set of keys.

    DynamicArray<T> d_values;
      // The set of values.

    DynamicArray<sgdu::HashCode> d_entries;
      // The set of pre-hashed key entries. This is maintained to avoid the
      // overhead of rehashing the keys during every equals operation.

    HashFunc d_hashFunc;
      // The key hash function.

    Bin* d_bins;
      // The set of available bins.

    Bin* d_oldBins;
      // The set of old bins. This is maintained to avoid moving all of the
      // entries at once. Instead entries are moved as they are accessed until
      // there are no more full bins, at which point the old bins are
      // released. If there are no old bins then this is equal to nullptr
      // (zero).

    unsigned int d_binsInUse;
    // The number of bins currently in use.

    unsigned int d_binCount;
      // The number of available bins.

    unsigned int d_oldBinIndex;
      // The index of the last checked item in entries. If this is equal to
      // or greater than d_oldBinCount then there are no more items to check.

    unsigned int d_oldBinCount;
      // The number of old bins.

    // HELPER FUNCTIONS
    void grow();
      // Grows the map to twice the current capacity.

    void shrink();
      // Shrinks the map to half the current capacity.

    void reduceOldBins();
      // Copy some more items over from the old bin set to the new bin set.
      // This will release the old bins if there are non remaining.

    int find( const sgdu::HashCode& code, Bin* bins,
              unsigned int count ) const;
      // Checks for the the specified hash in the specified set of bins and
      // gets the index of the bin that contains it.
      // If it is not found this will return BIN_INVALID.

    int probe( const sgdu::HashCode& code, Bin* bins,
               unsigned int count ) const;
      // Probes for an available bin for the given hash in the specified set of
      // bins. If one is found this returns its index, otherwise this
      // returns BIN_INVALID.

    unsigned int jump( unsigned int probes ) const;
      // Returns how many bins should be jumped while probing.
      //
      // The implementation is a linear (returns the input value).

    unsigned int wrap( int index, unsigned int count ) const;
      // Wraps the index to the specified bounds.

    unsigned int getCopyCount( unsigned int count,
                               unsigned int oldCount ) const;
      // Determines how many items should be copied when transfering to a new
      // memory region.

    bool doesContain( const Bin& bin, const sgdu::HashCode& code ) const;
      // Checks if the given bin contains the given key.

    bool isEmpty( const Bin& bin ) const;
      // Checks if the bin is empty.

    bool wasDeleted( const Bin& bin ) const;
      // Checks if the bin was deleted.

    bool isAvailable( const Bin& bin ) const;
      // Checks if the bin is available to store a value.

    bool shouldGrow( unsigned int inUse, unsigned int count ) const;
      // Determines if the map should increase the number of bins.

    bool shouldShrink( unsigned int inUse, unsigned int count ) const;
      // Determines if the map should reduce the number of bins.

  public:
    // CONSTRUCTORS
    Map();
      // Constructs a new map using the default allocator.
      // This will use the default Hasher specialization as its hash function.

    Map( const HashFunc& hashFunc );
      // Constructs a new map that is using the given hash function and
      // the default allocator.

    Map( unsigned int capacity );
      // Constructs a new map using the given capacity and the default
      // allocator.
      // This will use the default Hasher specialization as its hash function.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Map( unsigned int capacity, const HashFunc& hashFunc );
      // Constructs a new map using the given capacity, hash function, and
      // the default allocator.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Map( sgdm::IAllocator<T>* valueAlloc );
      // Constructs a new map using the given allocator.
      // This will use the default Hasher specialization as its hash function.

    Map( sgdm::IAllocator<T>* valueAlloc,
         sgdm::IAllocator<K>* keyAlloc,
         sgdm::IAllocator<Bin>* binAlloc,
         sgdm::IAllocator<sgdu::HashCode>* hashAlloc );
      // Constructs a new map using the given allocators.
      // This will use the default Hasher specialization as its hash function.

    Map( sgdm::IAllocator<T>* allocator, const HashFunc& hashFunc );
      // Constructs a new map using the given allocator and hash function.

    Map( sgdm::IAllocator<T>* valueAlloc,
         sgdm::IAllocator<K>* keyAlloc,
         sgdm::IAllocator<Bin>* binAlloc,
         sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
         const HashFunc& hashFunc );
      // Constructs a new map using the given allocators and hash function.

    Map( sgdm::IAllocator<T>* allocator, unsigned int capacity );
      // Constructs a new map with the given capacity and allocator.
      // This will use the default Hasher specialization as its hash function.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Map( sgdm::IAllocator<T>* valueAlloc,
         sgdm::IAllocator<K>* keyAlloc,
         sgdm::IAllocator<Bin>* binAlloc,
         sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
         unsigned int capacity );
      // Constructs a new map with the given capacity and allocators.
      // This will use the default Hasher specialization as its hash function.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Map( sgdm::IAllocator<T>* allocator, unsigned int capacity,
         const HashFunc& hashFunc );
      // Constructs a new map with the given capacity, hash function,
      // and allocator.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    Map( sgdm::IAllocator<T>* valueAlloc,
         sgdm::IAllocator<K>* keyAlloc,
         sgdm::IAllocator<Bin>* binAlloc,
         sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
         unsigned int capacity, const HashFunc& hashFunc );
    // Constructs a new map with the given capacity, hash function,
    // and allocators.
    // This constructor should be used in the event that it is known that a
    // large number of key-value pairs will be stored. This helps to reduce
    // allocations by initializing the internal key and value arrays to the
    // given capacity.

    Map( const Map<T, K>& other );
      // Constructs a copy of the other map.

    Map( Map<T, K>&& source );
      // Moves the resources from the source to this instance.

    virtual ~Map();
      // Releases the map and all of its resources.

    // OPERATORS
    Map<T, K>& operator=( const Map<T, K>& other );
      // Makes this a copy of the other map.

    Map<T, K>& operator=( Map<T, K>&& source );
      // Moves the resources from the source to this instance.

    const T& operator[]( const K& key ) const;
      // Gets the value that is mapped to the given key.
      //
      // Requirements:
      // There must be a mapping for the key.

    T& operator[]( const K& key );
      // Sets the mapping for the given key.

    // MEMBER FUNCTIONS
    bool has( const K& key ) const;
      // Checks if there is a key-value mapping for the given value.

    T remove( const K& key );
      // Removes the key-value mapping for the specified key.
      //
      // Given the nature of the dynamic array implementation this is the
      // slowest operation.
      //
      // Requirements:
      // A mapping for the given key must exist.

    const DynamicArray<K>& keys() const;
      // Gets all of the mapped keys.

    const DynamicArray<T>& values() const;
      // Gets all of the mapped values.
};

// CONSTANTS
template <typename T, typename K>
constexpr Bin Map<T, K>::BIN_EMPTY;

template <typename T, typename K>
constexpr Bin Map<T, K>::BIN_DELETED;

template <typename T, typename K>
constexpr Bin Map<T, K>::BIN_INVALID;

template <typename T, typename K>
constexpr unsigned int Map<T, K>::GROW_THRESHOLD;

template <typename T, typename K>
constexpr unsigned int Map<T, K>::SHRINK_THRESHOLD;

template <typename T, typename K>
constexpr unsigned int Map<T, K>::MIN_TRANSFER;

template <typename T, typename K>
constexpr unsigned int Map<T, K>::MIN_BINS;

// FREE OPERATORS
template <typename T, typename K>
inline
std::ostream& operator<<( std::ostream& stream,
                          const Map<T, K>& map )
{
    const DynamicArray<std::string>& keys = map.keys();
    int i;

    // object printer
    sgdd::JsonPrinter op( stream );

    // pairs printer
    sgdd::JsonPrinter pp( stream );

    op.open();
    op.print( "size", keys.size() );
    op.printKey( "pairs" );

    pp.open();
    for ( i = 0; i < keys.size(); ++i )
    {
        pp.print( keys[i], map[keys[i]] );
    }
    pp.close();

    op.close();
    return stream;
}

// CONSTRUCTORS
template <typename T, typename K>
inline
Map<T, K>::Map() : d_binAllocator(),
                   d_keys(),
                   d_values(),
                   d_entries(),
                   d_bins( nullptr ),
                   d_binsInUse( 0 ),
                   d_binCount( MIN_BINS ),
                   d_oldBins( nullptr ),
                   d_oldBinIndex( 0 ),
                   d_oldBinCount( 0 )
{
    d_hashFunc = &sgdu::Hasher<K>::hash;
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}


template <typename T, typename K>
Map<T, K>::Map( const HashFunc& hashFunc )
        : d_binAllocator(),
          d_keys(),
          d_values(),
          d_entries(),
          d_hashFunc( hashFunc ),
          d_bins( nullptr ),
          d_binsInUse( 0 ),
          d_binCount( MIN_BINS ),
          d_oldBins( nullptr ),
          d_oldBinIndex( 0 ),
          d_oldBinCount( 0 )
{
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
inline
Map<T, K>::Map( unsigned int capacity ) : d_binAllocator(),
                                          d_keys( capacity ),
                                          d_values( capacity ),
                                          d_entries( capacity ),
                                          d_bins( nullptr ),
                                          d_binsInUse( 0 ),
                                          d_binCount( MIN_BINS ),
                                          d_oldBins( nullptr ),
                                          d_oldBinIndex( 0 ),
                                          d_oldBinCount( 0 )
{
    d_hashFunc = &sgdu::Hasher<K>::hash;
    while ( d_binCount < capacity )
    {
        d_binCount <<= 1;
    }

    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
Map<T, K>::Map( unsigned int capacity, const HashFunc& hashFunc )
        : d_binAllocator(),
          d_keys( capacity ),
          d_values( capacity ),
          d_entries( capacity ),
          d_hashFunc( hashFunc ),
          d_bins( nullptr ),
          d_binsInUse( 0 ),
          d_binCount( MIN_BINS ),
          d_oldBins( nullptr ),
          d_oldBinIndex( 0 ),
          d_oldBinCount( 0 )
{
    while ( d_binCount < capacity )
    {
        d_binCount <<= 1;
    }

    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
inline
Map<T, K>::Map( sgdm::IAllocator<T>* allocator )
    : d_binAllocator(),
      d_keys(),
      d_values( allocator ),
      d_entries(),
      d_binsInUse( 0 ),
      d_binCount( MIN_BINS ),
      d_oldBins( nullptr ),
      d_oldBinIndex( 0 ),
      d_oldBinCount( 0 )
{
    d_hashFunc = &sgdu::Hasher<K>::hash;
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
Map<T, K>::Map( sgdm::IAllocator<T>* valueAlloc,
                sgdm::IAllocator<K>* keyAlloc,
                sgdm::IAllocator<Bin>* binAlloc,
                sgdm::IAllocator<sgdu::HashCode>* hashAlloc )
        : d_binAllocator( binAlloc ),
          d_keys( keyAlloc ),
          d_values( valueAlloc ),
          d_entries( hashAlloc ),
          d_binsInUse( 0 ),
          d_binCount( MIN_BINS ),
          d_oldBins( nullptr ),
          d_oldBinIndex( 0 ),
          d_oldBinCount( 0 )
{
    d_hashFunc = &sgdu::Hasher<K>::hash;
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
Map<T, K>::Map( sgdm::IAllocator<T>* allocator, const HashFunc& hashFunc )
        : d_binAllocator(),
          d_keys(),
          d_values( allocator ),
          d_entries(),
          d_hashFunc( hashFunc ),
          d_binsInUse( 0 ),
          d_binCount( MIN_BINS ),
          d_oldBins( nullptr ),
          d_oldBinIndex( 0 ),
          d_oldBinCount( 0 )
{
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
Map<T, K>::Map( sgdm::IAllocator<T>* valueAlloc,
                sgdm::IAllocator<K>* keyAlloc,
                sgdm::IAllocator<Bin>* binAlloc,
                sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
                const HashFunc& hashFunc )
        : d_binAllocator( binAlloc ),
          d_keys( keyAlloc ),
          d_values( valueAlloc ),
          d_entries( hashAlloc ),
          d_hashFunc( hashFunc ),
          d_binsInUse( 0 ),
          d_binCount( MIN_BINS ),
          d_oldBins( nullptr ),
          d_oldBinIndex( 0 ),
          d_oldBinCount( 0 )
{
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
inline
Map<T, K>::Map( sgdm::IAllocator<T>* allocator, unsigned int capacity )
    : d_binAllocator(),
      d_keys( nullptr, capacity ),
      d_values( allocator, capacity ),
      d_entries( nullptr, capacity ),
      d_binsInUse( 0 ),
      d_binCount( MIN_BINS ),
      d_oldBins( nullptr ),
      d_oldBinIndex( 0 ),
      d_oldBinCount( 0 )
{
    d_hashFunc = &sgdu::Hasher<K>::hash;
    while ( d_binCount < capacity )
    {
        d_binCount <<= 1;
    }

    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
Map<T, K>::Map( sgdm::IAllocator<T>* valueAlloc,
                sgdm::IAllocator<K>* keyAlloc,
                sgdm::IAllocator<Bin>* binAlloc,
                sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
                unsigned int capacity )
        : d_binAllocator( binAlloc ),
          d_keys( keyAlloc, capacity ),
          d_values( valueAlloc, capacity ),
          d_entries( hashAlloc, capacity ),
          d_binsInUse( 0 ),
          d_binCount( MIN_BINS ),
          d_oldBins( nullptr ),
          d_oldBinIndex( 0 ),
          d_oldBinCount( 0 )
{
    d_hashFunc = &sgdu::Hasher<K>::hash;
    while ( d_binCount < capacity )
    {
        d_binCount <<= 1;
    }

    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
Map<T, K>::Map( sgdm::IAllocator<T>* allocator, unsigned int capacity,
                const HashFunc& hashFunc )
        : d_binAllocator(),
          d_keys( nullptr, capacity ),
          d_values( allocator, capacity ),
          d_entries( nullptr, capacity ),
          d_hashFunc( hashFunc ),
          d_binsInUse( 0 ),
          d_binCount( MIN_BINS ),
          d_oldBins( nullptr ),
          d_oldBinIndex( 0 ),
          d_oldBinCount( 0 )
{
    while ( d_binCount < capacity )
    {
        d_binCount <<= 1;
    }

    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
Map<T, K>::Map( sgdm::IAllocator<T>* valueAlloc,
                sgdm::IAllocator<K>* keyAlloc,
                sgdm::IAllocator<Bin>* binAlloc,
                sgdm::IAllocator<sgdu::HashCode>* hashAlloc,
                unsigned int capacity, const HashFunc& hashFunc )
        : d_binAllocator( binAlloc ),
          d_keys( keyAlloc, capacity ),
          d_values( valueAlloc, capacity ),
          d_entries( hashAlloc, capacity ),
          d_hashFunc( hashFunc ),
          d_binsInUse( 0 ),
          d_binCount( MIN_BINS ),
          d_oldBins( nullptr ),
          d_oldBinIndex( 0 ),
          d_oldBinCount( 0 )
{
    while ( d_binCount < capacity )
    {
        d_binCount <<= 1;
    }

    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
inline
Map<T, K>::Map( const Map<T, K>& other )
    : d_binAllocator( other.d_binAllocator ),
      d_keys( other.d_keys ),
      d_values( other.d_values ),
      d_entries( other.d_entries ),
      d_hashFunc( other.d_hashFunc ),
      d_binsInUse( other.d_binsInUse ),
      d_binCount( other.d_binCount ),
      d_oldBinIndex( other.d_oldBinIndex ),
      d_oldBinCount( other.d_oldBinCount )
{
    if ( other.d_bins != nullptr )
    {
        d_bins = d_binAllocator.get( d_binCount );
        sgdm::Mem::copy<Bin>( d_binAllocator.allocator(), d_bins,
                              other.d_bins, d_binCount );
    }
    else
    {
        d_bins = nullptr;
    }

    if ( other.d_oldBins != nullptr )
    {
        d_oldBins = d_binAllocator.get( d_oldBinCount );
        sgdm::Mem::copy<Bin>( d_binAllocator.allocator(), d_oldBins,
                              other.d_oldBins, d_oldBinCount );
    }
    else
    {
        d_oldBins = nullptr;
    }
}

template <typename T, typename K>
inline
Map<T, K>::Map( Map<T, K>&& source ) : d_keys( std::move( source.d_keys ) ),
                                 d_values( std::move( source.d_values ) ),
                                 d_entries( std::move( source.d_entries ) ),
                                 d_hashFunc( std::move( source.d_hashFunc ) )
{
    d_binAllocator = source.d_binAllocator;

    d_bins = source.d_bins;
    d_binsInUse = source.d_binsInUse;
    d_binCount = source.d_binCount;

    d_oldBins = source.d_oldBins;
    d_oldBinIndex = source.d_oldBinIndex;
    d_oldBinCount = source.d_oldBinCount;

    source.d_bins = nullptr;
    source.d_binsInUse = 0;
    source.d_binCount = 0;

    source.d_oldBins = nullptr;
    source.d_oldBinIndex = 0;
    source.d_oldBinCount = 0;
}

template <typename T, typename K>
inline
Map<T, K>::~Map()
{
    if ( d_bins != nullptr )
    {
        d_binAllocator.release( d_bins, d_binCount );
    }

    if ( d_oldBins != nullptr )
    {
        d_binAllocator.release( d_oldBins, d_oldBinCount );
    }
}

// OPERATORS
template <typename T, typename K>
Map<T, K>& Map<T, K>::operator=( const Map<T, K>& other )
{
    if ( d_bins != nullptr )
    {
        d_binAllocator.release( d_bins, d_binCount );
    }

    if ( d_oldBins != nullptr )
    {
        d_binAllocator.release( d_oldBins, d_binCount );
    }

    d_keys = other.d_keys;
    d_values = other.d_values;
    d_entries = other.d_entries;

    d_binsInUse = other.d_binsInUse;
    d_binCount = other.d_binCount;

    d_oldBinIndex = other.d_oldBinIndex;
    d_oldBinCount = other.d_oldBinCount;

    if ( other.d_bins != nullptr )
    {
        d_bins = d_binAllocator.get( d_binCount );
        sgdm::Mem::copy<>( d_bins, other.d_bins, d_binCount );
    }
    else
    {
        d_bins = nullptr;
    }

    if ( other.d_oldBins != nullptr )
    {
        d_oldBins = d_binAllocator.get( d_oldBinCount );
        sgdm::Mem::copy<>( d_oldBins, other.d_oldBins, d_oldBinCount );
    }
    else
    {
        d_oldBins = nullptr;
    }

    d_binAllocator = other.d_binAllocator;

    return *this;
}

template <typename T, typename K>
Map<T, K>& Map<T, K>::operator=( Map<T, K>&& source )
{
    if ( d_bins != nullptr )
    {
        d_binAllocator.release( d_bins, d_binCount );
    }

    if ( d_oldBins != nullptr )
    {
        d_binAllocator.release( d_oldBins, d_binCount );
    }

    d_binAllocator = source.d_binAllocator;

    d_keys = std::move( source.d_keys );
    d_values = std::move( source.d_values );
    d_entries = std::move( source.d_entries );

    d_bins = source.d_bins;
    d_binsInUse = source.d_binsInUse;
    d_binCount = source.d_binCount;

    d_oldBins = source.d_oldBins;
    d_oldBinIndex = source.d_oldBinIndex;
    d_oldBinCount = source.d_oldBinCount;

    source.d_bins = nullptr;
    source.d_binsInUse = 0;
    source.d_binCount = 0;

    source.d_oldBins = nullptr;
    source.d_oldBinIndex = 0;
    source.d_oldBinCount = 0;

    return *this;
}

template <typename T, typename K>
const T& Map<T, K>::operator[]( const K& key ) const
{
    sgdu::HashCode code = d_hashFunc( key );

    int index = find( code, d_bins, d_binCount );

    if ( index == BIN_INVALID && d_oldBins != nullptr )
    {
        index = find( code, d_oldBins, d_oldBinCount );
    }

    assert( index != BIN_INVALID );

    return d_values[d_bins[index]];
}

template <typename T, typename K>
T& Map<T, K>::operator[]( const K& key )
{
    // pre-insertion operations
    if ( d_oldBins != nullptr )
    {
        reduceOldBins();
    }
    else if ( shouldGrow( d_binsInUse, d_binCount ) )
    {
        grow();
    }

    sgdu::HashCode code = d_hashFunc( key );

    int oldIndex = BIN_INVALID;
    int index = probe( code, d_bins, d_binCount );

    assert( index != BIN_INVALID );

    Bin& bin = d_bins[index];

    // make sure it doesn't exist in old bins
    if ( d_oldBins != nullptr && !doesContain( bin, code ) )
    {
        oldIndex = find( code, d_oldBins, d_oldBinCount );
    }

    // move to new
    if ( oldIndex != BIN_INVALID && doesContain( d_oldBins[oldIndex], code ) )
    {
        bin = d_oldBins[oldIndex];
        d_oldBins[oldIndex] = BIN_DELETED;
    }
    else if ( isAvailable( bin ) )
    {
        d_values.push( T() );
        d_keys.push( key );
        d_entries.push( code );
        bin = d_values.size() - 1;
        ++d_binsInUse;
    }

    return d_values[bin];
}

// MEMBER FUNCTIONS
template <typename T, typename K>
bool Map<T, K>::has( const K& key ) const
{
    if ( d_keys.size() <= 0 )
    {
        return false;
    }

    sgdu::HashCode code = d_hashFunc( key );

    int bin = find( code, d_bins, d_binCount );

    if ( bin == BIN_INVALID && d_oldBins != nullptr )
    {
        bin = find( code, d_oldBins, d_oldBinCount );
    }

    return bin != BIN_INVALID;
}

template <typename T, typename K>
T Map<T, K>::remove( const K& key )
{
    // pre-removal operations
    if ( d_oldBins != nullptr )
    {
        reduceOldBins();
    }
    else if ( shouldShrink( d_binsInUse, d_binCount ) )
    {
        shrink();
    }

    sgdu::HashCode code = d_hashFunc( key );

    Bin* bins = d_bins;
    int pos = find( code, bins, d_binCount );

    if ( pos == BIN_INVALID )
    {
        bins = d_oldBins;
        pos = find( code, bins, d_oldBinCount );
    }
    else
    {
        --d_binsInUse;
    }

    assert( pos != BIN_INVALID );

    Bin& bin = bins[pos];

    // remove information and clear bin
    unsigned int index = ( unsigned int )bin;

    T value = d_values.removeAt( index );
    d_keys.removeAt( index );
    d_entries.removeAt( index );

    // correct bin reference indices
    for ( pos = 0; pos < d_binCount; ++pos )
    {
        if ( !isAvailable( d_bins[pos] ) && d_bins[pos] > bin )
        {
            --d_bins[pos];
        }
    }

    if ( d_oldBins != nullptr )
    {
        for ( pos = d_oldBinIndex; pos < d_oldBinCount; ++pos )
        {
            if ( !isAvailable( d_oldBins[pos] ) && d_oldBins[pos] > bin )
            {
                --d_oldBins[pos];
            }
        }
    }

    bin = BIN_DELETED;

    return value;
}

template <typename T, typename K>
inline
const DynamicArray<K>& Map<T, K>::keys() const
{
    return d_keys;
}

template <typename T, typename K>
inline
const DynamicArray<T>& Map<T, K>::values() const
{
    return d_values;
}

// HELPER FUNCTIONS
template <typename T, typename K>
void Map<T, K>::grow()
{
    assert( d_oldBins == nullptr );

    d_oldBins = d_bins;
    d_oldBinIndex = 0;
    d_oldBinCount = d_binCount;

    d_binCount <<= 1;
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
void Map<T, K>::shrink()
{
    assert( d_oldBins == nullptr );

    d_oldBins = d_bins;
    d_oldBinIndex = 0;
    d_oldBinCount = d_binCount;

    d_binCount >>= 1;
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T, typename K>
void Map<T, K>::reduceOldBins()
{
    assert( d_oldBins != nullptr );

    sgdu::HashCode code;
    Bin* old;
    int index;
    unsigned int req = getCopyCount( d_binCount, d_oldBinCount );

    // continue while there's more work to do
    while ( req-- > 0 && d_oldBinIndex < d_oldBinCount )
    {
        old = &d_oldBins[d_oldBinIndex++];

        if ( !isAvailable( *old ) )
        {
            code = d_entries[*old];

            index = probe( code, d_bins, d_binCount );

            assert( index != BIN_INVALID );

            d_bins[index] = *old;
            *old = BIN_DELETED;
        }
    }

    // check if all items have been transferred
    if ( d_oldBinIndex >= d_oldBinCount )
    {
        d_binAllocator.release( d_oldBins, d_oldBinCount );
        d_oldBins = nullptr;
        d_oldBinIndex = 0;
        d_oldBinCount = 0;
    }
}

template <typename T, typename K>
inline
int Map<T, K>::find( const sgdu::HashCode& code, Bin* bins,
                     unsigned int count ) const
{
    assert( bins != nullptr );

    if ( count <= 0 )
    {
        return BIN_INVALID;
    }

    int i;
    unsigned int jumps;

    // search for the specified bin
    for ( i = wrap( code, count ), jumps = 0;
          !isEmpty( bins[i] ) && !doesContain( bins[i], code ) &&
          jumps < count;
          i = wrap( i + jump( jumps ), count ), ++jumps )
    {
        // do nothing
    }

    if ( !doesContain( bins[i], code ))
    {
        return BIN_INVALID;
    }

    return i;

}

template <typename T, typename K>
inline
int Map<T, K>::probe( const sgdu::HashCode& code, Bin* bins,
                      unsigned int count ) const
{
    assert( bins != nullptr );

    if ( count <= 0 )
    {
        return BIN_INVALID;
    }

    int i;
    int open = BIN_INVALID;
    unsigned int jumps;

    // search for an available bin, but also search for an existing copy
    for ( i = wrap( code, count ), jumps = 0;
          !isEmpty( bins[i] ) && !doesContain( bins[i], code ) &&
              jumps < count;
          i = wrap( i + jump( jumps ), count ), ++jumps )
    {
        // store only the first available index
        if ( open == BIN_INVALID && isAvailable( bins[i] ) )
        {
            open = i;
        }
    }

    if ( doesContain( bins[i], code ) )
    {
        return i;
    }

    if ( bins[i] == BIN_EMPTY && open == BIN_INVALID )
    {
        open = i;
    }

    assert( open != BIN_INVALID );

    return open;
}

template <typename T, typename K>
inline
unsigned int Map<T, K>::jump( unsigned int probes ) const
{
    return probes;
}

template <typename T, typename K>
inline
unsigned int Map<T, K>::wrap( int index, unsigned int count ) const
{
    return index & ( count - 1 );
}

template <typename T, typename K>
inline
unsigned int Map<T, K>::getCopyCount( unsigned int count,
                                      unsigned int oldCount ) const
{
    // The following is an equation can be use to calculate the minimum
    // number of items that must be copied per progressive pass.
    //
    // This is calculated based on the fact that you can only insert or
    // remove one item at a time so thus this will be called at least once
    // per insert/removal.
    //
    // The following are a few of the sizes that can be observed and
    // information regarding them:
    //
    // Count    75%      30%
    // 32        24       -- <- irrelevant because can't shrink past 32
    // 64        48     19.2
    // 128       96     38.4
    // 256      192     76.8
    //
    // When the in use count hits the 75% mark it grows, when it hits the
    // 30% mark it shrinks. Therefore before that time the old bins must have
    // been completely copied by that time.
    //
    // Since you can only insert/remove exactly one item per step you have
    // exactly the number of steps equal to the difference between the current
    // in use count and the nearest threshold to finish transferring the old
    // bins. The number of steps for each transition can be seen below.
    //
    // Note: The thresholds are rounded down since you can't insert or remove
    //       part of an item.
    //
    // Transition  Count   LTH   UTH   ST
    // 32->64         24    19    48    4
    // 64->128        48    38    96    9
    // 128->256       64    76   192   19
    // ...
    // 256->128       76    38    96   19
    // 128->64        38    19    48    9
    // 64->32         19    --    24    4
    //
    // LTH = Lower Threshold, UTH = Upper Threshold, ST = Steps Available
    //
    // Note: The copy operation must be done at least one step in advance of
    //       when the next change is necessary.
    //
    // From that data I noticed th following pattern:
    //
    // steps = ( 5 * ( min( oldCount, newCount ) >> 4 ) ) - 1
    //
    // Then using that number to copy per step can be calculated as:
    //
    // oldCount / steps
    //
    // Finally for safety I ensured a minimum copy count was enforced.
    //
    return std::max( ( 5 * ( std::min( oldCount, count ) >> 4 ) - 1 ),
                     MIN_TRANSFER );
}

template <typename T, typename K>
inline
bool Map<T, K>::doesContain( const Bin& bin,
                             const sgdu::HashCode& code ) const
{
    assert( bin != BIN_INVALID );

    return !isAvailable( bin ) && code == d_entries[bin];
}

template <typename T, typename K>
inline
bool Map<T, K>::isEmpty( const Bin& bin ) const
{
    return bin == BIN_EMPTY;
}

template <typename T, typename K>
inline
bool Map<T, K>::wasDeleted( const Bin& bin ) const
{
    return bin == BIN_DELETED;
}

template <typename T, typename K>
inline
bool Map<T, K>::isAvailable( const Bin& bin ) const
{
    return isEmpty( bin ) || wasDeleted( bin );
}

template <typename T, typename K>
inline
bool Map<T, K>::shouldGrow( unsigned int inUse, unsigned int count ) const
{
    return ( ( inUse * 100 ) / count ) > GROW_THRESHOLD;
}

template <typename T, typename K>
inline
bool Map<T, K>::shouldShrink( unsigned int inUse, unsigned int count ) const
{
    return ( ( inUse * 100 ) / count ) < SHRINK_THRESHOLD && count > MIN_BINS;
}

} // End nspc sgdc

} // End nspc StevensDev

#endif
