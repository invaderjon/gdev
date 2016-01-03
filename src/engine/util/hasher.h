// hasher.h
#ifndef INCLUDED_HASHER
#define INCLUDED_HASHER

#include "engine/util/hash_utils.h"

namespace StevensDev
{

namespace sgdu
{

// STRUCTURES
template <typename T>
struct Hasher
{
    static HashCode hash( const T& value );
      // Computes the hash for a given value.
      //
      // This struct must be specialized for each value type otherwise it
      // always returns zero.
};

// HASHER SPECIALIZATION
template <typename T>
inline
HashCode Hasher<T>::hash( const T& value )
{
    return 0;
}

template <>
inline
HashCode Hasher<std::string>::hash( const std::string& value )
{
    return HashUtils::fnv1a( value );
}

template <>
inline
HashCode Hasher<unsigned int>::hash( const unsigned int& value )
{
    return ( HashUtils::FNV_OFFSET_32 ^ value ) * HashUtils::FNV_PRIME_32;
}

template <>
inline
HashCode Hasher<int>::hash( const int& value )
{
    return ( HashUtils::FNV_OFFSET_32 ^ value ) * HashUtils::FNV_PRIME_32;
}

} // End nspc sgdu

} // End nspc StevensDev
#endif
