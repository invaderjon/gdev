// string_utils.h
#ifndef INCLUDED_STRING_UTILS
#define INCLUDED_STRING_UTILS

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
// StringUtils::compileTimeHash<sizeof( "Hello" )>( "Hello" );
// to
// chash( "Hello" )
//
// This can be used to do accomplish tasks such as using strings in switch
// statements
#define chash( string ) \
StevensDev::sgdu::StringUtils::compileTimeHash<sizeof( string ) - 1>( string )

struct StringUtils
{
    static constexpr unsigned int FNV_OFFSET = 2166136261;
      // Defines the initial offset used by the FNV-1A hashing function.
      // From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a

    static constexpr unsigned int FNV_PRIME_32 = 16777619;
      // Defines the prime number used by the FNV-1A hashing function.
      // From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a

    static bool areEqual( const std::string& first,
                          const std::string& second );
      // Checks if two strings are equal.

    static int compare( const std::string& first,
                        const std::string& second );
      // Compares two strings.
      // Returns 0 if equal
      // Returns > 0 if first is greater than second
      // Returns < 0 if first is less than second

    static unsigned int hash( const std::string& str );
      // Obtains an fnv hash code for a string.

    template <int length>
    static constexpr unsigned int compileTimeHash( const char* str );
      // Computes the fnv hash code at compile time.
      //
      // This can only be used with constants. Unknown behavior otherwise.

};

inline
bool StringUtils::areEqual( const std::string& first,
                            const std::string& second )
{
    return !StringUtils::compare( first, second );
}

inline
int StringUtils::compare( const std::string& first,
                          const std::string& second )
{
    int d;
    unsigned int i;
    for ( i = 0, d = 0; !d && i < first.length() && i < second.length(); ++i )
    {
        d = int( first.at( i ) - second.at( i ) );
    }

    if ( d == 0 && first.length() != second.length() )
    {
        if ( first.length() > second.length() )
        {
            d = 1;
        }
        else
        {
            d = -1;
        }
    }

    return d;
}

inline
unsigned int StringUtils::hash( const std::string& str )
{
    unsigned int hash = FNV_OFFSET;
    int i;

    for ( i = 0; i < str.length(); ++i )
    {
        hash ^= str[i];
        hash *= FNV_PRIME_32;
    }

    return hash;
}

template <>
inline
constexpr unsigned int
StringUtils::compileTimeHash<int( 0 )>( const char* str )
{
    return StringUtils::FNV_OFFSET;
}

template <int rem>
inline
constexpr unsigned int StringUtils::compileTimeHash( const char* str )
{
    return ( compileTimeHash<rem - 1>( str ) ^ str[rem - 1] ) *
        StringUtils::FNV_PRIME_32;
}

} // End nspc sgdu

} // End nspc StevensDev

#endif
