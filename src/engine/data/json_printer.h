// json_printer.h
//
// This helper class is used to print a JSON string by appending values. The
// primary purpose of which is to ensure consistent JSON styling for
// streamlined comparisons and quick checks.
//
// This should only be used by classes when overriding the std ostream out
// operator. Using it elsewhere is likely to lead to unforseen consequences
// such as malformed JSON.
//
// Limitations:
// Each instance of JsonPrinter is limited to printing one JSON object at a
// time. As such each class must utilize their own unique instance when
// printing. This is because each JSON entity manually tracks how many items
// are currently in the for formatting purposes (specifically for determining
// when to print value separators [commas]).
//
// Reasons for Limitations:
// Although this class could be easily modified to handle nested objects by
// utilizing a stack of value counts which it pushes to for each additional
// nested level it would require a dynamically sized structure. Furthermore
// organizing the printer in such a way would promote poor usage practices
// where classes attempt to also handle printing it's complex data rather
// than leaving it to the data's implementation. The need for nested printing
// can in most situations be avoided entirely by improving structure.
//
// Easy Solution for Nest Objects:
// When printing nested objects is necessary the simplest solution is to use
// a different JsonPrinter instance for each layer of nesting.
#ifndef INCLUDED_JSON_PRINTER
#define INCLUDED_JSON_PRINTER

#include <assert.h>
#include <iostream>
#include <math.h>
#include <cmath>

namespace StevensDev
{

namespace sgdd
{

namespace
{

template<typename T>
struct IsBool
{
    static const bool value = false;
};

template<>
struct IsBool<bool>
{
    static const bool value = true;
};

} // End nspc anonymous

class JsonPrinter
{
  private:
    static const unsigned int CLOSED = ( unsigned int )-1;

    std::ostream* d_ostream;
      // The stream to write to.

    unsigned int d_pairCount;
      // The number of key-value pairs that are in the open object.
      // If no object is open then this is equal to CLOSED.
      // This is not incremented for just values, but is incremented for just
      // keys to allow complex values.

    // PRIVATE MEMBER FUNCTIONS
    bool isOpen() const;
      // Gets if an object is currently open and being printed.

    void nextPair();
      // Prepares the JSON string for the next key-value pair.
      //
      // This prints a separator (comma) followed by a space if when there
      // is at least one key-value pair.

  public:
    // CONSTRUCTORS
    JsonPrinter();
      // Constructs a json stream that writes to std::cout.

    JsonPrinter( std::ostream& ostream );
      // Constructs a json stream that writes to the given output stream.

    JsonPrinter( const JsonPrinter& other );
      // Construct a copy of the given printer. This printer will reference
      // the same internal stream as the other.

    ~JsonPrinter();
      // Destructor.

    // OPERATORS
    JsonPrinter& operator=( const JsonPrinter& other );
      // Makes this a copy of the other printer. This printer will reference
      // the same internal stream as the other.

    template<typename T>
    JsonPrinter& operator<<( const T* value );
      // Formats and prints the value to the internal stream.
      //
      // This will only print C style strings as expected. For other types
      // this is simply a safety.

    template<typename T>
    JsonPrinter& operator<<( const T& value );
      // Formats and prints the value to the internal stream.
      //
      // All non-simple types are assumed to handle their own formatting.

    // MEMBER FUNCTIONS
    JsonPrinter& open();
      // Opens a json object.

    JsonPrinter& close();
      // Closes a json object.

    template<typename T>
    JsonPrinter& print( const T* value );
      // Formats and prints the value to the internal stream.
      //
      // This will only print C style strings as expected. For other types
      // this is simply a safety.

    template<typename T>
    JsonPrinter& print( const T& value );
      // Formats and prints a value to the internal stream.

    template<typename T>
    JsonPrinter& print( const std::string& key, const T* value );
      // Formats and prints the key-value pair to the internal stream.
      //
      // This will only print C style strings as expected. For other types
      // this is simply a safety.

    template<typename T>
    JsonPrinter& print( const std::string& key, const T& value );
      // Formats and prints the key-value pair to the internal stream.
      //
      // All non-simple types are assumed to handle their own formatting.

    JsonPrinter& printKey( const std::string& key );
      // Prints the given key in anticipation of a value.

    template<typename T>
    JsonPrinter& printArr( const T* value, size_t size );
      // Formats and prints an array of values to the internal stream.

    template<typename T>
    JsonPrinter& printArr( const T& iterable, size_t size );
      // Formats and prints the array to the internal stream.
      //
      // The iterable object must override the [] operator.

    template<typename T>
    JsonPrinter& printArr( const std::string& key, const T* value,
                           size_t size );
      // Formats and prints the key-array pair to the internal stream.
      //
      // Non-simple element types are assumed to handle their own formatting.

