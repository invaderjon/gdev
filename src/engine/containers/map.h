// map.h
#ifndef INCLUDED_MAP
#define INCLUDED_MAP

#include "../memory/counting_allocator.h"
#include "../memory/iallocator.h"
#include "dynamic_array.h"
#include <string>
#include <search.h>

#define INVALID_MAP_INDEX ( ( unsigned int )-1 )
#define FNV_PRIME_32

namespace StevensDev
{

namespace sgdc
{

namespace
{

typedef unsigned int HashCode;

// Use a bin structure to minimize the waste from empty bins and to improve
// grow operation performance.
typedef struct
{
    unsigned int index;
      // The index of the entry in the entries array. If there is no item
      // associated with the bin then this is equal to INVALID_MAP_INDEX.
} Bin;

// Uses an entry structure to detect collisions.
typedef struct
{
    HashCode hashCode;
      // The hash code associated with the entry. This is used for collision
      // detection.

    unsigned int index;
      // The index of the item in the values array.

    unsigned int next;
      // The index of the next item in the bin. If there are no more items in
      // the bin then this is equal to INVALID_MAP_INDEX.
} Entry;


// ANONYMOUS HELPER FUNCTIONS
unsigned int mapToBinIndex( HashCode code, unsigned int binCount );
  // Maps a hash code to a bin index.

} // End nspc anonymous

template<typename T>
class Map
{
    sgdm::CountingAllocator<Bin> d_binAllocator;
    // Allocator used to obtain new bins.

    sgdm::CountingAllocator<Entry> d_entryAllocator;
    // Allocator used to obtain new entries.

    DynamicArray<std::string> d_keys;
      // The set of keys.

    DynamicArray<T> d_values;
      // The set of values.

    DynamicArray<Entry> d_entries;
      // The set of entries. These are stored in the order they that the
      // key-value pairs are created. This is used instead of bins
      // maintaining pointers to arrays of entries to reduce dynamic memory
      // allocation.

    Bin* d_bins;
      // The set of available bins.

    Bin* d_oldBins;
      // The set of old bins. This is maintained to avoid moving all of the
      // entries at once. Instead entries are moved as they are accessed until
      // there are no more full bins, at which point the old bins are
      // released. If there are no old bins then this is equal to nullptr
      // (zero).

    unsigned int d_binCount;
      // The number of available bins.

    unsigned int d_oldBinIndex;
      // The index of the last checked item in entries. If this is equal to
      // or greater than d_oldBinCount then there are no more items to check.

    unsigned int d_oldBinCount;
      // The number of old bins.

    // HELPER FUNCTIONS
    HashCode hash( const std::string& key ) const;
      // Gets the hash code of a given key using FNV-1A.

    const Entry& mapToEntry( HashCode code ) const;
      // Maps a hash code to a mutable entry.

    Entry& mapToEntry( HashCode code );
      // Maps a hash code to a mutable entry. This will create the entry if
      // it is not found.

    Entry* getEntry( const Bin& bin, HashCode code, Entry*& collided ) const;
      // Gets the entry from the bin with the given hash code. If the entry
      // cannot be found then nullptr (zero) is returned. This will store the
      // last known collision in collided if one is encountered.

    unsigned int getEntryIndex( const Bin& bin, HashCode code,
                          Entry*& collided ) const;
      // Gets the index of the entry from the given bin with the given hash
      // code. If the entry cannot be found INVALID_MAP_INDEX is returned.
      // This will store the last known collision in collided if one is
      // encountered.

    Entry& createEntry( HashCode code, Entry* collided );
      // Creates a new entry and adds it to the set of entries. This will
      // also set the collided entry's next index to itself if provided.

    void reduceOldBins();
      // Copy some more items over from the old bin set to the new bin set.
      // This will release the old bins if there are non remaining.

  public:
    // CONSTRUCTORS
    Map();
      // Constructs a new map without an allocator.
      // Behavior for maps created this way is undefined.

    Map( sgdm::IAllocator<T>* allocator );
      // Constructs a new map using the given allocator.

