// resource_manager.h
//
// The resource manager handles loading files and managing resources based
// on reference counts. This does not create the engine specific wrappers
// for the assorted resources which must be created independently.
#ifndef INCLUDED_RESOURCE_MANAGER
#define INCLUDED_RESOURCE_MANAGER

#include "engine/assets/data_manager.h"
#include "engine/assets/config_data_factory.h"
#include "texture_data_factory.h"
#include "engine/containers/map.h"
#include "engine/data/json_entity.h"
#include "engine/scene/itickable.h"
#include <fstream>
#include <SFML/Graphics/Texture.hpp>

namespace StevensDev
{

namespace sgda
{

struct TextureTag
{
};

struct ConfigTag
{
};

struct LevelDataTag
{
};

class ResourceManager : public sgds::ITickable
{
  private:
    // GLOBAL CONSTANTS
    static ResourceManager d_manager;
      // The singleton resource manager.

    static TextureDataFactory d_textureFactory;
      // The singleton texture factory.

    static ConfigDataFactory d_configFactory;
      // The singleton config factory.

    // MEMBERS
    DataManager<TextureTag, sf::Texture> d_textures;
      // The texture resources.

    DataManager<ConfigTag, sgdd::JsonEntity> d_configs;
      // The configuration resources.

    DataManager<LevelDataTag, sgdd::JsonEntity> d_levels;
      // The level data resources.

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
    const Handle<TextureTag>& getTextureHandle( const ResourceID& rid );
      // Gets a handle for the texture with the given id.

    const Handle<ConfigTag>& getConfigHandle( const ResourceID& rid );
      // Gets a handle for the configuration with the given id.

    const Handle<LevelDataTag>& getLevelDataHandle( const ResourceID& rid );
      // Gets a handle for the level data with the given id.

    void loadTexture( const ResourceID& rid, bool isPermanent = false );
      // Loads the texture with the given id.

    void loadConfig( const ResourceID& rid, bool isPermanent = false  );
      // Loads the configuration with the given id.

    void loadLevelData( const ResourceID& rid, bool isPermanent = false );
      // Loads the level data with the given id.

    sf::Texture& get( const Handle<TextureTag>& handle );
      // Gets the texture for the given handle.

    const sf::Texture& get( const Handle<TextureTag>& handle ) const;
      // Gets the texture for the given handle.

    sgdd::JsonEntity& get( const Handle<ConfigTag>& handle );
      // Gets the configuration for thse given handle.

    const sgdd::JsonEntity& get( const Handle<ConfigTag>& handle ) const;
      // Gets the configuration for the given handle.

    sgdd::JsonEntity& get( const Handle<LevelDataTag>& handle );
      // Gets the level data for the given handle.

    const sgdd::JsonEntity& get( const Handle<LevelDataTag>& handle ) const;
      // Gets the level data for the given handle.

    void release( const Handle<TextureTag>& handle );
      // Releases the texture handle..

    void release( const Handle<ConfigTag>& handle );
      // Releases the configuration handle.

    void release( const Handle<LevelDataTag>& handle );
      // Releases the level data handle.

    bool isTextureLoaded( const ResourceID& rid ) const;
      // Checks if the texture with the given id is loaded.

    bool isConfigLoaded( const ResourceID& rid ) const;
      // Checks if the config with the given id is loaded.

    bool isLevelDataLoaded( const ResourceID& rid ) const;
      // Checks if the level data with the given id is loaded.

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
    : d_textures( nullptr, &d_textureFactory ),
      d_configs( nullptr, &d_configFactory ),
      d_levels( nullptr, &d_configFactory ),
      d_frame( 0 )
{
}

inline
ResourceManager::ResourceManager( ResourceDatabase* database )
    : d_database( database ), d_textures( database, &d_textureFactory ),
      d_configs( database, & d_configFactory ),
      d_levels( database, &d_configFactory ), d_frame( 0 )
{
}

inline
ResourceManager::ResourceManager( const ResourceManager& manager )
    : d_textures( manager.d_textures ), d_configs( manager.d_configs ),
      d_levels( manager.d_levels ), d_frame( manager.d_frame )
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
    d_textures = manager.d_textures;
    d_configs = manager.d_configs;
    d_levels = manager.d_levels;
    d_frame = manager.d_frame;

    return *this;
}

// MEMBER FUNCTIONS
inline
const Handle<TextureTag>&
ResourceManager::getTextureHandle( const ResourceID& rid )
{
    return d_textures.acquire( rid );
}

inline
const Handle<ConfigTag>& ResourceManager::getConfigHandle(
    const ResourceID& rid )
{
    return d_configs.acquire( rid );
}

inline
const Handle<LevelDataTag>& ResourceManager::getLevelDataHandle(
    const ResourceID& rid )
{
    return d_levels.acquire( rid );
}

inline
void ResourceManager::loadTexture( const ResourceID& rid, bool isPermanent )
{
    d_textures.load( rid , isPermanent );
}

inline
void ResourceManager::loadConfig( const ResourceID& rid, bool isPermanent )
{
    d_configs.load( rid, isPermanent );
}

inline
void ResourceManager::loadLevelData( const ResourceID& rid, bool isPermanent )
{
    d_levels.load( rid, isPermanent );
}

inline
sf::Texture& ResourceManager::get( const Handle<TextureTag>& handle )
{
    return d_textures.dereference( handle );
}

inline
const sf::Texture& ResourceManager::get( const Handle<TextureTag>& handle ) const
{
    return d_textures.dereference( handle );
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
sgdd::JsonEntity& ResourceManager::get( const Handle<LevelDataTag>& handle )
{
    return d_levels.dereference( handle );
}

inline
const sgdd::JsonEntity&
ResourceManager::get( const Handle<LevelDataTag>& handle ) const
{
    return d_levels.dereference( handle );
}

inline
void ResourceManager::release( const Handle<TextureTag>& handle )
{
    d_textures.release( handle );
}

inline
void ResourceManager::release( const Handle<ConfigTag>& handle )
{
    d_configs.release( handle );
}

inline
void ResourceManager::release( const Handle<LevelDataTag>& handle )
{
    d_levels.release( handle );
}

inline
bool ResourceManager::isTextureLoaded( const ResourceID& rid ) const
{
    return d_textures.isLoaded( rid );
}

inline
bool ResourceManager::isConfigLoaded( const ResourceID& rid ) const
{
    return d_configs.isLoaded( rid );
}

inline
bool ResourceManager::isLevelDataLoaded( const ResourceID& rid ) const
{
    return d_levels.isLoaded( rid );
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
