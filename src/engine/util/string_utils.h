// string_utils.h
#ifndef INCLUDED_STRING_UTILS
#define INCLUDED_STRING_UTILS

#include <string>

namespace StevensDev
{

namespace sgdu
{

struct StringUtils
{
    static bool areEqual( const std::string& first,
                          const std::string& second );
      // Checks if two strings are equal.

    static int compare( const std::string& first,
                        const std::string& second );
      // Compares two strings.
      // Returns 0 if equal
      // Returns > 0 if first is greater than second
      // Returns < 0 if first is less than second
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

} // End nspc sgdu

} // End nspc StevensDev

#endif
