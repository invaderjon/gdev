// json_parser.cpp
#include "json_parser.h"
#include "basic_string_reader.h"
#include "plain_text_file_reader.h"
#include <engine/data/istringreader.h>
#include <engine/util/string_utils.h>
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

void parseValue( Allocator alloc, JsonEntity* entity, IStringReader* raw );
  // Parses a generic json value using raw json and offset position and
  // stores it in the given entity.

inline
void fail( const std::string& reason, IStringReader* reader )
{
    std::ostringstream oss;
    oss << "Parsing Error: " << reason << " Encountered at "
        << reader->position() << ": \"" << reader->get(20) << "\"";

    std::string what = oss.str();
    throw std::runtime_error( what );
}

inline
void skipWhiteSpace( IStringReader* reader, bool checkNotEnd = true )
{
    while ( !reader->isEnd() && isspace( reader->get() )  )
    {
        reader->advance( 1 );
    }

    if ( checkNotEnd && reader->isEnd() )
    {
        fail( "Reached end while parsing!", reader );
    }
}

void parseKey( std::string* storage, IStringReader* reader )
{
    if ( reader->get() != '"' )
    {
        fail( "Invalid key!", reader );
    }

    std::ostringstream oss;
    reader->advance( 1 );
    while ( !reader->isEnd() && reader->get() != '"' )
    {
        oss << reader->get();
        reader->advance( 1 );
    }

    if ( reader->isEnd() )
    {
        fail( "Unclosed key string!", reader );
    }

    reader->advance( 1 );
    skipWhiteSpace( reader );

    if ( reader->get() != ':' )
    {
        fail( "Incomplete key-value pair!", reader );
    }

    *storage = oss.str();
    reader->advance( 1 );
}

void parseEmpty( JsonEntity* entity, IStringReader* reader )
{
    assert( reader->get() == 'n' );

    if ( reader->get( 4 ).length() < 4 )
    {
        fail( "Unclosed value!", reader );
    }

    // valid?
    if ( !sgdu::StringUtils::areEqual( reader->get( 4 ), "null" ) )
    {
        fail( "Invalid null value!", reader );
    }

    *entity = std::move( JsonEntity() );

    reader->advance( 4 );
}

void parseTrue( JsonEntity* entity, IStringReader* reader )
{
    assert( reader->get() == 't' );

    if ( reader->get( 4 ).length() < 4 )
    {
        fail( "Unclosed value!", reader );
    }

    // valid?
    if ( !sgdu::StringUtils::areEqual( reader->get( 4 ), "true" ) )
    {
        fail( "Invalid true value!", reader );
    }

    JsonEntity e ( true );
    *entity = std::move( e );

    reader->advance( 4 );
}

void parseFalse( JsonEntity* entity, IStringReader* reader )
{
    assert( reader->get() == 'f' );

    if ( reader->get( 5 ).length() < 5 )
    {
        fail( "Unclosed value!", reader );
    }

    // valid?
    if ( !sgdu::StringUtils::areEqual( reader->get( 5 ), "false" ) )
    {
        fail( "Invalid false value!", reader );
    }

    *entity = std::move( JsonEntity( false ) );

    reader->advance( 5 );
}

void parseNumber( JsonEntity* entity, IStringReader* reader )
{
    assert( reader->get() == '-' || isdigit( reader->get() ) );

    double sign = 1;
    double value = 0;

    // negative?
    if ( reader->get() == '-' )
    {
        sign *= -1;
        reader->advance( 1 );
    }

    // still valid?
    if ( reader->isEnd() || !isdigit( reader->get() ) )
    {
        fail( "Invalid number!", reader );
    }

    // integral portion
    while ( !reader->isEnd() && isdigit( reader->get() ) )
    {
        value = ( value * 10 ) + ( reader->get() - '0' );
        reader->advance( 1 );
    }

    // shouldn't reach the end of a string while parsing a value
    if ( reader->isEnd() )
    {
        fail( "Unclosed value!", reader );
    }

    // integral?
    if ( reader->get() != '.' )
    {
        // fail on overflow
        assert( int( sign * value ) == sign * value );
        *entity = std::move( JsonEntity( int( sign * value ) ) );
        return;
    }

    // still valid?
    reader->advance( 1 );
    if ( reader->isEnd() || !isdigit( reader->get() ) )
    {
        fail( "Invalid number!", reader );
    }

    // fraction portion
    unsigned long old = reader->position();
    while ( !reader->isEnd() && isdigit( reader->get() ) )
    {
        value += double( reader->get() - '0' ) /
                 double( 10 * ( ( reader->position() + 1 ) - old ) );
        reader->advance( 1 );
    }

    // shouldn't reach the end of a string while parsing a value
    if ( reader->isEnd() )
    {
        fail( "Unclosed value!", reader );
    }

    *entity = std::move( JsonEntity( sign * value ) );
}

