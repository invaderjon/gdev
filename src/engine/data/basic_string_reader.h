// basic_string_reader.h
#ifndef INCLUDED_BASIC_STRING_READER
#define INCLUDED_BASIC_STRING_READER

#include <assert.h>
#include <engine/data/istringreader.h>

namespace StevensDev
{

namespace sgdd
{

class BasicStringReader : public IStringReader
{
  private:
    std::string d_string;
      // The string being read.

    unsigned long d_position;
      // The current position.

  public:
    // CONSTRUCTORS
    BasicStringReader();
      // Constructs a new basic string reader with no string.

    BasicStringReader( const std::string& string );
      // Constructs a basic string reader for the given string.

    BasicStringReader( const BasicStringReader& reader );
      // Constructs a copy of the given reader.

    ~BasicStringReader();
      // Destructs the basic string reader.

    // OPERATORS
    BasicStringReader& operator=( const BasicStringReader& reader );
      // Makes this a copy of the other reader.

    // MEMBER FUNCTIONS
    virtual char get() const;
      // Gets the current character.

    virtual std::string get( unsigned int n ) const;
      // Gets up to the next n characters.
      //
      // If there are not n characters remaining it will return as many as
      // it can.


    virtual void advance( unsigned int n );
      // Advances the reader n characters.

    virtual unsigned long position() const;
      // Gets the number of read items.

    virtual bool isEnd() const;
      // Checks if the end of the string has been reached.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const BasicStringReader& reader )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
BasicStringReader::BasicStringReader() : d_string(), d_position( 0 )
{
}

inline
BasicStringReader::BasicStringReader( const std::string& string )
    : d_string( string ), d_position( 0 )
{
}

inline
BasicStringReader::BasicStringReader( const BasicStringReader& reader )
    : d_string( reader.d_string ), d_position( reader.d_position )
{
}

inline
BasicStringReader::~BasicStringReader()
{
}

// OPERATORS
inline
BasicStringReader& BasicStringReader::operator=(
    const BasicStringReader& reader )
{
    d_string = reader.d_string;
    d_position = reader.d_position;

    return *this;
}

// MEMBER FUNCTIONS
inline
char BasicStringReader::get() const
{
    assert( !isEnd() );
    return d_string[d_position];
}

inline
std::string BasicStringReader::get( unsigned int n ) const
{
    assert( !isEnd() );
    if ( d_position + n > d_string.length() )
    {
        n = ( unsigned int )( d_string.length() - d_position );
    }
    return d_string.substr( d_position, n );
}

inline
void BasicStringReader::advance( unsigned int n )
{
    d_position = std::min( d_position + n, d_string.size() );
}

inline
unsigned long BasicStringReader::position() const
{
    return d_position;
}

inline
bool BasicStringReader::isEnd() const
{
    return d_position >= d_string.size();
}

} // End nspc sgdd

} // End nspc StevensDev

#endif
