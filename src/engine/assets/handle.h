// handle.h
#ifndef INCLUDED_HANDLE
#define INCLUDED_HANDLE

#include <engine/data/json_printer.h>
#include <iostream>

namespace StevensDev
{

namespace sgda
{

template <typename Tag>
class Handle
{
  private:
    // MEMBERS
    unsigned short d_index;
      // The resource index.

    unsigned short d_checkValue;
      // The check value.

    Tag d_tag;
      // The tag.

  public:
    static const Handle<Tag> NULL_HANDLE;
      // The handle equivalent of a null pointer.

    // CONSTRUCTORS
    Handle();
      // Constructs a null handle.

    Handle( unsigned short index, unsigned short checkValue );
      // Constructs a handle with the given index and check value.

    Handle( const Handle<Tag>& handle );
      // Constructs a copy of the given handle.

    ~Handle();
      // Destructs the handle.

    // OPERATORS
    Handle<Tag>& operator=( const Handle<Tag>& handle );
      // Makes this a copy of the other handle.

    bool operator==( const Handle<Tag>& handle ) const;
      // Checks if two handles are equivalent.

    bool operator!=( const Handle<Tag>& handle ) const;
      // checks if two handles are not equivalent.

    // ACCESSOR FUNCTIONS
    unsigned short index() const;
      // Gets the data index.

    unsigned short checkValue() const;
      // Gets the check value.
};

// CONSTANTS
template <typename T>
const Handle<T> Handle<T>::NULL_HANDLE = Handle();

// FREE OPERATORS
template <typename T>
inline
std::ostream& operator<<( std::ostream& stream, const Handle<T>& handle )
{
    sgdd::JsonPrinter p( stream );

    p.open().print( "index", handle.index() )
            .print( "checkValue", handle.checkValue() )
            .close();

    return stream;
}


// CONSTRUCTORS
template <typename T>
inline
Handle<T>::Handle() : d_index( ( unsigned short )-1 ),
                      d_checkValue( 0 ),
                      d_tag()
{
}

template <typename T>
inline
Handle<T>::Handle( unsigned short index, unsigned short checkValue )
    : d_index( index ), d_checkValue( checkValue ), d_tag()
{
}

template <typename T>
Handle<T>::Handle( const Handle<T>& handle )
    : d_index( handle.d_index ), d_checkValue( handle.d_checkValue ), d_tag()
{
}

template <typename T>
inline
Handle<T>::~Handle()
{
}

// OPERATORS
template <typename T>
inline
Handle<T>& Handle<T>::operator=( const Handle<T>& handle )
{
    d_index = handle.d_index;
    d_checkValue = handle.d_checkValue;

    return *this;
}

template <typename T>
inline
bool Handle<T>::operator==( const Handle<T>& handle ) const
{
    return d_index == handle.d_index && d_checkValue == handle.d_checkValue;
}

template <typename T>
inline
bool Handle<T>::operator!=( const sgda::Handle<T>& handle ) const
{
    return d_index != handle.d_index || d_checkValue != handle.d_checkValue;
}

// ACCESSOR FUNCTIONS
template <typename T>
inline
unsigned short Handle<T>::index() const
{
    return d_index;
}

template <typename T>
inline
unsigned short Handle<T>::checkValue() const
{
    return d_checkValue;
}

} // End nspc sgda

} // End nspc StevensDev

#endif