    template<typename T>
    JsonPrinter& printArr( const std::string& key, const T& iterable,
                           size_t size );
      // Formats and prints the key-array pair to the internal stream.
      //
      // The iterable object must override the [] operator
};

// FREE OPERATOR
inline
std::ostream& operator<<( std::ostream& stream,
                          const JsonPrinter& printer )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
JsonPrinter::JsonPrinter() : d_ostream( &std::cout ), d_pairCount( CLOSED )
{
}

inline
JsonPrinter::JsonPrinter( std::ostream& ostream )
    : d_ostream( &ostream ), d_pairCount( CLOSED )
{
}

inline
JsonPrinter::JsonPrinter( const JsonPrinter& other )
    : d_ostream( other.d_ostream ), d_pairCount( other.d_pairCount )
{
}

inline
JsonPrinter::~JsonPrinter()
{
    // make sure objects are always closed
    assert( !isOpen() );
    d_ostream = nullptr;
}

// OPERATORS
inline
JsonPrinter& JsonPrinter::operator=( const JsonPrinter& other )
{
    assert( !isOpen() );
    d_ostream = other.d_ostream;
    d_pairCount = other.d_pairCount;
    return *this;
}

template<typename T>
inline
JsonPrinter& JsonPrinter::operator<<( const T* value )
{
    return print<>( value );
}

template<typename T>
inline
JsonPrinter& JsonPrinter::operator<<( const T& value )
{
    return print<>( value );
}

// MEMBER FUNCTIONS
inline
JsonPrinter& JsonPrinter::open()
{
    assert( !isOpen() );
    *d_ostream << "{ ";
    d_pairCount = 0;
    return *this;
}

inline
JsonPrinter& JsonPrinter::close()
{
    assert( isOpen() );
    *d_ostream << " }";
    d_pairCount = CLOSED;
    return *this;
}

template<typename T>
inline
JsonPrinter& JsonPrinter::print( const T* value )
{
    *d_ostream << "\"" << value << "\"";
    return *this;
}

template <>
inline
JsonPrinter& JsonPrinter::print( const bool& value )
{
    // type protection
    *d_ostream << ( value ? "\"true\"" : "\"false\"" );
    return *this;
}

template <>
inline
JsonPrinter& JsonPrinter::print( const float& value )
{
    float integral;
    if ( std::modf( value, &integral ) == 0.0f )
    {
        *d_ostream << "\"" << integral << ".0\"";
    }
    else
    {
        *d_ostream << "\"" << value << "\"";
    }
    return *this;
}

template <>
inline
JsonPrinter& JsonPrinter::print( const double& value )
{
    double integral;
    if ( std::modf( value, &integral ) == 0.0 )
    {
        *d_ostream << "\"" << integral << ".0\"";
    }
    else
    {
        *d_ostream << "\"" << value << "\"";
    }
    return *this;
}

template <>
inline
JsonPrinter& JsonPrinter::print( const long double& value )
{
    long double integral;
    if ( std::modf( value, &integral ) == 0.0 )
    {
        *d_ostream << "\"" << integral << ".0\"";
    }
    else
    {
        *d_ostream << "\"" << value << "\"";
    }
    return *this;
}

template<typename T>
JsonPrinter& JsonPrinter::print( const T& value )
{
    // is other primitive? (by json standards)
    if ( std::is_enum<T>::value ||
         std::is_integral<T>::value ||
         std::is_same<T, std::string>::value )
    {
        *d_ostream << "\"" << value << "\"";
    }
    // assume complex
    else
    {
        *d_ostream << value;
    }

    return *this;
}

template<typename T>
inline
JsonPrinter& JsonPrinter::print( const std::string& key, const T* value )
{
    printKey( key );
    return print<>( value );
}

template<typename T>
inline
JsonPrinter& JsonPrinter::print( const std::string& key, const T& value )
{
    printKey( key );
    return print<>( value );
}

inline
JsonPrinter& JsonPrinter::printKey( const std::string& key )
{
    nextPair();
    *d_ostream << "\"" << key << "\": ";
    return *this;
}

template<typename T>
inline
JsonPrinter& JsonPrinter::printArr( const T* value, size_t size )
{
    *d_ostream << "[";
    if ( size > 0 )
    {
        size_t i;
        for ( i = 0; i < size - 1; ++i )
        {
            *d_ostream << " ";
            print<>( value[i] );
            *d_ostream << ",";
        }
        *d_ostream << " ";
        print<>( value[size - 1] );
        *d_ostream << " ";
    }
    *d_ostream << "]";
    return *this;
}

template<typename T>
inline
JsonPrinter& JsonPrinter::printArr( const T& iterable, size_t size )
{
    *d_ostream << "[";
    if ( size > 0 )
    {
        size_t i;
        for ( i = 0; i < size - 1; ++i )
        {
            *d_ostream << " ";
            print<>( iterable[i] );
            *d_ostream << ",";
        }
        *d_ostream << " ";
        print<>( iterable[size - 1] );
        *d_ostream << " ";
    }
    *d_ostream << "]";
    return *this;
}

template<typename T>
inline
JsonPrinter& JsonPrinter::printArr( const std::string& key,
                                    const T* value, size_t size )
{
    printKey( key );
    printArr<>( value, size );
    return *this;
}

template<typename T>
inline
JsonPrinter& JsonPrinter::printArr( const std::string& key, const T& iterable,
                                    size_t size )
{
    printKey( key );
    printArr<>( iterable, size );
    return *this;
}

// PRIVATE MEMBER FUNCTIONS
inline
bool JsonPrinter::isOpen() const
{
    return d_pairCount != CLOSED;
}

inline
void JsonPrinter::nextPair()
{
    assert( isOpen() );

    if ( d_pairCount++ > 0 )
    {
        *d_ostream << ", ";
    }
}


} // End nspc sgdd

} // End nspc StevensDev

#endif
