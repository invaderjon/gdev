// resource_database.h
//
// This class maintains a running list of all known resources available in the
// game can be used to query basic information regarding the resource such as
// file path, size, last modified timestamp, created timestamp, and version
// information.
//
// The primary goals of this class are to:
// o prevent duplicate RID's
// o decouple RID and resource typeto simplify patching and hot-swapping
// o provide an easy patch system
// o simplify hot-swapping
//
#ifndef INCLUDED_RESOURCE_DATABASE
#define INCLUDED_RESOURCE_DATABASE

#include <engine/containers/fast_map.h>
#include "engine/containers/dynamic_array.h"

namespace StevensDev
{

namespace sgda
{

typedef unsigned int ResourceID;
  // Defines a resource identifier.

// Provides basic information about a resource.

// Note: Most of the fields are not yet implemented. Only rid and path
// are currently available.
struct ResourceEntry
{
    ResourceID id;
      // The resource's id

    std::string path;
      // The resource's file path.

    size_t fileSize;
      // The resource's file size.

    unsigned long lastModified;
      // When the file was last modified (epoch timestamp).

    unsigned long dateCreated;
      // When the file was created (epoch timestamp).

    unsigned int version;
      // The version number.
};

class ResourceDatabase
{
  private:
    // MEMBERS
    sgdc::FastMap<ResourceEntry> d_entries;
      // The list of known resources.

  public:
    // CONSTRUCTORS
    ResourceDatabase();
      // Creates a new resource database.

    ResourceDatabase( const ResourceDatabase& database );
      // Makes a copy of the given database.

    ~ResourceDatabase();
      // Destructs the database.

    // OPERATORS
    ResourceDatabase& operator=( const ResourceDatabase& database );
      // Makes this a copy of the given database.

    // MEMBER FUNCTIONS
    void addPackageResources( const std::string& dbPath, ResourceID offset );
      // Loads the package's resources' information into the database.
      //
      // All duplicate items will be replaced.
      // All new items will use the specified id offset.

    const ResourceEntry& getEntry( ResourceID rid ) const;
      // Get the entry will the given id.
      //
      // If the requested resource is not found this wil return a resource
      // entry with a different id.

    bool hasEntryFor( ResourceID rid ) const;
      // Checks if there is an entry for the given resource id.
};

// CONSTRUCTORS
inline
ResourceDatabase::ResourceDatabase() : d_entries()
{
}

inline
ResourceDatabase::ResourceDatabase( const ResourceDatabase& database )
    : d_entries( database.d_entries )
{
}

inline
ResourceDatabase::~ResourceDatabase()
{
}

// OPERATORS
inline
ResourceDatabase& ResourceDatabase::operator=(
    const ResourceDatabase& database )
{
    d_entries = database.d_entries;

    return *this;
}

// MEMBER FUNCTIONS
inline
const ResourceEntry& ResourceDatabase::getEntry( ResourceID rid ) const
{
    return d_entries[rid];
}

inline
bool ResourceDatabase::hasEntryFor( ResourceID rid ) const
{
    return d_entries.has( rid );
}

} // End nspc sgda

} // End nspc StevensDev

#endif