// json_entity.cpp
#include "json_entity.h"

namespace StevensDev
{

namespace sgdd
{

JsonEntity::JsonEntity( const JsonEntity& other )
    : d_type( other.d_type )
{
    switch ( d_type )
    {
        case OBJECT:
            d_mapAlloc = other.d_mapAlloc;
            d_map = d_mapAlloc.get( 1 );
            d_mapAlloc.construct( d_map, *other.d_map );
            break;

        case ARRAY:
            d_arrAlloc = other.d_arrAlloc;
            d_array = d_arrAlloc.get( 1 );
            d_arrAlloc.construct( d_array, *other.d_array );
            break;

        case STRING:
            d_strAlloc = other.d_strAlloc;
            d_string = d_strAlloc.get( 1 );
            d_strAlloc.construct( d_string, *other.d_string );
            break;

        case DECIMAL:
            d_decimal = other.d_decimal;
            break;

        case INTEGRAL:
            d_integral = other.d_integral;
            break;

        case BOOLEAN:
            d_boolean = other.d_boolean;
            break;

        default:
            d_integral = 0;
            break;
    }
}

JsonEntity::JsonEntity( JsonEntity&& other ) : d_type( other.d_type )
{
    switch ( d_type )
    {
        case OBJECT:
            d_mapAlloc = other.d_mapAlloc;
            d_map = std::move( other.d_map );

            other.d_map = nullptr;
            break;

        case ARRAY:
            d_arrAlloc = other.d_arrAlloc;
            d_array = std::move( other.d_array );

            other.d_array = nullptr;
            break;

        case STRING:
            d_strAlloc = other.d_strAlloc;
            d_string = std::move( other.d_string );

            other.d_string = nullptr;
            break;

        case DECIMAL:
            d_decimal = other.d_decimal;
            break;

        case INTEGRAL:
            d_integral = other.d_integral;
            break;

        case BOOLEAN:
            d_boolean = other.d_boolean;
            break;

        default:
            d_integral = 0;
            break;
    }
}

JsonEntity::~JsonEntity()
{
    switch ( d_type )
    {
        case OBJECT:
            if ( d_map != nullptr )
            {
                d_mapAlloc.release( d_map, 1 );
            }
            break;

        case ARRAY:
            if ( d_array != nullptr )
            {
                d_arrAlloc.release( d_array, 1 );
            }
            break;

        case STRING:
            if ( d_string != nullptr )
            {
                d_strAlloc.release( d_string, 1 );
            }
            break;
    }
}

JsonEntity& JsonEntity::operator=( const JsonEntity& other )
{
    switch ( d_type )
    {
        case OBJECT:
            if ( d_map != nullptr )
            {
                d_mapAlloc.release( d_map, 1 );
            }
            break;

        case ARRAY:
            if ( d_array != nullptr )
            {
                d_arrAlloc.release( d_array, 1 );
            }
            break;

        case STRING:
            if ( d_string != nullptr )
            {
                d_strAlloc.release( d_string, 1 );
            }
            break;
    }

    d_type = other.d_type;

    switch ( d_type )
    {
        case OBJECT:
            d_mapAlloc = other.d_mapAlloc;
            d_map = d_mapAlloc.get( 1 );
            d_mapAlloc.construct( d_map, *other.d_map );
            break;

        case ARRAY:
            d_arrAlloc = other.d_arrAlloc;
            d_array = d_arrAlloc.get( 1 );
            d_arrAlloc.construct( d_array, *other.d_array );
            break;

        case STRING:
            d_strAlloc = other.d_strAlloc;
            d_string = d_strAlloc.get( 1 );
            d_strAlloc.construct( d_string, *other.d_string );
            break;

        case DECIMAL:
            d_decimal = other.d_decimal;
            break;

        case INTEGRAL:
            d_integral = other.d_integral;
            break;

        case BOOLEAN:
            d_boolean = other.d_boolean;
            break;

        default:
            d_integral = 0;
            break;
    }

    return *this;
}

JsonEntity& JsonEntity::operator=( JsonEntity&& other )
{
    switch ( d_type )
    {
        case OBJECT:
            if ( d_map != nullptr )
            {
                d_mapAlloc.release( d_map, 1 );
            }
            break;

        case ARRAY:
            if ( d_array != nullptr )
            {
                d_arrAlloc.release( d_array, 1 );
            }
            break;

        case STRING:
            if ( d_string != nullptr )
            {
                d_strAlloc.release( d_string, 1 );
            }
            break;
    }

    d_type = other.d_type;

    switch ( d_type )
    {
        case OBJECT:
            d_mapAlloc = other.d_mapAlloc;
            d_map = std::move( other.d_map );

            other.d_map = nullptr;
            break;

        case ARRAY:
            d_arrAlloc = other.d_arrAlloc;
            d_array = std::move( other.d_array );

            other.d_array = nullptr;
            break;

        case STRING:
            d_strAlloc = other.d_strAlloc;
            d_string = std::move( other.d_string );

            other.d_string = nullptr;
            break;

        case DECIMAL:
            d_decimal = other.d_decimal;
            break;

        case INTEGRAL:
            d_integral = other.d_integral;
            break;

        case BOOLEAN:
            d_boolean = other.d_boolean;
            break;

        default:
            d_integral = 0;
            break;
    }

    return *this;
}

} // End nspc sgdd

} // End nspc StevensDev