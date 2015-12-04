// game.h
#ifndef INCLUDED_GAME_ENGINE
#define INCLUDED_GAME_ENGINE

#include "engine/assets/resource_database.h"
#include "engine/containers/dynamic_array.h"
#include "engine/rendering/renderer.h"
#include <string>

namespace StevensDev
{

namespace mgb
{

class Game
{
  private:
    // MEMBERS
    sgda::ResourceDatabase d_resources;
      // The game's resource database.

    sgdr::Renderer d_renderer;
      // The game's renderer.

    sgdc::DynamicArray<unsigned int> d_events;
      // The list of broadcasted events that are waiting to be consumed.

    std::string d_name;
      // The instance's name.

    bool d_isRunning;
      // If the game is still running.

    bool d_isDead;
      // If the game has been shutdown or encountered an error.
      //
      // An game that isn't running may not necessarily be dead. So
      // thus this exists to make that distinction.

    // CONSTRUCTORS
    Game( const Game& game );
      // Making copies of the game leads to undefined behavior.

    // OPERATORS
    Game& operator=( const Game& game );
      // Making copies of the game leads to underfned behavior.

  public:
    // ENUMERATIONS
    enum EngineEvent
    {
        // null event
        NO_EVENT = 0,

        // STATUSES
        STATUS_INITIALIZING,
          // The game has begun initializing.

        STATUS_INITIALIZED,
          // The game has initialized.

        STATUS_STARTED,
          // The game has started.

        STATUS_CHANGED_STATE,
          // The game has changed states.

        STATUS_SHUTTING_DOWN,
          // The game has begun shutting down.

        STATUS_SHUT_DOWN,
          // The game has shut down.

        STATUS_LOW_MEMORY_RESOLVED,
          // A low memory issue has been resolved.

        // WARNINGS
        WARNING_LOW_MEMORY,
          // The game is using a dangerous amount of memory.

        // CRITICAL
        CRITICAL_LOW_MEMORY,
          // The game could fail from memory unavailability.

        // FATAL
        FATAL_RESOURCE_NOT_FOUND
          // The game crashed because a vital resource could not be found.
          //
          // This typically means a core system configuration is missing.
    };

    // CONSTRUCTORS
    Game();
      // Constructs a new game instance.

    ~Game();
      // Destroys the game.
      //
      // The game must be shutdown prior to being destroyed for data safety
      // purposes. Failing to do so will raise an exception.

    const std::string& name() const;
          // Gets the name of the game.

    // MEMBER FUNCTIONS
    void initialize( const std::string& name );
      // Prepares the game for startup.
      //
      // Loads core game configurations and prepares the core subsystems.
      //
      // Upon completion the game will broadcast an ENGINE_INITIALIZED event.

    void startup();
      // Starts up the game.
      //
      // This will begin loading game files and switches to the
      // given initial state.
      //
      // The game will broadcast an ENGINE_STARTING event but does not
      // broadcast an event force once it has started as the significance of
      // said event is determined by the game.

    void update();
      // Performs one update cycle.
      //
      // Behavior is undefined if the game has not been initialized and
      // started.

    void shutdown();
      // Shutdowns the game.

    EngineEvent poll();
      // Polls for an event.
      //
      // If there are no events to consume then this will return NO_EVENT.

    bool isRunning() const;
      // Checks if the game is running.
};

// CONSTRUCTORS
inline
Game::Game() : d_resources(), d_renderer(), d_events(),
               d_isRunning( false ), d_isDead( false )
{
}

inline
Game::Game( const Game& game )
    : d_resources(), d_renderer(), d_events(), d_isRunning( false ),
      d_isDead( true )
{
    throw std::runtime_error( "The game instance cannot be copied!" );
}

inline
Game::~Game()
{
    assert( d_isDead );
}

// OPERATORS
inline
Game& Game::operator=( const Game& game )
{
    d_isRunning = false;
    d_isDead = true;
    throw std::runtime_error( "The game instance cannot be copied!" );
}

// ACCESSOR FUNCTIONS
inline
const std::string& Game::name() const
{
    return d_name;
}

// MEMBER FUNCTIONS
inline
bool Game::isRunning() const
{
    return d_isRunning && !d_isDead;
}

}  // End nspc mgb

}  // End nspc StevensDev

#endif
