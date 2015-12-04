// data_manager.h
#ifndef INCLUDED_DATA_MANAGER
#define INCLUDED_DATA_MANAGER

#include <engine/assets/handle_manager.h>
#include <engine/assets/idata_factory.h>
#include <engine/containers/map.h>
#include <engine/scene/itickable.h>
#include "resource_database.h"

namespace StevensDev
{

namespace sgda
{

template <typename Tag, typename Data>
class DataManager
{
  private:
    // STRUCTURES
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
    ResourceDatabase* d_database;
      // The resource database that provides basic information about
      // resources.

    IDataFactory<Data>* d_factory;
      // Loads and constructs the data given a file path.

    HandleManager<Tag, Data> d_handleManager;
      // The handle manager.

    sgdc::Map<Object> d_objects;
      // Maps the file paths to the associated handles along with some garbage
      // collection information.
      //
      // This can also be used to check if a file already has a handle yet.

    sgdc::DynamicArray<Object> d_oldVersions;
      // A list of old objects for when objects are reloaded during
      // hot-swapping.
      // todo: use this

    Object& get( ResourceID rid );
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

    DataManager( ResourceDatabase* database, IDataFactory<Data>* factory );
      // Constructs a new data manager that uses the given database and
       // factory to load resources.

    DataManager( const DataManager<Tag, Data>& manager );
      // Constructs a copy of the given manager.

    ~DataManager();
      // Destructs a data manager.

    // OPERATORS
    DataManager<Tag, Data>& operator=( const DataManager<Tag, Data>& manager );
      // Makes this a copy of the give manager.

    // MEMBER FUNCTIONS
    const Handle<Tag>& acquire( ResourceID rid );
      // Gets a handle for the resource with the given id.
      //
      // If a handle does not exist for the resource it will be created.

    void load( ResourceID rid, bool isPermanent = false );
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

    bool unload( ResourceID rid );
      // Unloads the resource with the given id and returns if it was
      // successful.

    bool release( const Handle<Tag>& handle );
      // Releases the handle and returns it to the pool.

    bool isLoaded( ResourceID rid ) const;
      // Checks if the given resource is loaded.

    bool hasHandle( ResourceID rid ) const;
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
DataManager<T, D>::DataManager()
    : d_database( nullptr ), d_factory( nullptr ), d_handleManager(),
      d_objects(), d_oldVersions()
{
}

template <typename T, typename D>
inline
DataManager<T, D>::DataManager( ResourceDatabase* database,
                                IDataFactory<D>* factory )
    : d_database( database ), d_factory( factory ), d_handleManager(),
      d_objects(), d_oldVersions()
{
}

template <typename T, typename D>
inline
DataManager<T, D>::DataManager( const sgda::DataManager<T, D>& manager )
    : d_database( manager.d_database ), d_factory( manager.d_factory ),
      d_handleManager( manager.d_handleManager ),
      d_objects( manager.d_objects ), d_oldVersions( manager.d_oldVersions )
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
    d_database = manager.d_database;
    d_factory = manager.d_factory;
    d_handleManager = manager.d_handleManager;
    d_objects = manager.d_objects;
    d_oldVersions = manager.d_oldVersions;

    return *this;
}

// MEMBER FUNCTIONS
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

    const ResourceEntry& entry = d_database->getEntry( rid );
    d_handleManager.set( obj.handle, d_factory->get( entry.path ) );
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
    if ( !d_database->hasEntryFor( rid ) )
    {
        return false;
    }

    const ResourceEntry& entry = d_database->getEntry( rid );
    return d_objects.has( entry.path ) && d_objects[entry.path].isLoaded;
}

template <typename T, typename D>
bool DataManager<T, D>::hasHandle( unsigned int rid ) const
{
    if ( !d_database->hasEntryFor( rid ) )
    {
        return false;
    }

    return d_objects.has( d_database->getEntry( rid ).path );
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
    assert( !d_database->hasEntryFor( rid ) );

    const ResourceEntry& entry = d_database->getEntry( rid );

    if ( !d_objects.has( entry.path ) )
    {
        Object obj;
        obj.handle = d_handleManager.acquire();
        obj.references = 0;
        obj.isPermanent = false;
        obj.isLoaded = false;
        d_objects[entry.path] = obj;
    }

    return d_objects[entry.path];
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