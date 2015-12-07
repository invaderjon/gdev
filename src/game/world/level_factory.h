// level_factory.h
#ifndef INCLUDED_LEVEL_FACTORY
#define INCLUDED_LEVEL_FACTORY

#include "game/world/level.h"

namespace StevensDev
{

namespace mgw
{

class LevelFactory
{
  private:
    sgdm::AllocatorGuard<Level> d_alloc;
      // Allocates new levels.

    // HELPER METHODS
    void buildMap( Level* level, const sgdd::JsonEntity& map );
      // Builds the board and world.

    void buildEntities( Level* level, const sgdd::JsonEntity& entities );
      // Builds the entities.

    void buildControllers( Level* level, const sgdd::JsonEntity& controllers );
      // Builds the level controllers.

  public:
    Level* get( const sgda::Handle<sgda::LevelDataTag>& handle );
      // Creates the level using the data.

    void release( Level* level );
      // Release the level and its resources.
};

} // End nspc mgw

} // End nspc StevensDev

#endif
