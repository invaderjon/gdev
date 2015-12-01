// istringreader.h
#ifndef INCLUDED_ISTRING_READER
#define INCLUDED_ISTRING_READER

#include <engine/data/ireader.h>
#include <string>

namespace StevensDev
{

namespace sgdd
{

class IStringReader : public IReader<char>
{
  public:
    virtual ~IStringReader() = 0;
      // Destructs the string reader.

    virtual char get() const = 0;
      // Gets the current character.

    virtual std::string get( unsigned int n ) const = 0;
      // Gets up to the next n characters.
      //
      // If there are not n characters remaining it will return as many as
      // it can.

    virtual void advance( unsigned int n ) = 0;
      // Advances the reader n characters.

    virtual unsigned long position() const = 0;
      // Gets the number of characters read.

    virtual bool isEnd() const = 0;
      // Checks if the end has been reached.
};

inline
IStringReader::~IStringReader()
{
}

} // End nspc sgdd

} // End nspc StevensDev

#endif
