// renderer.h
//
// View Space scaling and motion is currently unsupported.
// View Spaces of different sizes than the window are currently unsupported.
// DP resolution scaling is currently unsupported.
// See WorldView for more information.
//
// todo: add support for aforementioned features
#ifndef INCLUDED_RENDERER
#define INCLUDED_RENDERER
#include "../containers/dynamic_array.h"
#include "../containers/map.h"
#include "renderable_sprite.h"
#include <SFML/Graphics/RenderWindow.hpp>

namespace StevensDev
{

namespace sgdr
{

class Renderer
{
  private:
    sgdc::DynamicArray<const RenderableSprite*> d_sprites;
      // The set of sprites to be rendered.

    sgdc::Map<sf::Texture> d_textures;
      // The set of available textures.

    sf::RenderWindow d_window;
      // The window that is being rendered to.

  public:
    // CONSTRUCTORS
    Renderer();
      // Creates a new renderer.

    Renderer( const Renderer& renderer );
      // Creates a copy of a renderer.

    Renderer( Renderer&& renderer );
      // Moves a renderer to a new instance.

    ~Renderer();
      // Destructs a renderer.

    // OPERATORS
    Renderer& operator=( const Renderer& renderer );
      // Makes this a copy of another renderer.

    Renderer& operator=( Renderer&& renderer );
      // Moves the other renderer to this instance.

    // ACCESSOR FUNCTIONS
    unsigned int width() const;
      // Gets the window width.

    unsigned int height() const;
      // Gets the window height.

    // MEMBER FUNCTIONS
    void draw();
      // Draw all of the sprites.

    void addSprite( const RenderableSprite* sprite );
      // Adds a sprite from the rendered list.

    void removeSprite( const RenderableSprite* sprite );
      // Removes a sprite from the rendered list.

    void setupWindow( int width, int height );
      // Sets up the renderer for the given window size.

    bool loadTexture( const std::string& name, const std::string& path );
      // Loads a texture and returns if it was found.

    const sf::Texture& getTexture( const std::string& name );
      // Gets the texture with the specified name.

    bool isActive() const;
      // Whether or not there is an associated window.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const Renderer& renderer )
{
    sgdd::JsonPrinter printer( stream );
    printer.open().print( "isActive", renderer.isActive() ).close();
    return stream;
}

// CONSTRUCTORS
inline
Renderer::Renderer() : d_sprites(), d_window()
{
}

inline
Renderer::Renderer( const Renderer& renderer )
    : d_sprites( renderer.d_sprites )
{
    if ( renderer.isActive() )
    {
        setupWindow( renderer.d_window.getSize().x,
                     renderer.d_window.getSize().y );
    }
}

inline
Renderer::Renderer( Renderer&& renderer )
    : d_sprites( std::move( renderer.d_sprites ) )
{
    if ( renderer.isActive() )
    {
        renderer.setupWindow( renderer.d_window.getSize().x,
                              renderer.d_window.getSize().y );
    }
}

inline
Renderer::~Renderer()
{
    d_window.close();
}

// OPERATORS
inline
Renderer& Renderer::operator=( const Renderer& renderer )
{
    if ( d_window.isOpen() )
    {
        d_window.close();
    }

    d_sprites = renderer.d_sprites;

    if ( renderer.isActive() )
    {
        setupWindow( renderer.d_window.getSize().x,
                     renderer.d_window.getSize().y );
    }

    return *this;
}

inline
Renderer& Renderer::operator=( Renderer&& renderer )
{
    if ( d_window.isOpen() )
    {
        d_window.close();
    }

    d_sprites = std::move( renderer.d_sprites );

    if ( renderer.isActive() )
    {
        setupWindow( renderer.d_window.getSize().x,
                     renderer.d_window.getSize().y );
    }

    return *this;
}

// ACCESSOR FUNCTIONS
inline
unsigned int Renderer::width() const
{
    return d_window.getSize().x;
}

inline
unsigned int Renderer::height() const
{
    return d_window.getSize().y;
}

// MEMBER FUNCTIONS
inline
void Renderer::addSprite( const RenderableSprite* sprite )
{
    d_sprites.push( sprite );
}

inline
void StevensDev::sgdr::Renderer::removeSprite(
    const StevensDev::sgdr::RenderableSprite* sprite )
{
    d_sprites.remove( sprite );
}

inline
const sf::Texture& StevensDev::sgdr::Renderer::getTexture(
    const std::string& name )
{
    return d_textures[name];
}

inline
bool StevensDev::sgdr::Renderer::isActive() const
{
    return d_window.isOpen();
}

} // End nspc sgdr

} // End nspc StevensDev

#endif
