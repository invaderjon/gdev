// spawn_controller.h
#ifndef INCLUDED_SPAWN_CONTROLLER
#define INCLUDED_SPAWN_CONTROLLER

#include "game/world/level.h"

namespace StevensDev
{

namespace mgc
{

class SpawnController : public sgds::ITickable
{
  private:
    mgw::Level* d_level;
      // The level to spawn into.

    sgdd::JsonEntity d_entity;
      // The entity to spawn.

    unsigned int d_row;
      // The row to spawn the entity into.

    unsigned int d_column;
      // The column to spawn the entity into.

    bool d_hasSpawned;
      // If the entity has been spawned.

  public:
    // CONSTRUCTORS
    SpawnController();
      // Constructs a spawn controller without a spawn type.

    SpawnController( mgw::Level* level, const sgdd::JsonEntity& entity,
                     unsigned int column, unsigned int row );
      // Constructs a spawn controller that creates the given entity at the
      // specified board location.

    SpawnController( const SpawnController& controller );
      // Constructs a copy of the given controller.

    ~SpawnController();
      // Destructs the controller.

    // OPERATORS
    SpawnController& operator=( const SpawnController& controller );
      // Makes this a copy of the given controller.

    // MEMBER FUNCTIONS
    virtual void preTick();
      // Does nothing.

    virtual void tick( float dtS );
      // Spawns entity if it is time.

    virtual void postTick();
      // Does nothing.

    // ABSTRACT FUNCTIONS
    virtual void spawn();
      // Spanws the entity into the world.

    virtual bool shouldSpawn( float dtS ) const;
      // Checks if the controller should spawn the entity.
};

// CONSTRUCTORS
inline
SpawnController::SpawnController() : d_level( nullptr ), d_entity(),
                                     d_row( 0 ), d_column( 0 ),
                                     d_hasSpawned( false )
{
}

inline
SpawnController::SpawnController( mgw::Level* level,
                                  const sgdd::JsonEntity& entity,
                                  unsigned int column,
                                  unsigned int row )
    : d_level( level ), d_entity( entity ), d_column( column ), d_row( row ),
      d_hasSpawned( false )
{
}

inline
SpawnController::SpawnController( const SpawnController& controller )
    : d_level( controller.d_level ), d_entity( controller.d_entity ),
      d_column( controller.d_column ), d_row( controller.d_row ),
      d_hasSpawned( controller.d_hasSpawned )
{
}

inline
SpawnController::~SpawnController()
{
}

// OPERATORS
inline
SpawnController& SpawnController::operator=(
    const SpawnController& controller )
{
    d_level = controller.d_level;
    d_entity = controller.d_entity;
    d_column = controller.d_column;
    d_row = controller.d_row;
    d_hasSpawned = controller.d_hasSpawned;

    return *this;
}

// MEMBER FUNCTIONS
inline
void SpawnController::preTick()
{
    // does nothing
}

inline
void SpawnController::tick( float dtS )
{
    if ( shouldSpawn( dtS ) )
    {
        spawn();
    }
}

inline
void SpawnController::postTick()
{
    // does nothing
}

inline
bool SpawnController::shouldSpawn( float dtS ) const
{
    return !d_hasSpawned;
}

} // End nspc mgc

} // End nspc StevensDev

#endif
