// plain_text_file_reader.h
#ifndef INCLUDED_PLAIN_TEXT_READER
#define INCLUDED_PLAIN_TEXT_READER

#include "engine/containers/dynamic_array.h"
#include "engine/data/istringreader.h"
#include <fstream>
#include <sstream>

namespace StevensDev
{

namespace sgdd
{

class PlainTextFileReader : public IStringReader
{
  private:
    std::ifstream d_stream;
      // The input file stream.

    sgdc::DynamicArray<std::string> d_buffer;
      // The string buffer.

    unsigned long d_position;
      // The current position.

    unsigned long d_bufferSize;
      // The buffer size (the combined length of all loaded strings).

    unsigned int d_bufferPos;
      // The current position in the buffer.

    // CONSTRUCTORS
    PlainTextFileReader( const PlainTextFileReader& reader );
      // Constructs a copy of the given reader.
      //
      // Streams cannot be copied so this is disabled.

    // OPERATORS
    PlainTextFileReader& operator=( const PlainTextFileReader& reader );
      // Makes this a copy of the given reader.
      //
      // Streams cannot be copied so this is disabled.

    // HELPER FUNCTIONS
    long readLine();
      // Read the next line from the file and returns number of characters
      // read.

  public:
    // CONSTRUCTORS
    PlainTextFileReader();
      // Constructs a new plain text file reader without a file.

    PlainTextFileReader( const std::string& path );
      // Constructs a new plain text file reader for the file at the
      // specified path.

    ~PlainTextFileReader();
      // Destructs the reader.

    virtual char get();
      // Gets the next character in the file.
      //
      // Behavior is undefined when:
      // The end of the file has been reached.

    virtual std::string get( unsigned int n );
      // Gets the next n characters in the file.

    virtual void advance( unsigned int n );
      // Moves forward n characters in the file.

    virtual unsigned long position() const;
      // Gets the current position in the file.

    virtual bool isEnd() const;
      // Checks if the end of the file has been reached.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const PlainTextFileReader& reader )
{
    sgdd::JsonPrinter p( stream );
    p.open().print( "position", reader.position() ).close();
    return stream;
}

// CONSTRUCTORS
inline
PlainTextFileReader::PlainTextFileReader()
    : d_stream(), d_buffer(), d_position( 0 ), d_bufferSize( 0 ),
      d_bufferPos( 0 )
{
}

inline
PlainTextFileReader::PlainTextFileReader( const std::string& path )
    : d_stream( path ), d_buffer(), d_position( 0 ), d_bufferSize( 0 ),
      d_bufferPos( 0 )
{
}

inline
PlainTextFileReader::PlainTextFileReader( const PlainTextFileReader& reader )
    : d_stream(), d_buffer( reader.d_buffer ), d_position( reader.d_position ),
      d_bufferSize( reader.d_bufferSize ), d_bufferPos( reader.d_bufferPos )
{
}

inline
PlainTextFileReader::~PlainTextFileReader()
{
    d_stream.close();
}

// OPERATORS
inline
PlainTextFileReader& PlainTextFileReader::operator=(
    const PlainTextFileReader& reader )
{
    d_buffer = reader.d_buffer;
    d_position = reader.d_position;
    d_bufferSize = reader.d_bufferSize;
    d_bufferPos = reader.d_bufferPos;

    return *this;
}

// MEMBER FUNCTIONS
inline
unsigned long PlainTextFileReader::position() const
{
    return d_position;
}

inline
bool PlainTextFileReader::isEnd() const
{
    return d_stream.eof() && d_bufferPos >= d_bufferSize;
}

} // End nspc sgdd

} // End nspc StevensDev

#endif