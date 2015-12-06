// json_entity.h
#ifndef INCLUDED_JSON_ENTITY
#define INCLUDED_JSON_ENTITY

#include "../containers/dynamic_array.h"
#include "../containers/map.h"
#include "../memory/iallocator.h"
#include "../memory/stack_guard.h"
#include <string.h>

namespace StevensDev
{

namespace sgdd
{

class JsonEntity
{
  private:
    int d_type;
      // The type of the value contained by this json entity.

    sgdm::AllocatorGuard<sgdc::Map<JsonEntity>> d_mapAlloc;
      // The map allocator.

    sgdm::AllocatorGuard<sgdc::DynamicArray<JsonEntity>> d_arrAlloc;
      // The array allocator.

    sgdm::AllocatorGuard<std::string> d_strAlloc;
      // The string allocator.

    union
    {
        sgdc::Map<JsonEntity>* d_map;
          // Value as a pionter to an map.

        sgdc::DynamicArray<JsonEntity>* d_array;
          // Value as a pointer to an array.

        std::string* d_string;
          // Value as a pointer to an string.

        double d_decimal;
          // Value as a decimal.

        int d_integral;
          // Value as a natural number.

        bool d_boolean;
          // Value as a boolean.
    };

  public:
    // ENUMERATIONS
    enum EntityTypes
    {
        EMPTY,
          // A null value. This is called empty to avoid conflict with NULL.

        OBJECT,
          // A json object value.

        ARRAY,
          // A json array value.

        STRING,
          // A string value.

        DECIMAL,
          // A floating point number value.

        INTEGRAL,
          // A integer number value.

        BOOLEAN,
          // A boolean value.

        INVALID,
          // An invalid/unsupported type value.
    };

    // CONSTRUCTORS
    JsonEntity();
      // Constructs a new null json object.

    JsonEntity( sgdc::Map<JsonEntity>* value,
                sgdm::IAllocator<sgdc::Map<JsonEntity>>* allocator );
      // Constructs a new json object wit the given key-value mappings.

    JsonEntity( sgdc::DynamicArray<JsonEntity>* value,
                sgdm::IAllocator<sgdc::DynamicArray<JsonEntity>>* allocator );
      // Constructs a new json entity that holds an array.

    JsonEntity( std::string* value, sgdm::IAllocator<std::string>* allocator );
      // Constructs a new json entity that holds a string.

    JsonEntity( double value );
      // Constructs a new json entity with a double value.

    JsonEntity( int value );
      // Constructs a new json entity with an integer value.

    JsonEntity( bool value );
      // Constructs a new json entity with a boolean value.

    JsonEntity( const JsonEntity& other );
      // Constructs a new entity that is a copy of another entity.

    JsonEntity( JsonEntity&& other );
      // Constructs a new entity that takes the data from another entity.

    ~JsonEntity();
      // Deconstructs the entity.

    // OPERATORS
    JsonEntity& operator=( const JsonEntity& other );
      // Makes this a copy of the other json entity.

    JsonEntity& operator=( JsonEntity&& other );
      // Moves the data from the other json entity to this.

    const JsonEntity& operator[]( const std::string& key ) const;
      // Gets the value associated with the given key.
      //
      // Requirements:
      // value is an object
      // there is a value for the given key

    const JsonEntity& operator[]( int index ) const;
      // Gets the value at the given index in the array.
      //
      // Requirements:
      // value is an array
      // index is not out of bounds

    bool has( const std::string& key ) const;
      // Checks if the object has a mapping for the given key.
      //
      // Requirements:
      // value is an object

    // MEMBER FUNCTIONS
    const sgdc::DynamicArray<std::string>& keys() const;
      // Gets the list of keys in the object.

    const sgdc::DynamicArray<JsonEntity>& values() const;
      // Gets the list of values in the object.

    // TYPE CAST FUNCTIONS
    const sgdc::DynamicArray<JsonEntity>& asArray() const;
      // Gets the value as an array.

    const std::string& asString() const;
      // Gets the value as a string.

    double asDouble() const;
      // Gets the value as a double.

    int asInt() const;
      // Gets the value as an integer.

    bool asBoolean() const;
      // Gets the value as a boolean.

    // TYPE CHECK FUNCTIONS
    EntityTypes type() const;

    bool isObject() const;
      // Checks if the value is a json object.

    bool isArray() const;
      // Checks if the value is a json array.

    bool isString() const;
      // Checks if the value is a string.

    bool isDouble() const;
      // Checks if the value is a floating point.

    bool isInt() const;
      // Checks if the value is an integer.

    bool isBoolean() const;
      // Checks if the value is a boolean.

