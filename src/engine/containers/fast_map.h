// fast_map.h
//
// This defines a hash map which uses an unsigned integer as its key.
#ifndef INCLUDED_FAST_MAP
#define INCLUDED_FAST_MAP

#include "../data/json_printer.h"
#include "../memory/allocator_guard.h"
#include "../memory/iallocator.h"
#include "../memory/mem.h"
#include <assert.h>
#include "dynamic_array.h"
#include <string>
#include <search.h>

namespace StevensDev
{

namespace sgdc
{

template<typename T>
class FastMap
{
  private:
    // TYPES
    typedef int Bin;
      // Defines a bin. The bin value is the index of the value in the array.
      // Only positive values are valid where-as certain negative values have
      // special meanings.

    typedef unsigned int HashCode;
      // Defines a hash code. This is used so that the integer size can be
      // easily changed.

    // CONSTANTS
    static const Bin BIN_EMPTY;
      // Defines a bin that is empty.

    static const Bin BIN_DELETED;
      // Defines a bin that was deleted.

    static const Bin BIN_INVALID;
      // Defines a bin that is invalid.

    static const unsigned int FNV_OFFSET;
      // Defines the initial offset used by the FNV-1A hashing function.
      // From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a

    static const unsigned int FNV_PRIME_32;
      // Defines the prime number used by the FNV-1A hashing function.
      // From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a

    static const unsigned int GROW_THRESHOLD;
      // Grows once more than 75% full.

    static const unsigned int SHRINK_THRESHOLD;
      // Shrink once less than 25% full.

    static const unsigned int MIN_TRANSFER;
      // The minimum number of items that must be transferred with each
      // progressive copy pass.

    static const unsigned int MIN_BINS;
      // The minimum number of bins.
    
    // MEMBERS
    sgdm::AllocatorGuard<Bin> d_binAllocator;
      // The allocator used for obtaining new bins.

    DynamicArray<unsigned int> d_keys;
      // The set of keys.

    DynamicArray<T> d_values;
      // The set of values.

    DynamicArray<HashCode> d_entries;
      // The set of pre-hashed key entries. This is maintained to avoid the
      // overhead of rehashing the keys during every equals operation.

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

    unsigned int jump( unsigned int probes ) const;
      // Probes to a new position.

    int find( HashCode code, Bin* bins, unsigned int count ) const;
      // Checks for the the specified hash in the specified set of bins and
      // gets the index of the bin that contains it.
      // If it is not found this will return FAST_BIN_INVALID.

    int probe( HashCode code, Bin* bins, unsigned int count ) const;
      // Probes for an available bin for the given hash in the specified set of
      // bins. If one is found this returns its index, otherwise this
      // returns FAST_BIN_INVALID.

    bool doesContain( Bin bin, HashCode code ) const;
      // Checks if the given bin contains the given key.

    HashCode hash( unsigned int key ) const;
      // Gets the hash code of a given key using FNV-1A.

    unsigned int wrap( int index, unsigned int count ) const;
      // Wraps an index to a size.

    unsigned int getCopyCount( unsigned int count,
                               unsigned int oldCount ) const;
      // Gets minimum number of items that must be copied in next update.

    bool isEmpty( Bin bin ) const;
      // Checks if a bin is empty.

    bool wasDeleted( Bin bin ) const;
      // Checks if a bin was deleted.

    bool isAvailable( Bin bin ) const;
      // Checks if a bin is available.

    bool shouldGrow( unsigned int inUse, unsigned int count ) const;
      // Checks if the bin collection should grow.

    bool shouldShrink( unsigned int inUse, unsigned int count ) const;
      // Checks if the number of bins should shrink.
  public:
    // CONSTRUCTORS
    FastMap();
      // Constructs a new map using the default allocator.

    FastMap( unsigned int capacity );
      // Constructs a new map using the given capacity and the default
      // allocator.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    FastMap( sgdm::IAllocator<T>* allocator );
      // Constructs a new map using the given allocator.

    FastMap( sgdm::IAllocator<T>* allocator, unsigned int capacity );
      // Constructs a new map with the given key and value storage capacities.
      // This constructor should be used in the event that it is known that a
      // large number of key-value pairs will be stored. This helps to reduce
      // allocations by initializing the internal key and value arrays to the
      // given capacity.

    FastMap( const FastMap<T>& other );
      // Constructs a copy of the other map.

