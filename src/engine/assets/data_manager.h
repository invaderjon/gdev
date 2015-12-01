// data_manager.h
#ifndef INCLUDED_DATA_MANAGER
#define INCLUDED_DATA_MANAGER

#include <engine/assets/handle_manager.h>
#include <engine/assets/idata_factory.h>
#include <engine/containers/map.h>
#include <engine/scene/itickable.h>

namespace StevensDev
{

namespace sgda
{

template <typename Tag, typename Data>
class DataManager
{
  private:
    // STRUCTURES
    struct Descriptor
    {
        std::string path;
          // The file path.

        size_t size;
          // The file size.
    };

    struct Object
    {
        Handle<Tag> handle;
          // The object's handle.

        unsigned int references;
          // The reference count.

        bool isPermanent;
          // If the resource should never be released during cleaning.

        bool isLoaded;
          // If the object is loaded.
    };

    // MEMBERS
    IDataFactory<Data>* d_factory;
      // Loads and constructs the data given a file path.

    HandleManager<Tag, Data> d_handleManager;
      // The handle manager.

    sgdc::Map<Object> d_objects;
      // Maps the file paths to the associated handles along with some garbage
      // collection information.
      //
      // This can also be used to check if a file already has a handle yet.

    sgdc::DynamicArray<Descriptor> d_descriptors;
      // Maps resource ids to the associated file descriptors.

    sgdc::DynamicArray<Object> d_oldVersions;
      // A list of old objects for when objects are reloaded during
      // hot-swapping.
      // todo: use this

    Object& get( unsigned int rid );
      // Gets the resource with the given path.

    Object* find( const Handle<Tag>& handle );
      // Finds the object with the given handle or null if it could not be
      // found.
  public:
    // CONSTRUCTORS
    DataManager();
      // Constructs a new data manager without a factory.
      //
      // This manager will be incapable of loading new items.

    explicit DataManager( IDataFactory<Data>* factory );
      // Constructs a new data manager that uses the given factory to load
      // files.

    DataManager( const DataManager<Tag, Data>& manager );
      // Constructs a copy of the given manager.

    ~DataManager();
      // Destructs a data manager.

    // OPERATORS
    DataManager<Tag, Data>& operator=( const DataManager<Tag, Data>& manager );
      // Makes this a copy of the give manager.

    // MEMBER FUNCTIONS
    void addFile( unsigned int rid, const std::string& path, size_t size );
      // Add to the list of known files

    const Handle<Tag>& acquire( unsigned int rid );
      // Gets a handle for the resource with the given id.
      //
      // If a handle does not exist for the resource it will be created.

    void load( unsigned int rid, bool isPermanent = false );
      // Loads the resource with the given id.
      //
      // Permanence only affects the garbage collector therefore permanent
      // objects can be unloaded by manually calling unload.

    Data& dereference( const Handle<Tag>& handle );
      // Gets the data associated with the given handle.
      //
      // Undefined behavior if the handle is invalid.

    const Data& dereference( const Handle<Tag>& handle ) const;
      // Gets the data associated with the given handle.
      //
      // Undefined behavior if the handle is invalid.

    bool unload( unsigned int rid );
      // Unloads the resource with the given id and returns if it was
      // successful.

    bool release( const Handle<Tag>& handle );
      // Releases the handle and returns it to the pool.

    bool isLoaded( unsigned int rid ) const;
      // Checks if the given resource is loaded.

    bool hasHandle( unsigned int rid ) const;
      // Checks if the resource with the given id has a pre-existing handle
      // associated with it.

