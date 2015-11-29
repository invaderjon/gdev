// handle_manager.h
#ifndef INCLUDED_HANDLE_MANAGER
#define INCLUDED_HANDLE_MANAGER

#include <engine/assets/handle.h>
#include <engine/containers/dynamic_array.h>

namespace StevensDev
{

namespace sgda
{

template <typename Tag, typename Data>
class HandleManager
{
  private:
    // MEMBERS
    sgdc::DynamicArray<Handle<Tag>> d_handles;
      // The set of handles.

    sgdc::DynamicArray<Data*> d_data;
      // The data associated with the handles.

    sgdc::DynamicArray<unsigned short> d_free;
      // The free indices.

  public:
    // CONSTRUCTORS
    HandleManager();
      // Constructs a new handle manager.

    HandleManager( const HandleManager<Tag, Data>& manager );
      // Constructs a copy of the given manager.

    ~HandleManager();
      // Destructs the manager.

    // OPERATORS
    HandleManager<Tag, Data>& operator=(
        const HandleManager<Tag, Data>& manager );
      // Makes this a copy of the other manager.

    // MEMBER FUNCTIONS
    const Handle<Tag>& acquire();
      // Gets a free handle that can be assigned to new data.
      //
      // This gets a handle that isn't already in use.

    Data* release( const Handle<Tag>& handle );
      // Releases the given handle and returns the associated data.
      //
      // If the handle is invalid this returns null. This may also return
      // null if no data was set for the handle.

    Data* get( const Handle<Tag>& handle );
      // Gets the data associated with the given handle.
      //
      // This returns null if the handle is invalid or no data was set.

    const Data* get( const Handle<Tag>& handle ) const;
      // Gets the data associated with the given handle.
      //
      // This returns null if the handle is invalid or no data was set.

    bool set( const Handle<Tag>& handle, Data* data );
      // Sets the data for the given handle and returns if it was
      // successfully set.

    bool has( const Handle<Tag>& handle ) const;
      // Checks if the handle is in use.

    bool hasData( const Handle<Tag>& handle ) const;
      // Checks if the handels is in use and has data non-null mapped to it.
};

// FREE OPERATORS
template <typename T, typename D>
inline
std::ostream& operator<<( std::ostream& stream,
                          const HandleManager<T, D>& manager )
{
    return stream << "{  }";
}

// CONSTRUCTORS
template <typename T, typename D>
inline
HandleManager<T, D>::HandleManager() : d_handles(), d_data(), d_free()
{
}

template <typename T, typename D>
inline
HandleManager<T, D>::HandleManager( const sgda::HandleManager<T, D>& manager )
    : d_handles( manager.d_handles ), d_data( manager.d_data ),
      d_free( manager.d_free )
{
}

template <typename T, typename D>
inline
HandleManager<T, D>::~HandleManager()
{
}

// OPERATORS
template <typename T, typename D>
inline
HandleManager<T, D>& HandleManager<T, D>::operator=(
    const sgda::HandleManager<T, D>& manager )
{
    d_handles = manager.d_handles;
    d_data = manager.d_data;
    d_free = manager.d_free;

    return *this;
}

// MEMBER FUNCTIONS
template <typename T, typename D>
const Handle<T>& HandleManager<T, D>::acquire()
{
    // re-use handles if there is a pre-existing free one (update check value)
    if ( d_free.size() > 0 )
    {
        unsigned short index = d_free.popFront();
        unsigned short check =
            ( unsigned short )( d_handles[index].checkValue() + 1 );

        d_data[index] = nullptr;
        return ( d_handles[index] = Handle<T>( index, check ) );
    }

    d_data.push( nullptr );
    d_handles.push( Handle<T>( ( unsigned short )d_handles.size(), 0 ) );
    return d_handles[d_handles.size() - 1];
}

template <typename T, typename D>
D* HandleManager<T, D>::release( const Handle<T>& handle )
{
    assert( handle.index() < d_handles.size() );

    if ( d_handles[handle.index()] != handle )
    {
        return nullptr;
    }

    D* data = d_data[handle.index()];
    d_data[handle.index()] = nullptr;
    d_free.push( handle.index() );
    return data;
}

template <typename T, typename D>
D* HandleManager<T, D>::get( const Handle<T>& handle )
{
    assert( handle.index() < d_handles.size() );

    if ( d_handles[handle.index()] != handle )
    {
        return nullptr;
    }

    return d_data[handle.index()];
}

template <typename T, typename D>
const D* HandleManager<T, D>::get( const Handle<T>& handle ) const
{
    assert( handle.index() < d_handles.size() );

    if ( d_handles[handle.index()] != handle )
    {
        return nullptr;
    }

    return d_data[handle.index()];
}

template <typename T, typename D>
bool HandleManager<T, D>::set( const Handle<T>& handle, D* data )
{
    assert( handle.index() < d_handles.size() );

    if ( d_handles[handle.index()] != handle )
    {
        return false;
    }

    d_data[handle.index()] = data;
    return true;
}

template <typename T, typename D>
inline
bool HandleManager<T, D>::has( const Handle<T>& handle ) const
{
    return handle.index() < d_handles.size() &&
           d_handles[handle.index()] == handle &&
           !d_free.doesContain( handle.index() );
}

template <typename T, typename D>
inline
bool HandleManager<T, D>::hasData( const Handle<T>& handle ) const
{
    return has( handle ) && d_data[handle.index()] != nullptr;
};

} // End nspc sgda

} // End nspc StevensDev

#endif
