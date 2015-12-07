// level.h
//
// Defines a level.
#ifndef INCLUDED_LEVEL
#define INCLUDED_LEVEL

#include <engine/assets/handle.h>
#include <engine/assets/resource_manager.h>
#include <engine/data/json_entity.h>
#include <engine/scene/scene.h>
#include "game/objects/actor.h"
#include "game/world/board.h"

namespace StevensDev
{

namespace mgw
{

typedef std::string LevelID;
  // Defines a level id.

struct LevelTag
{
};

class Level : public sgds::ITickable
{
  private:
    sgdc::DynamicArray<sgda::Handle<sgda::ConfigTag>> d_configs;
      // The set of configurations associated with the level.

    sgdc::DynamicArray<sgda::Handle<sgda::TextureTag>> d_textures;
      // The set of textures associated with the level.

    sgdc::DynamicArray<sgds::ITickable*> d_controllers;
      // The set of controllers associated with the level.

    sgdc::DynamicArray<mgo::Actor*> d_actors;
      // The set of controllers associated with the level.

    sgdc::DynamicArray<mgo::Actor*> d_newActors;
      // Actors waiting to be added.

    sgdc::DynamicArray<sgds::ITickable*> d_newControllers;
      // Controllers waiting to be added.

    mgw::Board d_board;
      // The board used by the level.

    std::string d_name;
      // The level name.

  public:
    // CONSTRUCTORS
    Level();
      // Constructs a new blank level.

    Level( const Level& level );
      // Constructs a copy of the level.

    ~Level();
      // Destructs a level.

    // OPERATORS
    Level& operator=( const Level& level );
      // Makes this a copy of the given level.

    // ACCESSOR FUNCTIONS
    const mgw::Board& board() const;
      // Gets the level's game board.

    // MUTATOR FUNCTIONS
    void setBoard( mgw::Board board );
      // Sets the game board.

    // MEMBER FUNCTIONS
    void addController( sgds::ITickable* controller );
      // Add a controller to the level.

    void addActor( mgo::Actor* actor );
      // Adds an actor to the level.

    void open();
      // Opens the level.

    void close();
      // Close the level and release the resources.

    virtual void preTick();
      // Prepares for the next update.

    virtual void tick( float dtS );
      // Performs the update.

    virtual void postTick();
      // Cleans up after the update.
};

// CONSTRUCTORS
inline
Level::Level() : d_configs(), d_textures(), d_controllers(), d_actors(),
                 d_name()
{
}

inline
Level::Level( const Level& level )
    : d_configs( level.d_configs ), d_textures( level.d_textures ),
      d_controllers( level.d_controllers ), d_actors( level.d_actors ),
      d_name( level.d_name )
{
}

inline
Level::~Level()
{
}

// OPERATORS
inline
Level& Level::operator=( const Level& level )
{
    d_configs = level.d_configs;
    d_textures = level.d_textures;
    d_controllers = level.d_controllers;
    d_actors = level.d_actors;
    d_name = level.d_name;

    return *this;
}

// ACCESSOR FUNCTIONS
inline
const mgw::Board& Level::board() const
{
    return d_board;
}

// MUTATOR FUNCTIONS
inline
void Level::setBoard( mgw::Board board )
{
    d_board = board;
}

// MEMBER FUNCTIONS
inline
void Level::open()
{
    sgds::Scene& s = sgds::Scene::inst();

    mgo::Actor* actor;
    while ( d_newActors.size() > 0 )
    {
        actor = d_newActors.popFront();
        s.addRenderable( &actor->sprite() );
        d_actors.push( actor );
    }

    sgds::ITickable* controller;
    while ( d_newControllers.size() > 0 )
    {
        controller = d_newControllers.popFront();
        s.addTickable( controller );
        d_controllers.push( controller );
    }
}

inline
void Level::addActor( mgo::Actor* actor )
{
    d_newActors.push( actor );
}

inline
void Level::addController( sgds::ITickable* controller )
{
    d_newControllers.push( controller );
}

inline
void Level::preTick()
{
    // does nothing
}

inline
void Level::tick( float dtS )
{
    // does nothing
}


} // End nspc mgw

} // End nspc StevensDev

#endif