    void clean();
      // Perform garbage collection.
};

// FREE OPERATORS
template <typename T, typename D>
inline
std::ostream& operator<<( std::ostream& stream, const DataManager<T, D>& mgr )
{
    return stream << "{  }";
}

// CONSTRUCTORS
template <typename T, typename D>
inline
DataManager<T, D>::DataManager() : d_factory( nullptr ), d_handleManager(),
                                   d_objects(), d_descriptors(),
                                   d_oldVersions()
{
}

template <typename T, typename D>
inline
DataManager<T, D>::DataManager( IDataFactory<D>* factory )
    : d_factory( factory ), d_handleManager(), d_objects(), d_descriptors(),
      d_oldVersions()
{
}

template <typename T, typename D>
inline
DataManager<T, D>::DataManager( const sgda::DataManager<T, D>& manager )
    : d_factory( manager.d_factory ),
      d_handleManager( manager.d_handleManager ),
      d_objects( manager.d_objects ), d_descriptors( manager.d_descriptors ),
      d_oldVersions( manager.d_oldVersions )
{
}

template <typename T, typename D>
inline
DataManager<T, D>::~DataManager()
{
}

// OPERATORS
template <typename T, typename D>
inline
DataManager<T, D>& DataManager<T, D>::operator=(
    const sgda::DataManager<T, D>& manager )
{
    d_factory = manager.d_factory;
    d_handleManager = manager.d_handleManager;
    d_objects = manager.d_objects;
    d_descriptors = manager.d_descriptors;
    d_oldVersions = manager.d_oldVersions;

    return *this;
}

// MEMBER FUNCTIONS
template <typename T, typename D>
void DataManager<T, D>::addFile( unsigned int rid,
                                 const std::string& path, size_t size )
{
    Descriptor desc;
    desc.path = path;
    desc.size = size;
    d_descriptors.insertAt( rid, desc );
}

template <typename T, typename D>
const Handle<T>& DataManager<T, D>::acquire( unsigned int rid )
{
    Object& obj = get( rid );
    ++obj.references;
    return obj.handle;
}

template <typename T, typename D>
void DataManager<T, D>::load( unsigned int rid, bool isPermanent )
{
    Object& obj = get( rid );

    if ( obj.isLoaded )
    {
        return;
    }

    obj.isPermanent = isPermanent;
    obj.isLoaded = true;

    const Descriptor& desc = d_descriptors[rid];
    d_handleManager.set( obj.handle, d_factory->get( desc.path ) );
}

template <typename T, typename D>
inline
D& DataManager<T, D>::dereference( const Handle<T>& handle )
{
    return *d_handleManager.get( handle );
}

template <typename T, typename D>
inline
const D& DataManager<T, D>::dereference( const Handle<T>& handle ) const
{
    return *d_handleManager.get( handle );
}

template <typename T, typename D>
bool DataManager<T, D>::unload( unsigned int rid )
{
    Object& obj = get( rid );

    if ( !obj.isLoaded )
    {
        return false;
    }

    obj.isLoaded = false;

    D* data = d_handleManager.get( obj.handle );
    d_handleManager.set( obj.handle, nullptr );
    d_factory->release( data );
    return true;
}

template <typename T, typename D>
bool DataManager<T, D>::release( const Handle<T>& handle )
{
    Object* obj = find( handle );
    if ( obj && obj->references )
    {
        --(obj->references);
        return true;
    }

    return false;
}

template <typename T, typename D>
bool DataManager<T, D>::isLoaded( unsigned int rid ) const
{
    assert( rid < d_descriptors.size() );

    const Descriptor& desc = d_descriptors[rid];

    return d_objects.has( desc.path ) && d_objects[desc.path].isLoaded;
}

template <typename T, typename D>
bool DataManager<T, D>::hasHandle( unsigned int rid ) const
{
    assert( rid < d_descriptors.size() );

    return d_objects.has( d_descriptors[rid].path );
}

template <typename T, typename D>
void DataManager<T, D>::clean()
{
    // todo: create master list of all items (old or current version)

    Object obj;
    D* data;
    unsigned int i;

    // todo: decide: delete the handles or just unload the content?
    for ( i = 0; i < d_objects.keys().size(); ++i )
    {
        const std::string& key = d_objects.keys()[i];
        obj = d_objects[key];
        if ( obj.references <= 0 && !obj.isPermanent )
        {
            obj = d_objects.remove( key );
            data = d_handleManager.release( obj.handle );
            if ( data )
            {
                d_factory->release( data );
            }
            i--;
        }
    }

    for ( i = 0; i < d_oldVersions.size(); ++i )
    {
        if ( d_oldVersions[i].references <= 0 )
        {
            obj = d_oldVersions.removeAt( i );
            data = d_handleManager.release( obj.handle );
            if ( data )
            {
                d_factory->release( data );
            }
            i--;
        }
    }

    // todo: sort master list differently based on a parameter

    // todo: unload items at the bottom of the list until the urgency level
    //       is resolved
}

// HELPER FUNCTIONS
template <typename T, typename D>
typename DataManager<T, D>::Object&
DataManager<T, D>::get( unsigned int rid )
{
    assert( rid < d_descriptors.size() );

    const Descriptor& desc = d_descriptors[rid];

    if ( !d_objects.has( desc.path ) )
    {
        Object obj;
        obj.handle = d_handleManager.acquire();
        obj.references = 0;
        obj.isPermanent = false;
        obj.isLoaded = false;
        d_objects[desc.path] = obj;
    }

    return d_objects[desc.path];
}

template <typename T, typename D>
typename DataManager<T, D>::Object*
DataManager<T, D>::find( const Handle<T>& handle )
{
    Object* obj;
    unsigned int i;

    for ( i = 0, obj = nullptr;
          obj == nullptr && i < d_objects.values().size();
          ++i )
    {
        if ( d_objects.values()[i].handle == handle )
        {
            obj = ( Object* )&d_objects.values()[i];
        }
    }

    for ( i = 0; obj == nullptr && i < d_oldVersions.size(); ++i )
    {
        if ( d_oldVersions[i].handle == handle )
        {
            obj = &d_oldVersions[i];
        }
    }

    return obj;
}

} // End nspc sgda

} // End nspc StevensDev

#endif