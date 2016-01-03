// hash_utils.h
#ifndef INCLUDED_HASH_UTILS
#define INCLUDED_HASH_UTILS
#include <string>

namespace StevensDev
{

namespace sgdu
{

// chash is a compile-time function that computes the hash code for the given
// string.
//
// This helper exists to reduce code redundancy.
//
// For example, this shortens the following call
// HashUtils::compileTimeHash<sizeof( "Hello" )>( "Hello" );
// to
// chash( "Hello" )
//
// This can be used to do accomplish tasks such as using strings in switch
// statements
#define chash( string ) \
StevensDev::sgdu::HashUtils::compileTimeHash<sizeof( string ) - 1>( string )

// TYPES
typedef unsigned int HashCode;
  // Defines the hashed integer representation of a value.

// STRUCTURES
struct HashUtils
{
    static constexpr HashCode FNV_OFFSET_32 = 2166136261;
      // Defines the initial offset used by the FNV-1A hashing function.
      // From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a

    static constexpr HashCode FNV_PRIME_32 = 16777619;
      // Defines the prime number used by the FNV-1A hashing function.
      // From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a

    static HashCode fnv1a( const std::string& value );
      // Computes the fnv1a hash of a string value.

    template <size_t length>
    static constexpr HashCode compileTimeHash( const char* value );
      // Computes the fnv hash code at compile time.
      //
      // This can only be used with constants. Unknown behavior otherwise.
};

template <>
inline
constexpr HashCode
HashUtils::compileTimeHash<size_t( 0 )>( const char* value )
{
    return HashUtils::FNV_OFFSET_32;
}

template <size_t rem>
inline
constexpr HashCode HashUtils::compileTimeHash( const char* value )
{
    return ( compileTimeHash<rem - 1>( value ) ^ value[rem - 1] ) *
           HashUtils::FNV_PRIME_32;
}

} // End nspc sgdu

} // End nspc StevensDev

#endif