    FastMap( FastMap<T>&& source );
      // Moves the resources from the source to this instance.

    virtual ~FastMap();
      // Releases the map and all of its resources.

    // OPERATORS
    FastMap<T>& operator=( const FastMap<T>& other );
      // Makes this a copy of the other map.

    FastMap<T>& operator=( FastMap<T>&& source );
      // Moves the resources from the source to this instance.

    const T& operator[]( unsigned int key ) const;
      // Gets the value that is mapped to the given key.
      //
      // Requirements:
      // There must be a mapping for the key.

    T& operator[]( unsigned int key );
      // Sets the mapping for the given key.

    // MEMBER FUNCTIONS
    bool has( unsigned int key ) const;
      // Checks if there is a key-value mapping for the given value.

    T remove( unsigned int key );
      // Removes the key-value mapping for the specified key.
      //
      // Given the nature of the dynamic array implementation this is the
      // slowest operation.
      //
      // Requirements:
      // A mapping for the given key must exist.

    const DynamicArray<unsigned int>& keys() const;
      // Gets all of the mapped keys.

    const DynamicArray<T>& values() const;
      // Gets all of the mapped values.
};

// CONSTANTS
template <typename T>
const typename FastMap<T>::Bin FastMap<T>::BIN_EMPTY = -1;
// Defines a bin that is empty.

template <typename T>
const typename FastMap<T>::Bin FastMap<T>::BIN_DELETED = -2;
// Defines a bin that was deleted.

template <typename T>
const typename FastMap<T>::Bin FastMap<T>::BIN_INVALID = -3;
// Defines a bin that is invalid.

template <typename T>
const unsigned int FastMap<T>::FNV_OFFSET = 2166136261;
// Defines the initial offset used by the FNV-1A hashing function.
// From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a

template <typename T>
const unsigned int FastMap<T>::FNV_PRIME_32 = 16777619;
// Defines the prime number used by the FNV-1A hashing function.
// From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a

template <typename T>
const unsigned int FastMap<T>::GROW_THRESHOLD = 75;
// Grows once more than 75% full.

template <typename T>
const unsigned int FastMap<T>::SHRINK_THRESHOLD = 30;
// Shrink once less than 25% full.

template <typename T>
const unsigned int FastMap<T>::MIN_TRANSFER = 16;
// The minimum number of items that must be transferred with each
// progressive copy pass.

template <typename T>
const unsigned int FastMap<T>::MIN_BINS = 32;
// The minimum number of bins.

// FREE OPERATORS
template<typename T>
inline
std::ostream& operator<<( std::ostream& stream,
                          const FastMap<T>& map )
{
    const DynamicArray<unsigned int>& keys = map.keys();
    int i;

    // object printer
    sgdd::JsonPrinter op( stream );

    op.open();
    op.print( "size", keys.size() );
    op.printArr( "keys", keys, keys.size() );
    op.printArr( "values", map.values(), map.values().size() );
    op.close();

    return stream;
}

// CONSTRUCTORS
template<typename T>
inline
FastMap<T>::FastMap() : d_binAllocator(),
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
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template <typename T>
inline
FastMap<T>::FastMap( unsigned int capacity ) : d_binAllocator(),
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
    while ( d_binCount < capacity )
    {
        d_binCount <<= 1;
    }

    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template<typename T>
inline
FastMap<T>::FastMap( sgdm::IAllocator<T>* allocator )
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
    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template<typename T>
inline
FastMap<T>::FastMap( sgdm::IAllocator<T>* allocator, unsigned int capacity )
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
    while ( d_binCount < capacity )
    {
        d_binCount <<= 1;
    }

    d_bins = d_binAllocator.get( d_binCount );
    sgdm::Mem::set<Bin>( d_binAllocator.allocator(), d_bins, BIN_EMPTY,
                         d_binCount );
}

template<typename T>
inline
FastMap<T>::FastMap( const FastMap<T>& other )
    : d_binAllocator( other.d_binAllocator ),
      d_keys( other.d_keys ),
      d_values( other.d_values ),
      d_entries( other.d_entries ),
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

template<typename T>
inline
FastMap<T>::FastMap( FastMap<T>&& source )
    : d_keys( std::move( source.d_keys ) ),
      d_values( std::move( source.d_values ) ),
      d_entries( std::move( source.d_entries ) )
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

template<typename T>
inline
FastMap<T>::~FastMap()
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
template<typename T>
FastMap<T>& FastMap<T>::operator=( const FastMap<T>& other )
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

template<typename T>
FastMap<T>& FastMap<T>::operator=( FastMap<T>&& source )
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

template<typename T>
const T& FastMap<T>::operator[]( unsigned int key ) const
{
    HashCode code = hash( key );

    int index = find( code, d_bins, d_binCount );

    if ( index == BIN_INVALID && d_oldBins != nullptr )
    {
        index = find( code, d_oldBins, d_oldBinCount );
    }

    assert( index != BIN_INVALID );

    return d_values[d_bins[index]];
}

template<typename T>
T& FastMap<T>::operator[]( unsigned int key )
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