    Map( const Map<T>& other );
      // Constructs a copy of the other map.

    Map( Map<T>&& source );
      // Moves the resources from the source to this instance.

    virtual ~Map();
      // Releases the map and all of its resources.

    // OPERATORS
    Map<T>& operator=( const Map<T>& other );
      // Makes this a copy of the other map.

    Map<T>& operator=( Map<T>&& source );
     // Moves the resources from the source to this instance.

    const T& operator[]( const std::string& key ) const;
      // Gets the value that is mapped to the given key.
      //
      // Requirements:
      // There must be a mapping for the key.

    T& operator[]( const std::string& key );
     // Sets the mapping for the given key.

    // MEMBER FUNCTIONS
    bool has( const std::string& key ) const;
      // Checks if there is a key-value mapping for the given value.

    T remove( const std::string& key );
     // Removes the key-value mapping for the specified key.
     //
     // Given the nature of the dynamic array implementation this is the
     // slowest operation.

    const DynamicArray<std::string>& keys() const;
      // Gets all of the mapped keys.

    const DynamicArray<T>& values() const;
      // Gets all of the mapped values.
};

// FREE OPERATORS
template<typename T>
inline
std::ostream& operator<<( std::ostream& stream,
                          const Map<T>& map )
{
    const DynamicArray<std::string> keys = map.keys();
    int i;

    stream << "{ pairs: " << keys.size();

    for ( i = 0; i < keys.size(); ++i )
    {
        stream << ", " << keys[i] << ": " << map[keys[i]];
    }

    return stream << " }";
}

// CONSTRUCTORS
template<typename T>
inline
Map<T>::Map() : d_keys(), d_values(), d_entries(), d_bins( nullptr ),
                d_binCount( 0 ), d_oldBins( nullptr ), d_oldBinIndex( 0 ),
                d_oldBinCount( 0 )
{
}

template<typename T>
inline
Map<T>::Map( sgdm::IAllocator<T>* allocator )
    : d_keys( allocator ), d_values( allocator ),
      d_entries( &d_entryAllocator ),
      d_oldBinIndex( 0 ), d_oldBinCount( 0 )
{
}

template<typename T>
inline
Map<T>::Map( const Map<T>& other )
    : d_binAllocator( other.d_binAllocator ),
      d_entryAllocator( other.d_entryAllocator ),
      d_keys( other.d_keys ),
      d_values( other.d_values ),
      d_entries( other.d_entries ), d_binCount( other.d_binCount ),
      d_oldBinIndex( other.d_oldBinIndex ),
      d_oldBinCount( other.d_oldBinCount )
{
    d_bins = d_binAllocator.get( d_binCount );
    memcpy( d_bins, other.d_bins, sizeof( Bin ) * d_binCount );

    if ( d_oldBinCount > 0 )
    {
        d_oldBins = d_binAllocator.get( d_oldBinCount );
        memcpy( d_oldBins, other.d_oldBins, sizeof( Bin ) * d_oldBinCount );
    }
}

template<typename T>
inline
Map<T>::Map( Map<T>&& source ) : d_keys( std::move( source.d_keys ) ),
                                 d_values( std::move( source.d_values ) ),
                                 d_entries( std::move( source.d_entries ) )
{
    d_binAllocator = std::move( source.d_binAllocator );

    d_bins = source.d_bins;
    d_binCount = source.d_binCount;

    d_oldBins = source.d_oldBins;
    d_oldBinIndex = source.d_oldBinIndex;
    d_oldBinCount = source.d_oldBinCount;

    source.d_bins = nullptr;
    source.d_binCount = 0;

    source.d_oldBins = nullptr;
    source.d_oldBinIndex = INVALID_MAP_INDEX;
    source.d_oldBinCount = 0;

    d_entryAllocator = std::move( source.d_entryAllocator );
}

template<typename T>
inline
Map<T>::~Map()
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
Map<T>& Map<T>::operator=( const Map<T>& other )
{
    if ( d_bins != nullptr )
    {
        d_binAllocator.release( d_bins, d_binCount );
        d_binCount = 0;
    }

    if ( d_oldBins != nullptr )
    {
        d_binAllocator.release( d_oldBins, d_binCount );
        d_oldBinCount = 0;
        d_oldBinIndex = INVALID_MAP_INDEX;
    }

    d_keys = other.d_keys;
    d_values = other.d_values;
    d_entries = other.d_values;

    d_binCount = other.d_binCount;

    d_oldBinIndex = other.d_oldBinIndex;
    d_oldBinCount = other.d_oldBinCount;

    d_bins = d_binAllocator.get( d_binCount );
    memcpy( d_bins, other.d_bins, sizeof( Bin ) * d_binCount );

    if ( d_oldBinCount > 0 )
    {
        d_oldBins = d_binAllocator.get( d_oldBinCount );
        memcpy( d_oldBins, other.d_oldBins, sizeof( Bin ) * d_oldBinCount );
    }

    d_binAllocator = other.d_binAllocator;
    d_entryAllocator = other.d_entryAllocator;

    return *this;
}

template<typename T>
Map<T>& Map<T>::operator=( Map<T>&& source )
{
    if ( d_bins != nullptr )
    {
        d_binAllocator.release( d_bins, d_binCount );
    }

    if ( d_oldBins != nullptr )
    {
        d_binAllocator.release( d_oldBins, d_binCount );
    }

    d_binAllocator = std::move( source.d_binAllocator );

    d_keys = std::move( source.d_keys );
    d_values = std::move( source.d_values );
    d_entries = std::move( source.d_values );

    d_bins = source.d_bins;
    d_binCount = source.d_binCount;

    d_oldBins = source.d_oldBins;
    d_oldBinIndex = source.d_oldBinIndex;
    d_oldBinCount = source.d_oldBinCount;

    source.d_bins = nullptr;
    source.d_binCount = 0;

    source.d_oldBins = nullptr;
    source.d_oldBinIndex = INVALID_MAP_INDEX;
    source.d_oldBinCount = 0;

    d_entryAllocator = std::move( source.d_entryAllocator );

    return *this;
}

template<typename T>
inline
const T& Map<T>::operator[]( const std::string& key ) const
{
    return d_values[mapToEntry( hash(key) ).index];
}

template<typename T>
T& Map<T>::operator[]( const std::string& key )
{
    Entry& entry = mapToEntry( hash(key) );

    if ( entry.index == INVALID_MAP_INDEX ) // new item
    {
        T value;

        d_keys.push( key );

        d_values.push( value );

        entry.index = d_values.size() - 1;

        return d_values[entry.index];
    }

    reduceOldBins();

    return d_values[entry.index];
}

// MEMBER FUNCTIONS
template<typename T>
bool Map<T>::has( const std::string& key ) const
{
    Entry* entry;
    const HashCode code = hash( key );

    const Bin& bin = d_bins[mapToBinIndex( code, d_binCount )];

    entry = getEntry( bin, code, entry );

    if ( entry != nullptr ) // found
    {
        return true;
    }

    // not found in current bins
    if ( d_oldBins == nullptr )
    {
        return false;
    }

    const Bin& oldBin = d_oldBins[mapToBinIndex( code, d_oldBinCount )];

    entry = getEntry( bin, code, entry );

    return entry != nullptr;
}

template<typename T>
T Map<T>::remove( const std::string& key )
{
    const HashCode code = hash( key );

    Entry* entry;
    Entry* collided = nullptr;
    unsigned int index;

    // check in current bins
    const Bin& bin = d_bins[mapToBinIndex( code, d_binCount )];
    index = getEntryIndex( bin, code, collided );

    if ( index != INVALID_MAP_INDEX ) // found
    {
        entry = &d_entries[index];

        if ( collided != nullptr )
        {
            collided->next = entry->next;
        }

        d_entries.removeAt( index );

        return *entry;
    }

    // not found in current bins
    assert( d_oldBins != nullptr );

    // check old bins
    const Bin& oldBin = d_oldBins[mapToBinIndex( code, d_oldBinCount )];
    collided = nullptr;
    index = getEntryIndex( bin, code, collided );

    assert( index != INVALID_MAP_INDEX );

    entry = &d_entries[index];

    if ( collided != nullptr )
    {
        collided->next = entry->next;
    }

    d_entries.removeAt( index );

    return *entry;
}

template<typename T>
inline
const DynamicArray<std::string>& Map<T>::keys() const
{
    return d_keys;
}

template<typename T>
inline
const DynamicArray<T>& Map<T>::values() const
{
    return d_values;
}

// HELPER FUNCTIONS
template<typename T>
inline
HashCode Map<T>::hash( const std::string& key ) const
{
    int i;
    HashCode hash = 0;

    // todo: add FNV-1A hash function

    return hash;
}

template<typename T>
const Entry& Map<T>::mapToEntry( HashCode code ) const
{
    Entry* entry;

    const Bin& bin = d_bins[mapToBinIndex( code, d_binCount )];

    entry = getEntry( bin, code, entry );

    if ( entry != nullptr ) // found
    {
        return *entry;
    }

    // not found in current bins
    assert( d_oldBins != nullptr );

    const Bin& oldBin = d_oldBins[mapToBinIndex( code, d_oldBinCount )];

    entry = getEntry( bin, code, entry );

    assert( entry != nullptr );

    return *entry;
}

template<typename T>
Entry& Map<T>::mapToEntry( HashCode code )
{
    Entry* entry;
    Entry* collision = nullptr;

    const Bin& bin = d_bins[mapToBinIndex( code, d_binCount )];

    entry = getEntry( bin, code, collision );

    if ( entry != nullptr ) // found
    {
        return *entry;
    }
    else if ( d_oldBins == nullptr ) // no old bins, force insert
    {
        return createEntry( code, collision );
    }

    const Bin& oldBin = d_oldBins[mapToBinIndex( code, d_oldBinCount )];

    entry = getEntry( oldBin, code, entry );

    if ( entry == nullptr ) // not found in old bins, force insert
    {
        return createEntry( code, collision );
    }

    return *entry;
}

template<typename T>
inline
Entry* Map<T>::getEntry( const Bin& bin, HashCode code,
                         Entry*& collided ) const
{
    unsigned int index = getEntryIndex( bin, code, collided );

    if ( index == INVALID_MAP_INDEX )
    {
        return nullptr;
    }

    return &d_entries[index];
}

template<typename T>
unsigned int Map<T>::getEntryIndex( const Bin& bin, HashCode code,
                                    Entry*& collided ) const
{
    if ( bin.index == INVALID_MAP_INDEX )
    {
        return INVALID_MAP_INDEX;
    }

    unsigned int index = bin.index;
    Entry* entry;
    do
    {
        entry = &d_entries[index];

        if ( entry->hashCode != code )
        {
            collided = entry;
            index = entry->next;
            entry = nullptr;
        }
    }
    while ( entry == nullptr && index != INVALID_MAP_INDEX );

    if ( entry == nullptr )
    {
        return INVALID_MAP_INDEX;
    }

    return index;
}

template<typename T>
inline
Entry& Map<T>::createEntry( HashCode code, Entry* collided )
{
    Entry addition;
    addition.hashCode = code;
    addition.next = INVALID_MAP_INDEX;
    addition.index = INVALID_MAP_INDEX;

    d_entries.push( addition );

    if ( collided != nullptr )
    {
        collided->next = d_entries.size() - 1;
    }

    return d_entries[d_entries.size() - 1];
}

template<typename T>
void Map<T>::reduceOldBins()
{
    if ( d_oldBinCount <= 0 )
    {
        return;
    }

    // todo: copy old bins to new bins
}

// ANONYMOUS HELPER FUNCTIONS
inline
unsigned int mapToBinIndex( HashCode code, unsigned int binCount )
{
    // todo: replace modulo with masking if number of bins is a power of 2
    return code % binCount;
}

} // End nspc sgdc

} // End nspc StevensDev

#endif
