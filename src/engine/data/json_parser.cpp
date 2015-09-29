// json_parser.cpp
#include "json_parser.h"
#include "../util/string_utils.h"
#include <sstream>

namespace StevensDev
{

namespace sgdd
{

namespace
{

typedef sgdc::Map<JsonEntity> JsonObject;
  // Helper type to shorten name.

typedef sgdc::DynamicArray<JsonEntity> JsonArray;
  // Helper type to shorten name.

typedef sgdm::IAllocator<JsonEntity>* Allocator;

unsigned int parseValue( Allocator alloc, JsonEntity* entity,
                         const std::string& raw, unsigned int pos );
  // Parses a generic json value using raw json and offset position and
  // stores it in the given entity.

inline
void fail( const std::string& reason, unsigned int pos )
{
    std::ostringstream oss;
    oss << "Parsing Error: " << reason << " Encountered at " << pos << ".";

    std::string what = oss.str();
    throw std::runtime_error( what );
}

inline
unsigned int skipWhiteSpace( const std::string& raw, unsigned int offset,
                             bool checkNotEnd = true )
{
    while ( offset < raw.length() &&
            isspace( raw[offset] ) )
    {
        ++offset;
    }

    if ( checkNotEnd && offset >= raw.length() )
    {
        fail( "Reached end while parsing!", offset - 1 );
    }

    return offset;
}

unsigned int parseKey( std::string* storage, const std::string& raw,
                       unsigned int pos )
{
    if ( raw[pos] != '"' )
    {
        fail( "Invalid key!", pos );
    }

    std::ostringstream oss;
    while ( ++pos < raw.length() && raw[pos] != '"' )
    {
        oss << raw[pos];
    }

    if ( pos >= raw.length() )
    {
        fail( "Unclosed key string!", pos );
    }

    pos = skipWhiteSpace( raw, ++pos );

    if ( raw[pos] != ':' )
    {
        fail( "Incomplete key-value pair!", pos );
    }

    *storage = oss.str();

    return pos + 1;
}

unsigned int parseEmpty( JsonEntity* entity, const std::string& raw,
                         unsigned int pos )
{
    assert( raw[pos] == 'n' );

    if ( pos + 4 >= raw.length() )
    {
        fail( "Unclosed value!", pos );
    }

    // valid?
    if ( !sgdu::StringUtils::areEqual( raw.substr( pos, 4 ), "null" ) )
    {
        fail( "Invalid null value!", pos );
    }

    *entity = std::move( JsonEntity() );

    return pos + 4;
}

unsigned int parseTrue( JsonEntity* entity, const std::string& raw,
                        unsigned int pos )
{
    assert( raw[pos] == 't' );

    if ( pos + 4 >= raw.length() )
    {
        fail( "Unclosed value!", pos );
    }

    // valid?
    if ( !sgdu::StringUtils::areEqual( raw.substr( pos, 4 ), "true" ) )
    {
        fail( "Invalid true value!", pos );
    }

    JsonEntity e ( true );
    *entity = std::move( e );

    return pos + 4;
}

unsigned int parseFalse( JsonEntity* entity, const std::string& raw,
                         unsigned int pos )
{
    assert( raw[pos] == 'f' );

    if ( pos + 5 >= raw.length() )
    {
        fail( "Unclosed value!", pos );
    }

    // valid?
    if ( !sgdu::StringUtils::areEqual( raw.substr( pos, 5 ), "false" ) )
    {
        fail( "Invalid false value!", pos );
    }

    *entity = std::move( JsonEntity( false ) );

    return pos + 5;
}

unsigned int parseNumber( JsonEntity* entity, const std::string& raw,
                          unsigned int pos )
{
    assert( raw[pos] == '-' || isdigit( raw[pos] ) );

    double sign = 1;
    double value = 0;

    // negative?
    if ( raw[pos] == '-' )
    {
        sign *= -1;
        ++pos;
    }

    // still valid?
    if ( pos >= raw.length() || !isdigit( raw[pos] ) )
    {
        fail( "Invalid number!", pos );
    }

    // integral portion
    while ( pos < raw.length() && isdigit( raw[pos] ) )
    {
        value = ( value * 10 ) + ( raw[pos++] - '0' );
    }

    // shouldn't reach the end of a string while parsing a value
    if ( pos >= raw.length() )
    {
        fail( "Unclosed value!", pos );
    }

    // integral?
    if ( raw[pos] != '.' )
    {
        // fail on overflow
        assert( int( sign * value ) == sign * value );
        *entity = std::move( JsonEntity( int( sign * value ) ) );
        return pos;
    }

    // still valid?
    if ( ++pos >= raw.length() || !isdigit( raw[pos] ) )
    {
        fail( "Invalid number!", pos );
    }

    // fraction portion
    unsigned int old = pos;
    while ( pos < raw.length() && isdigit( raw[pos] ) )
    {
        value += double( raw[pos] - '0' ) / double( ( old + 1 ) - pos );
        ++pos;
    }

    // shouldn't reach the end of a string while parsing a value
    if ( pos >= raw.length() )
    {
        fail( "Unclosed value!", pos );
    }

    *entity = std::move( JsonEntity( sign * value ) );
    return pos;
}

unsigned int parseString( JsonEntity* entity, const std::string& raw,
                          unsigned int pos )
{
    assert( raw[pos] == '"' );

    std::ostringstream oss;

    while ( ++pos < raw.length() && raw[pos] != '"' )
    {
        oss << raw[pos];
    }

    if ( pos >= raw.length() )
    {
        fail( "Unclosed string!", pos );
    }

    sgdm::AllocatorGuard<std::string> alloc;
    std::string* value = alloc.get( 1 );
    alloc.construct( value, oss.str() );

    JsonEntity e( value, nullptr );
    *entity = std::move( e );

    return pos + 1;
}

unsigned int parseArray( Allocator alloc, JsonEntity* entity,
                         const std::string&  raw, unsigned int pos )
{
    assert( raw[pos] == '[' );

    sgdm::AllocatorGuard<JsonArray> arrAlloc;
    JsonArray* arr = arrAlloc.get( 1 );
    arrAlloc.construct( arr, std::move( JsonArray( alloc ) ) );

    pos = skipWhiteSpace( raw, ++pos );


    // empty?
    if ( raw[pos] == ']' )
    {
        *entity = std::move( JsonEntity( arr, nullptr ) );
        return pos + 1;
    }

    // parse
    bool hasMore = true;
    JsonEntity next;
    while ( hasMore )
    {
        // parse and add value
        pos = parseValue( alloc, &next, raw, pos );
        arr->push( std::move( next ) );

        pos = skipWhiteSpace( raw, pos );

        if ( raw[pos] != ',' )
        {
            hasMore = false;
        }
        else
        {
            pos = skipWhiteSpace( raw, ++pos );
        }
    }

    if ( raw[pos] != ']' )
    {
        fail( "Invalid array!", pos );
    }

    *entity = std::move( JsonEntity( arr, nullptr ) );
    return pos + 1;
}

unsigned int  parseObject( Allocator alloc, JsonEntity* entity,
                           const std::string& raw, unsigned int pos )
{
    assert( raw[pos] == '{' );

    sgdm::AllocatorGuard<JsonObject> objAlloc;
    JsonObject* obj = objAlloc.get( 1 );
    objAlloc.construct( obj, std::move( JsonObject( alloc ) ) );

    pos = skipWhiteSpace( raw, ++pos );

    // empty?
    if ( raw[pos] == '}' )
    {
        *entity = std::move( JsonEntity( obj, nullptr ) );
        return ++pos;
    }

    bool hasMore = true;
    std::string key;
    JsonEntity next;
    while ( hasMore )
    {
        // parse key-value pair
        pos = parseKey( &key, raw, pos );
        pos = skipWhiteSpace( raw, pos );
        pos = parseValue( alloc, &next, raw, pos );

        ( *obj )[key] = next;

        pos = skipWhiteSpace( raw, pos );

        if ( raw[pos] != ',' )
        {
            hasMore = false;
        }
        else
        {
            pos = skipWhiteSpace( raw, ++pos );
        }
    }

    if ( raw[pos] != '}' )
    {
        fail( "Invalid object!", pos );
    }

    *entity = std::move( JsonEntity( obj, nullptr ) );
    return pos + 1;
}

unsigned int parseValue( Allocator alloc, JsonEntity* entity,
                         const std::string& raw, unsigned int pos )
{
    // number?
    if ( raw[pos] == '-' || isdigit( raw[pos] ) )
    {
        pos = parseNumber( entity, raw, pos );
        return pos;
    }

    switch ( raw[pos] )
    {
        // object?
        case '{':
            pos = parseObject( alloc, entity, raw, pos );
            break;

        // array?
        case '[':
            pos = parseArray( alloc, entity, raw, pos );
            break;

        // string?
        case '"':
            pos = parseString( entity, raw, pos );
            break;

        // boolean?
        case 't':
            pos = parseTrue( entity, raw, pos );
            break;

        case 'f':
            pos = parseFalse( entity, raw, pos );
            break;

        // empty?
        case 'n':
            pos = parseEmpty( entity, raw, pos );
            break;

        default:
            fail( "Invalid value type!", pos );
    }

    return pos;
}

JsonEntity* parseJson( sgdm::IAllocator<JsonEntity>* alloc,
                       const std::string& raw )
{
    sgdm::AllocatorGuard<JsonEntity> allocator;

    JsonEntity* root = allocator.get( 1 );
    allocator.construct( root, JsonEntity() );

    unsigned int start = skipWhiteSpace( raw, 0, false );

    // done?
    if ( start >= raw.length() )
    {
        return root;
    }

    switch ( raw[start] )
    {
        // object
        case '{':
            parseObject( alloc, root, raw, start );
            break;

        // array
        case '[':
            parseArray( alloc, root, raw, start );
            break;

        default:
            fail( "Invalid json!", start );
    }

    return root;
}

} // End nspc anonymous

JsonEntity* JsonParser::fromString( const std::string& raw,
                                    sgdm::IAllocator<JsonEntity>* allocator )
{
    return parseJson( allocator, raw );
}

} // End nspc sgdd

} // End nspc StevensDev
