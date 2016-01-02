// hash_utils.h
#ifndef INCLUDED_HASH_UTILS
#define INCLUDED_HASH_UTILS
#include "engine/util/string_utils.h"
#include <string>

namespace StevensDev
{

namespace sgdu
{

// TYPES
typedef unsigned int HashCode;
  // Defines the hashed integer representation of a value.

template <typename T>
struct Hasher
{
    static HashCode hash( const T& value );
      // Computes the hash for a given value.
      //
      // This struct must be specialized for each value type otherwise it
      // always returns zero.
};

// HASHER SPECIALIZATION FUNCTIONS
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
    return StringUtils::hash( value );
}

} // End nspc sgdu

} // End nspc StevensDev

#endif
