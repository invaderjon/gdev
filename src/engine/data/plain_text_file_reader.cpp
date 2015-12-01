// plain_text_file_reader.cpp
#include "plain_text_file_reader.h"

namespace StevensDev
{

namespace sgdd
{

namespace
{

static const unsigned long BUFFER_SIZE = 256;

} // End nspc anonymous

// MEMBER FUNCTIONS
char PlainTextFileReader::get()
{
    long count;
    while ( d_bufferPos >= d_bufferSize )
    {
        count = readLine();
        assert( count > 0L );
    }

    assert( d_bufferPos < d_buffer[0].length() );
    return d_buffer[0][d_bufferPos];
}

std::string PlainTextFileReader::get( unsigned int n )
{
    long count;
    while ( d_bufferPos + n > d_bufferSize )
    {
        count = readLine();

        // no more to the file so cap reading until the end of the file
        if ( count < 0L )
        {
            n = ( unsigned int)( d_bufferSize - d_bufferPos );
        }
    }

    std::ostringstream oss;

    // copy full buffer segments while the number of remaining characters is
    // greater than the buffer segment size
    unsigned int i;
    unsigned int offset;
    for ( i = 0, offset = d_bufferPos;
          n >= d_buffer[i].length();
          n -= d_buffer[i].length() - offset, offset = 0, ++i )
    {
        oss << d_buffer[i].substr( offset, d_buffer[i].length() - offset );
    }

    // copy remainder
    oss << d_buffer[i].substr( offset, n );

     return oss.str();
}

void PlainTextFileReader::advance( unsigned int n )
{
    d_bufferPos += n;
    d_position += n;

    // drop parts of buffer that are no longer accessible
    unsigned long dropped;
    while ( d_buffer.size() > 0 && d_bufferPos >= d_buffer[0].length() )
    {
        dropped = d_buffer.popFront().length();
        d_bufferPos -= dropped;
        d_bufferSize -= dropped;
    }
}

// HELPER FUNCTIONS
long PlainTextFileReader::readLine()
{
    char buffer[BUFFER_SIZE];

    // read and count number read
    long read = d_stream.read( buffer, BUFFER_SIZE ).gcount();
    if ( read <= 0 && !d_stream )
    {
        return -1L;
    }

    d_buffer.push( std::string( buffer, ( size_t )read ) );
    d_bufferSize += read;
    return read;
}

} // End nspc sgdd

} // End nspc StevensDev