// iserializable.h
#ifndef INCLUDED_ISERIALIZABLE
#define INCLUDED_ISERIALIZABLE

#include <iosfwd>

namespace StevensDev
{

namespace sgdd
{

class ISerializable
{
  public:
    // CONSTRUCTORS
    virtual ~ISerializable() = 0;
      // Destructs the serializable object.

    // MEMBER FUNCTIONS
    virtual std::ostream& serialize( std::ostream& out ) const = 0;
      // Writes the json representation of the object to the output stream.
};

// CONSTRUCTORS
inline
ISerializable::~ISerializable()
{
}

} // End nspc sgdd

} // End nspc StevensDev

#endif