void parseString( JsonEntity* entity, IStringReader* reader )
{
    assert( reader->get() == '"' );

    std::ostringstream oss;

    reader->advance( 1 );
    while ( !reader->isEnd() && reader->get() != '"' )
    {
        oss << reader->get();
        reader->advance( 1 );
    }

    if ( reader->isEnd() )
    {
        fail( "Unclosed string!", reader );
    }

    sgdm::AllocatorGuard<std::string> alloc;
    std::string* value = alloc.get( 1 );
    alloc.construct( value, oss.str() );

    JsonEntity e( value, nullptr );
    *entity = std::move( e );

    reader->advance( 1 );
}

void parseArray( Allocator alloc, JsonEntity* entity,
                         IStringReader* reader )
{
    assert( reader->get() == '[' );

    sgdm::AllocatorGuard<JsonArray> arrAlloc;
    JsonArray* arr = arrAlloc.get( 1 );
    arrAlloc.construct( arr, std::move( JsonArray( alloc ) ) );

    reader->advance( 1 );
    skipWhiteSpace( reader );

    // empty?
    if ( reader->get() == ']' )
    {
        *entity = std::move( JsonEntity( arr, nullptr ) );
        reader->advance( 1 );
        return;
    }

    // parse
    bool hasMore = true;
    JsonEntity next;
    while ( hasMore )
    {
        // parse and add value
        parseValue( alloc, &next, reader );
        arr->push( std::move( next ) );

        skipWhiteSpace( reader );

        if ( reader->get() != ',' )
        {
            hasMore = false;
        }
        else
        {
            reader->advance( 1 );
            skipWhiteSpace( reader );
        }
    }

    if ( reader->get() != ']' )
    {
        fail( "Invalid array!", reader );
    }

    *entity = std::move( JsonEntity( arr, nullptr ) );
    reader->advance( 1 );
}

void parseObject( Allocator alloc, JsonEntity* entity,
                           IStringReader* reader )
{
    assert( reader->get() == '{' );

    sgdm::AllocatorGuard<JsonObject> objAlloc;
    JsonObject* obj = objAlloc.get( 1 );
    objAlloc.construct( obj, std::move( JsonObject( alloc ) ) );

    reader->advance( 1 );
    skipWhiteSpace( reader );

    // empty?
    if ( reader->get() == '}' )
    {
        *entity = std::move( JsonEntity( obj, nullptr ) );
        reader->advance( 1 );
        return;
    }

    bool hasMore = true;
    std::string key;
    JsonEntity next;
    while ( hasMore )
    {
        // parse key-value pair
        parseKey( &key, reader );
        skipWhiteSpace( reader );
        parseValue( alloc, &next, reader );

        ( *obj )[key] = next;

        skipWhiteSpace( reader );

        if ( reader->get() != ',' )
        {
            hasMore = false;
        }
        else
        {
            reader->advance( 1 );
            skipWhiteSpace( reader );
        }
    }

    if ( reader->get() != '}' )
    {
        fail( "Invalid object!", reader );
    }

    *entity = std::move( JsonEntity( obj, nullptr ) );
    reader->advance( 1 );
}

void parseValue( Allocator alloc, JsonEntity* entity, IStringReader* reader )
{
    // number?
    if ( reader->get() == '-' || isdigit( reader->get() ) )
    {
        parseNumber( entity, reader );
        return;
    }

    switch ( reader->get() )
    {
        // object?
        case '{':
            parseObject( alloc, entity, reader );
            break;

        // array?
        case '[':
            parseArray( alloc, entity, reader );
            break;

        // string?
        case '"':
            parseString( entity, reader );
            break;

        // boolean?
        case 't':
            parseTrue( entity, reader );
            break;

        case 'f':
            parseFalse( entity, reader );
            break;

        // empty?
        case 'n':
            parseEmpty( entity, reader );
            break;

        default:
            fail( "Invalid value type!", reader );
    }
}

JsonEntity* parseJson( sgdm::IAllocator<JsonEntity>* alloc,
                       IStringReader* reader )
{
    sgdm::AllocatorGuard<JsonEntity> allocator( alloc );

    JsonEntity* root = allocator.get( 1 );
    allocator.construct( root, JsonEntity() );

    skipWhiteSpace( reader, false );

    // done?
    if ( reader->isEnd() )
    {
        return root;
    }

    switch ( reader->get() )
    {
        // object
        case '{':
            parseObject( alloc, root, reader );
            break;

        // array
        case '[':
            parseArray( alloc, root, reader );
            break;

        default:
            fail( "Invalid json!", reader );
    }

    return root;
}

} // End nspc anonymous

JsonEntity* JsonParser::fromString( const std::string& raw,
                                    sgdm::IAllocator<JsonEntity>* allocator )
{
    BasicStringReader r( raw );
    return parseJson( allocator, &r );
}

JsonEntity* JsonParser::fromFile( const std::string& path,
                                  sgdm::IAllocator<JsonEntity>* allocator )
{
    PlainTextFileReader r( path );
    return parseJson( allocator, &r );
}

} // End nspc sgdd

} // End nspc StevensDev