    HashCode code = hash( key );

    int old = BIN_INVALID;
    int index = probe( code, d_bins, d_binCount );

    assert( index != BIN_INVALID );

    Bin& bin = d_bins[index];

    // make sure it doesn't exist in old bins
    if ( d_oldBins != nullptr && !doesContain( bin, code ) )
    {
        old = find( code, d_oldBins, d_oldBinCount );
    }

    // move to new
    if ( old != BIN_INVALID && doesContain( d_oldBins[old], code ) )
    {
        bin = d_oldBins[old];
        d_oldBins[index] = BIN_DELETED;
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
template<typename T>
bool FastMap<T>::has( unsigned int key ) const
{
    if ( d_keys.size() <= 0 )
    {
        return false;
    }

    HashCode code = hash( key );

    int bin = find( code, d_bins, d_binCount );

    if ( bin == BIN_INVALID && d_oldBins != nullptr )
    {
        bin = find( code, d_oldBins, d_oldBinCount );
    }

    return bin != BIN_INVALID;
}

template<typename T>
T FastMap<T>::remove( unsigned int key )
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

    HashCode code = hash( key );

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

template<typename T>
inline
const DynamicArray<unsigned int>& FastMap<T>::keys() const
{
    return d_keys;
}

template<typename T>
inline
const DynamicArray<T>& FastMap<T>::values() const
{
    return d_values;
}

// HELPER FUNCTIONS
template<typename T>
void FastMap<T>::grow()
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

template<typename T>
void FastMap<T>::shrink()
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

template<typename T>
void FastMap<T>::reduceOldBins()
{
    assert( d_oldBins != nullptr );

    HashCode code;
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

template <typename T>
inline
unsigned int FastMap<T>::jump( unsigned int probes ) const
{
    return probes;
}

template<typename T>
inline
int FastMap<T>::find( HashCode code, Bin* bins, unsigned int count ) const
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

template<typename T>
inline
int FastMap<T>::probe( HashCode code, Bin* bins, unsigned int count ) const
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

template<typename T>
inline
bool FastMap<T>::doesContain( Bin bin, HashCode code ) const
{
    assert( bin != BIN_INVALID );

    return !isAvailable( bin ) && code == d_entries[bin];
}

template<typename T>
inline
typename FastMap<T>::HashCode FastMap<T>::hash( unsigned int key ) const
{
    return ( FNV_OFFSET ^ key ) * FNV_PRIME_32;
}

template <typename T>
inline
unsigned int FastMap<T>::wrap( int index, unsigned int count ) const
{
    return index & ( count - 1 );
}

template <typename T>
inline
unsigned int FastMap<T>::getCopyCount( unsigned int count,
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

template <typename T>
inline
bool FastMap<T>::isEmpty( Bin bin ) const
{
    return bin == BIN_EMPTY;
}

template <typename T>
inline
bool FastMap<T>::wasDeleted( Bin bin ) const
{
    return bin == BIN_DELETED;
}

template <typename T>
inline
bool FastMap<T>::isAvailable( Bin bin ) const
{
    return isEmpty( bin ) || wasDeleted( bin );
}

template <typename T>
inline
bool FastMap<T>::shouldGrow( unsigned int inUse, unsigned int count ) const
{
    return ( ( inUse * 100 ) / count ) > GROW_THRESHOLD;
}

template <typename T>
inline
bool FastMap<T>::shouldShrink( unsigned int inUse, unsigned int count ) const
{
    return ( ( inUse * 100 ) / count ) < SHRINK_THRESHOLD
           && count > MIN_BINS;
}

} // End nspc sgdc

} // End nspc StevensDev

#endif
