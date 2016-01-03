// hash_utils.cpp
#include "hash_utils.h"

namespace StevensDev
{

namespace sgdu
{

HashCode HashUtils::fnv1a( const std::string& value )
{
    HashCode hash = HashUtils::FNV_OFFSET_32;
    int i;

    for ( i = 0; i < value.length(); ++i )
    {
        hash ^= value[i];
        hash *= HashUtils::FNV_PRIME_32;
    }

    return hash;
}

} // End nspc sgdu

} // End nspc StevensDev