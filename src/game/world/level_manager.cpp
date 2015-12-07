// level_manager.cpp
#include "level_manager.h"

namespace StevensDev
{

namespace mgw
{

// GLOBALS
LevelFactory LevelManager::d_factory = LevelFactory();


void LevelManager::prepare( const LevelID& lid )
{
    using namespace sgda;

    if ( !d_levelResources.has( lid ) || d_handles.has( lid ) )
    {
        return;
    }

    const ResourceID& rid = d_levelResources[lid];
    ResourceManager& mgr = ResourceManager::inst();

    mgr.loadLevelData( rid );

    Handle<LevelDataTag> data = mgr.getLevelDataHandle( rid );
    Handle<LevelTag> handle = d_handleManager.acquire();

    Level inst;

    mgr.loadLevelData( rid, false );

    Level* level = d_factory.get( data );

    d_handleManager.set( handle, level );
    d_handles[lid] = handle;
    d_dataHandles[lid] = data;
}

void LevelManager::open( const LevelID& lid )
{
    using namespace sgda;

    if ( !d_levelResources.has( lid ) || !d_handles.has( lid ) )
    {
        return;
    }

    Level* level = d_handleManager.get( d_handles[lid] );

    level->open();
}

void LevelManager::remove( const LevelID& lid )
{
    using namespace sgda;

    if ( !d_levelResources.has( lid ) || !d_handles.has( lid ) )
    {
        return;
    }

    ResourceManager& mgr = ResourceManager::inst();

    Handle<LevelTag> handle = d_handles[lid];
    Handle<LevelDataTag> data = d_dataHandles[lid];

    Level* level = d_handleManager.release( handle );

    level->close();

    d_factory.release( level );
    level = nullptr;

    mgr.release( data );

    d_handles.remove( lid );
    d_dataHandles.remove( lid );
}

} // End nspc mgw

} // End nspc StevensDev