// ResourceManager.h
#ifndef INCLUDED_RESOURCE_MANAGER
#define INCLUDED_RESOURCE_MANAGER
#include "../containers/map.h"
#include <SFML/Graphics.hpp>

namespace StevensDev
{

namespace sgdg
{

class ResourceManager
{
  private:
    sgdc::Map<sf::Font> d_fonts;
      // The loaded fonts.

  public:
    // CONSTRUCTORS
    ResourceManager();
      // Constructs a new resource manager.

    ResourceManager( const ResourceManager& resMgr );
      // Constructs a copy of another resource manager.

    ResourceManager( ResourceManager&& resMgr );
      // Moves the other resource manager to a new instance.

    ~ResourceManager();
      // Destructs the resource manager.

    // OPERATORS
    ResourceManager& operator=( const ResourceManager& resMgr );
      // Makes this resource manager a copy of another.

    ResourceManager& operator=( ResourceManager&& resMgr );
      // Moves the other resource manager into this instance.

    // FAST LOAD FUNCTIONS
    bool fastLoadFont( std::string name );
      // Loads a font on the calling thread and returns if it was successful.
      // Loading on the same thread is generally discouraged.

    // ACCESSOR FUNCTIONS
    sf::Font& getFont( std::string name );
      // Gets the loaded font.
      //
      // Throws runtime_error when:
      // Resource is not loaded.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream, const ResourceManager& resMgr )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
ResourceManager::ResourceManager() : d_fonts()
{
}

inline
ResourceManager::ResourceManager( const ResourceManager& resMgr )
    : d_fonts( resMgr.d_fonts )
{
}

inline
ResourceManager::ResourceManager( ResourceManager&& resMgr )
    : d_fonts( std::move( resMgr.d_fonts ) )
{
}

inline
ResourceManager::~ResourceManager()
{
}

// OPERATORS
inline
ResourceManager& ResourceManager::operator=( const ResourceManager& resMgr )
{
    d_fonts = resMgr.d_fonts;
    return *this;
}

inline
ResourceManager& ResourceManager::operator=( ResourceManager&& resMgr )
{
    d_fonts = std::move( resMgr.d_fonts );
    return *this;
}

} // End nspc sgdg

} // End nspc StevensDev

#endif
