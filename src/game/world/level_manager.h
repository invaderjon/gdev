// level_manager.h
#ifndef INCLUDED_LEVEL_MANAGER
#define INCLUDED_LEVEL_MANAGER

#include <engine/assets/handle_manager.h>
#include <engine/assets/data_manager.h>
#include <engine/assets/resource_manager.h>
#include "game/world/level.h"
#include "game/world/level_factory.h"

namespace StevensDev
{

namespace mgw
{

class LevelManager
{
  private:
    // GLOBALS
    static LevelFactory d_factory;
      // The level factory.

    // MEMBERS
    sgda::HandleManager<LevelTag, Level> d_handleManager;
      // The loaded levels.

    sgdc::Map<sgda::ResourceID> d_levelResources;
      // Maps the level ids to the resource ids.

    sgdc::Map<sgda::Handle<LevelTag>> d_handles;
      // Maps the level ids to their respective handle.

    sgdc::Map<sgda::Handle<sgda::LevelDataTag>> d_dataHandles;
      // Maps the levels to their respective data handles.

  public:
    // CONSTRUCTORS
    LevelManager();
      // Constructs a new level manager.

    LevelManager( const LevelManager& manager );
      // Constructs a copy of the manager.

    ~LevelManager();
      // Destructs the level manager.

    // OPERATORS
    LevelManager& operator=( const LevelManager& manager );
      // Makes this a copy of the given manager.

    // MEMBER FUNCTIONS
    void addLevel( const LevelID& lid, const sgda::ResourceID& dataID );
      // Add a level that uses the specified resource data.

    void prepare( const LevelID& lid );
      // Loads and caches the level data.

    void open( const LevelID& lid );;
      // Changes to the specified level.

    void remove( const LevelID& lid );
      // Unloads the level.
};

inline
LevelManager::LevelManager()
    : d_handleManager(), d_levelResources(), d_handles(),
      d_dataHandles()
{
}

inline
LevelManager::LevelManager( const LevelManager& manager )
    : d_handleManager( manager.d_handleManager ),
      d_levelResources( manager.d_levelResources ),
      d_handles( manager.d_handles ),
      d_dataHandles( manager.d_dataHandles )
{
}

inline
LevelManager::~LevelManager()
{
}

inline
LevelManager& LevelManager::operator=( const LevelManager& manager )
{
    d_handleManager = manager.d_handleManager;
    d_levelResources = manager.d_levelResources;
    d_handles = manager.d_handles;
    d_dataHandles = manager.d_dataHandles;

    return *this;
}

inline
void LevelManager::addLevel( const LevelID& lid,
                             const sgda::ResourceID& dataID )
{
    d_levelResources[lid] = dataID;
}

} // End nspc mgw

} // End nspc StevensDev

#endif