    bool isNull() const;
      // Checks if the value is null.
};

// FREE OPERATORS
static
std::ostream& operator<<( std::ostream& stream, const JsonEntity& entity )
{
    int i;
    switch ( entity.type() )
    {
        case JsonEntity::EMPTY:
            stream << "null";
            break;

        case JsonEntity::OBJECT:
            stream << "{ ";

            for ( i = 0; i < entity.keys().size(); ++i )
            {
                // keys and values are known to be stored in the ordered that
                // they are added. Thus, keys and values have the same indices.
                stream << "\"" << entity.keys()[i] << "\": ";
                stream << entity.values()[i];

                if ( i < entity.keys().size() - 1 )
                {
                    stream << ", ";
                }
            }

            stream << " }";
            break;

        case JsonEntity::ARRAY:
            stream << "[ ";

            for ( i = 0; i < entity.asArray().size(); ++i )
            {
                stream << entity[i];

                if ( i < entity.asArray().size() - 1 )
                {
                    stream << ", ";
                }
            }

            stream << " ]";
            break;

        case JsonEntity::STRING:
            stream << "\"" << entity.asString() << "\"";
            break;

        case JsonEntity::DECIMAL:
            stream << entity.asDouble();
            break;

        case JsonEntity::INTEGRAL:
            stream << entity.asInt();
            break;

        case JsonEntity::BOOLEAN:
            stream << ( entity.asBoolean() ? "true" : "false" );
            break;

        case JsonEntity::INVALID:
            // do nothing
            break;
    }

    return stream;
}

// CONSTRUCTORS
inline
JsonEntity::JsonEntity() : d_type( JsonEntity::EMPTY ), d_integral( 0 )
{
}

inline
JsonEntity::JsonEntity( sgdc::Map<JsonEntity>* value,
                        sgdm::IAllocator<sgdc::Map<JsonEntity>>* allocator )
    : d_type( JsonEntity::OBJECT ), d_map( value ), d_mapAlloc( allocator )
{
}

inline
JsonEntity::JsonEntity(
    sgdc::DynamicArray<JsonEntity>* value,
    sgdm::IAllocator<sgdc::DynamicArray<JsonEntity>>* allocator )
    : d_type( JsonEntity::ARRAY ), d_array( value ), d_arrAlloc( allocator )
{
}

inline
JsonEntity::JsonEntity( std::string* value,
                        sgdm::IAllocator<std::string>* allocator )
    : d_type( JsonEntity::STRING ), d_string( value ), d_strAlloc( allocator )
{
}

inline
JsonEntity::JsonEntity( double value )
    : d_type( JsonEntity::DECIMAL ), d_decimal( value )
{
}

inline
JsonEntity::JsonEntity( int value )
    : d_type( JsonEntity::INTEGRAL ), d_integral( value )
{
}

inline
JsonEntity::JsonEntity( bool value )
    : d_type( JsonEntity::BOOLEAN ), d_boolean( value )
{
}

// OPERATORS
inline
const JsonEntity& JsonEntity::operator[]( const std::string& key ) const
{
    return ( *d_map )[key];
}

inline
const JsonEntity& JsonEntity::operator[]( int index ) const
{
    return ( *d_array )[index];
}

inline
bool JsonEntity::has( const std::string& key ) const
{
    return d_map->has( key );
}

// MEMBER FUNCTIONS
inline
const sgdc::DynamicArray<std::string>& JsonEntity::keys() const
{
    return d_map->keys();
}

inline
const sgdc::DynamicArray<JsonEntity>& JsonEntity::values() const
{
    return d_map->values();
}

inline
const sgdc::DynamicArray<JsonEntity>& JsonEntity::asArray() const
{
    return *d_array;
}

inline
const std::string& JsonEntity::asString() const
{
    return *d_string;
}

inline
double JsonEntity::asDouble() const
{
    return d_decimal;
}

inline
int JsonEntity::asInt() const
{
    return d_integral;
}

inline
bool JsonEntity::asBoolean() const
{
    return d_boolean;
}

inline
JsonEntity::EntityTypes JsonEntity::type() const
{
    return EntityTypes( d_type );
}

inline
bool JsonEntity::isObject() const
{
    return d_type == OBJECT;
}

inline
bool JsonEntity::isArray() const
{
    return d_type == ARRAY;
}

inline
bool JsonEntity::isString() const
{
    return d_type == STRING;
}

inline
bool JsonEntity::isDouble() const
{
    return d_type == DECIMAL;
}

inline
bool JsonEntity::isInt() const
{
    return d_type == INTEGRAL;
}

inline
bool JsonEntity::isBoolean() const
{
    return d_type == BOOLEAN;
}

inline
bool JsonEntity::isNull() const
{
    return d_type == EMPTY;
}

} // End nspc sgdd

} // End nspc StevensDev

#endif
