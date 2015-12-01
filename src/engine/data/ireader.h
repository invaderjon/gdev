// ireader.h
#ifndef INCLUDED_IREADER
#define INCLUDED_IREADER

namespace StevensDev
{

namespace sgdd
{

template <typename T>
class IReader
{
  public:
    virtual ~IReader() = 0;
      // Destructs the reader.

    virtual T get() const = 0;
      // Gets the current item.

    virtual void advance( unsigned int n ) = 0;
      // Advances the reader n items.

    virtual unsigned long position() const = 0;
      // Gets the number of read items.

    virtual bool isEnd() const = 0;
      // Checks if the end has been reached.
};

template <typename T>
inline
IReader<T>::~IReader()
{
}

} // End nspc sgdd

} // End nspc StevensDev

#endif
