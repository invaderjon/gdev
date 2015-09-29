// json_printer.h
#ifndef INCLUDED_JSON_PRINTER
#define INCLUDED_JSON_PRINTER

#include <iostream>

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

class JSONPrinter
{
  private:
    std::ostream* d_ostream;
      // The stream to write to.

  public:
    JSONPrinter();
      // Constructs a json stream that writes to std::cout.

    JSONPrinter( std::ostream& ostream );
      // Constructs a json stream that writes to the given output stream.

    JSONPrinter( const JSONPrinter& other );
      // Construct a copy of the given printer. This printer will reference
      // the same internal stream as the other.

    ~JSONPrinter();
      // Destructor.

    JSONPrinter& operator=( const JSONPrinter& other );
      // Makes this a copy of the other printer. This printer will reference
      // the same internal stream as the other.

    template<typename T>
    JSONPrinter& operator<<( const T* value );
      // Formats and prints the value to the internal stream.
      //
      // This will only print C style strings as expected. For other types
      // this is simply a safety.

    template<typename T>
    JSONPrinter& operator<<( const T& value );
      // Formats and prints the value to the internal stream.
      //
      // All non-simple types are assumed to handle their own formatting.

    JSONPrinter& open();
      // Opens a json object.

    JSONPrinter& close();
      // Closes a json object.

    template<typename T>
    JSONPrinter& print( const T* value );
      // Formats and prints the value to the internal stream.
      //
      // This will only print C style strings as expected. For other types
      // this is simply a safety.

    template<typename T>
    JSONPrinter& print( const T& value );
      // Formats and prints a value to the internal stream.

    template<typename T>
    JSONPrinter& print( const std::string& key, const T* value );
      // Formats and prints the key-value pair to the internal stream.
      //
      // This will only print C style strings as expected. For other types
      // this is simply a safety.

    template<typename T>
    JSONPrinter& print( const std::string& key, const T& value );
      // Formats and prints the key-value pair to the internal stream.
      //
      // All non-simple types are assumed to handle their own formatting.

    JSONPrinter& printKey( const std::string& key );
      // Prints the given key in anticipation of a value.

    template<typename T>
    JSONPrinter& printArr( const T* value, size_t size );
      // Formats and prints an array of values to the internal stream.

    template<typename T>
    JSONPrinter& printArr( const T& iterable, size_t size );
      // Formats and prints the array to the internal stream.
      //
      // The iterable object must override the [] operator.

    template<typename T>
    JSONPrinter& printArr( const std::string& key, const T* value,
                           size_t size );
      // Formats and prints the key-array pair to the internal stream.
      //
      // Non-simple element types are assumed to handle their own formatting.

    template<typename T>
    JSONPrinter& printArr( const std::string& key, const T& iterable,
                           size_t size );
      // Formats and prints the key-array pair to the internal stream.
      //
      // The iterable object must override the [] operator.
};

// FREE OPERATOR
inline
std::ostream& operator<<( std::ostream& stream,
                          const JSONPrinter& printer )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
JSONPrinter::JSONPrinter() : d_ostream( &std::cout )
{
}

inline
JSONPrinter::JSONPrinter( std::ostream& ostream ) : d_ostream( &ostream )
{
}

inline
JSONPrinter::JSONPrinter( const JSONPrinter& other )
    : d_ostream( other.d_ostream )
{
}

inline
JSONPrinter::~JSONPrinter()
{
    d_ostream = nullptr;
}

// OPERATORS
inline
JSONPrinter& JSONPrinter::operator=( const JSONPrinter& other )
{
    d_ostream = other.d_ostream;
    return *this;
}

template<typename T>
inline
JSONPrinter& JSONPrinter::operator<<( const T* value )
{
    return print<>( value );
}

template<typename T>
inline
JSONPrinter& JSONPrinter::operator<<( const T& value )
{
    return print<>( value );
}

// MEMBER FUNCTIONS
inline
JSONPrinter& JSONPrinter::open()
{
    *d_ostream << "{ ";
    return *this;
}

inline
JSONPrinter& JSONPrinter::close()
{
    *d_ostream << " }";
    return *this;
}

template<typename T>
inline
JSONPrinter& JSONPrinter::print( const T* value )
{
    *d_ostream << "\"" << value << "\"";
    return *this;
}

template<typename T>
inline
JSONPrinter& JSONPrinter::print( const T& value )
{
    // is boolean?
    if ( IsBool<T>::value )
    {
        // type protection
        bool* v = ( bool* )( &value );
        *d_ostream << ( *v ? "\"true\"" : "\"false\"" );
    }
    // is primitive? (by json standards)
    else if ( std::is_enum<T>::value ||
         std::is_floating_point<T>::value ||
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
JSONPrinter& JSONPrinter::print( const std::string& key, const T* value )
{
    printKey( key );
    return print<>( value );
}

template<typename T>
inline
JSONPrinter& JSONPrinter::print( const std::string& key, const T& value )
{
    printKey( key );
    return print<>( value );
}

inline
JSONPrinter& JSONPrinter::printKey( const std::string& key )
{
    *d_ostream << "\"" << key << "\": ";
    return *this;
}

template<typename T>
inline
JSONPrinter& JSONPrinter::printArr( const T* value, size_t size )
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
JSONPrinter& JSONPrinter::printArr( const T& iterable, size_t size )
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
JSONPrinter& JSONPrinter::printArr( const std::string& key,
                                    const T* value, size_t size )
{
    printKey( key );
    printArr<>( value, size );
    return *this;
}

template<typename T>
inline
JSONPrinter& JSONPrinter::printArr( const std::string& key, const T& iterable,
                                    size_t size )
{
    printKey( key );
    printArr<>( iterable, size );
    return *this;
}

} // End nspc sgdd

} // End nspc StevensDev

#endif
