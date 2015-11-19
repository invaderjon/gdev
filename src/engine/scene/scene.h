// scene.h
#ifndef INCLUDED_SCENE
#define INCLUDED_SCENE

#include "../rendering/renderer.h"
#include "itickable.h"

namespace StevensDev
{

namespace sgds
{

class Scene
{
  private:
    static Scene d_instance;
      // The singleton instance.

    sgdr::Renderer* d_renderer;
      // The renderer used to draw the scene.

    sgdc::DynamicArray<ITickable*> d_tickables;
      // The objects in the scene.

    sf::Clock d_clock;
      // The clock used for timing.

    // CONSTRUCTORS
    Scene();
      // Constructs a new scene.

    Scene( const Scene& scene );
      // Constructs a copy of an existing scene.

    // OPERATORS
    Scene& operator=( const Scene& scene );
      // Makes this a copy of another scene.

  public:
    // CONSTRUCTORS
    ~Scene();
      // Destructs the scene.

    // MEMBER FUNCTIONS
    void tick();
      // Updates the scene.

    void addTickable( ITickable* tickable );
      // Adds a tickable to the scene.

    void removeTickable( ITickable* tickable );
      // Removes a tickable from the scene.

    void setRenderer( sgdr::Renderer* renderer );
      // Sets the renderer that is used by the scene.

    // GLOBAL FUNCTIONS
    static Scene& inst();
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream, const Scene& scene )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
Scene::Scene() : d_renderer( nullptr ), d_tickables()
{
}

inline
Scene::Scene( const Scene& scene ) : d_renderer( scene.d_renderer ),
                                     d_tickables( scene.d_tickables )
{
}

inline
Scene::~Scene()
{
    d_renderer = nullptr;
}

// OPERATORS
inline
Scene& Scene::operator=( const Scene& scene )
{
    d_renderer = scene.d_renderer;
    d_tickables = scene.d_tickables;
    return *this;
}

inline
void Scene::addTickable( ITickable* tickable )
{
    d_tickables.push( tickable );
}

inline
void Scene::removeTickable( ITickable* tickable )
{
    d_tickables.remove( tickable );
}

inline
void Scene::setRenderer( sgdr::Renderer* renderer )
{
    d_renderer = renderer;
}

inline
Scene& Scene::inst()
{
    return d_instance;
}

} // End nspc sgds

} // End nspc StevensDev

#endif
