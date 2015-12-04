// resource_manager.h
//
// The resource manager handles loading files and managing resources based
// on reference counts. This does not create the engine specific wrappers
// for the assorted resources which must be created independently.
#ifndef INCLUDED_RESOURCE_MANAGER
#define INCLUDED_RESOURCE_MANAGER

#include "engine/assets/data_manager.h"
#include "engine/assets/config_data_factory.h"
#include "engine/assets/image_data_factory.h"
#include "engine/containers/map.h"
#include "engine/data/json_entity.h"
#include "engine/scene/itickable.h"
#include <fstream>
#include <SFML/Graphics/Texture.hpp>

namespace StevensDev
{

namespace sgda
{

struct ImageTag
{
};

struct ConfigTag
{
};

class ResourceManager : public sgds::ITickable
{
  private:
    // GLOBAL CONSTANTS
    static ResourceManager d_manager;
      // The singleton resource manager.

    static ImageDataFactory d_imageFactory;
      // The singleton image factory.

    static ConfigDataFactory d_configFactory;
      // The singleton config factory.

    // MEMBERS
    DataManager<ImageTag, sf::Image> d_images;
      // The image resources.

    DataManager<ConfigTag, sgdd::JsonEntity> d_configs;
      // The configuration resources.

    ResourceDatabase* d_database;
      // The resource database

    unsigned int d_frame;
      // Frame counter for scheduling garbage collection.

    // CONSTRUCTORS
    ResourceManager();
      // Constructs a new resource manager without a database.

    ResourceManager( ResourceDatabase* database );
      // Constructs a new resource manager that is using the given database.

    ResourceManager( const ResourceManager& manager );
      // Constructs a copy of the given manager.

    // OPERATORS
    ResourceManager& operator=( const ResourceManager& manager );
      // Makes this a copy of another resource manager.
  public:
    // GLOBAL FUNCTIONS
    static ResourceManager& inst();
      // Gets a reference to the resource manager.

    static void init( ResourceDatabase* db );
      // Constructs a resource manager that is using the given database.

    // CONSTRUCTORS
    ~ResourceManager();
      // Destructs the resource manager.

    // MEMBER FUNCTIONS
    const Handle<ImageTag>& getImageHandle( const ResourceID& rid );
      // Gets a handle for the image with the given id.

    const Handle<ConfigTag>& getConfigHandle( const ResourceID& rid );
      // Gets a handle for the configuration with the given id.

    void loadImage( const ResourceID& rid, bool isPermanent = false );
      // Loads the image with the given id.

    void loadConfig( const ResourceID& rid, bool isPermanent = false  );
      // Loads the configuration with the given id.

    sf::Image& get( const Handle<ImageTag>& handle );
      // Gets the image for the given handle.

    const sf::Image& get( const Handle<ImageTag>& handle ) const;
      // Gets the image for the given handle.

    sgdd::JsonEntity& get( const Handle<ConfigTag>& handle );
      // Gets the configuration for the given handle.

    const sgdd::JsonEntity& get( const Handle<ConfigTag>& handle ) const;
      // Gets the configuration for the given handle.

    void release( const Handle<ImageTag>& handle );
      // Releases the image handle.

    void release( const Handle<ConfigTag>& handle );
      // Releases the configuration handle.

    bool isImageLoaded( const ResourceID& rid ) const;
      // Checks if the image with the given id is loaded.

    bool isConfigLoaded( const ResourceID& rid ) const;
      // Checks if the config with the given id is loaded.

    virtual void preTick();
      // Does nothing.

    virtual void tick( float dtS );
      // Does nothing.

    virtual void postTick();
      // Cleans up resources.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const ResourceManager& manager )
{
    return stream << "{  }";
}

// GLOBAL FUNCTIONS
inline
ResourceManager& ResourceManager::inst()
{
    return d_manager;
}

inline
void ResourceManager::init( ResourceDatabase* db )
{
    d_manager = ResourceManager( db );
}

// CONSTRUCTORS
inline
ResourceManager::ResourceManager()
    : d_images( nullptr, &d_imageFactory ),
      d_configs( nullptr, &d_configFactory ),
      d_frame( 0 )
{
}

inline
ResourceManager::ResourceManager( ResourceDatabase* database )
    : d_database( database ), d_images( database, &d_imageFactory ),
      d_configs( database, & d_configFactory ), d_frame( 0 )
{
}

inline
ResourceManager::ResourceManager( const ResourceManager& manager )
    : d_images( manager.d_images ), d_configs( manager.d_configs ),
      d_frame( manager.d_frame )
{
}

inline
ResourceManager::~ResourceManager()
{
}

// OPERATORS
inline
ResourceManager& ResourceManager::operator=( const ResourceManager& manager )
{
    d_images = manager.d_images;
    d_configs = manager.d_configs;
    d_frame = manager.d_frame;

    return *this;
}

// MEMBER FUNCTIONS
inline
const Handle<ImageTag>&
ResourceManager::getImageHandle( const ResourceID& rid )
{
    return d_images.acquire( rid );
}

inline
const Handle<ConfigTag>& ResourceManager::getConfigHandle(
    const ResourceID& rid )
{
    return d_configs.acquire( rid );
}

inline
void ResourceManager::loadImage( const ResourceID& rid, bool isPermanent  )
{
    d_images.load( rid , isPermanent );
}

inline
void ResourceManager::loadConfig( const ResourceID& rid, bool isPermanent )
{
    d_configs.load( rid, isPermanent );
}

inline
sf::Image& ResourceManager::get( const Handle<ImageTag>& handle )
{
    return d_images.dereference( handle );
}

inline
const sf::Image& ResourceManager::get( const Handle<ImageTag>& handle ) const
{
    return d_images.dereference( handle );
}

inline
sgdd::JsonEntity& ResourceManager::get( const Handle<ConfigTag>& handle )
{
    return d_configs.dereference( handle );
}

inline
const sgdd::JsonEntity&
ResourceManager::get( const Handle<ConfigTag>& handle ) const
{
    return d_configs.dereference( handle );
}

inline
void ResourceManager::release( const Handle<ImageTag>& handle )
{
    d_images.release( handle );
}

inline
void ResourceManager::release( const Handle<ConfigTag>& handle )
{
    d_configs.release( handle );
}

inline
bool ResourceManager::isImageLoaded( const ResourceID& rid ) const
{
    return d_images.isLoaded( rid );
}

inline
bool ResourceManager::isConfigLoaded( const ResourceID& rid ) const
{
    return d_configs.isLoaded( rid );
}

inline
void ResourceManager::preTick()
{
    // do nothing
}

inline
void ResourceManager::tick( float dtS )
{
    // do nothing
}

} // End nspc sgda

} // End nspc StevensDev

#endif
