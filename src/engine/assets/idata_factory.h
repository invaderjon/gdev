// idata_factory.h
#ifndef INCLUDED_IDATA_FACTORY
#define INCLUDED_IDATA_FACTORY

#include <string>

namespace StevensDev
{

namespace sgda
{

template <typename T>
class IDataFactory
{
  public:
    // CONSTRUCTORS
    virtual ~IDataFactory() = 0;

    // MEMBER FUNCTIONS
    virtual T* get( const std::string& path ) = 0;
      // Loads and parses the given data file.

    virtual void release( T* data ) = 0;
      // Releases the given data.
};

// CONSTRUCTORS
template <typename T>
inline
IDataFactory<T>::~IDataFactory()
{
}

} // End nspc sgda

} // End nspc StevensDev


#endif
