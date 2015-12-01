// renderable_sprite.h
#ifndef INCLUDED_RENDERABLE_SPRITE
#define INCLUDED_RENDERABLE_SPRITE
#include "../data/json_printer.h"
#include <engine/scene/world.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace StevensDev
{

namespace sgdr
{

class RenderableSprite
{
  private:
    sf::Sprite d_sprite;
      // The sprite that is rendered.

  public:
    // CONSTRUCTORS
    RenderableSprite();
      // Constructs an invalid sprite.

    RenderableSprite( const sf::Texture& texture );
      // Constructs a new sprite for a given texture.

    RenderableSprite( const sf::Sprite& sprite );
      // Constructs a new renderable sprite for a given underlying sprite.

    RenderableSprite( const RenderableSprite& other );
      // Constructs a copy of another renderable sprite.

    ~RenderableSprite();
      // Deconstructs a renderable sprite.

    // OPERATORS
    RenderableSprite& operator=( const RenderableSprite& other );
      // Makes this a copy of another renderable sprite.

    // ACCESSOR FUNCTIONS
    const sf::Sprite& sprite() const;
      // Gets the sprite drawn by this.

    // MEMBER FUNCTIONS
    void setPosition( float x, float y );
      // Sets the absolute position (DP).

    float getPositionX() const;
      // Gets the x coordinate of the position (DP).

    float getPositionY() const;
      // Gets the y coordinate of the position (DP).

    float getWidth() const;
      // Gets the width (DP).

    float getHeight() const;
      // Gets the height (DP).

    void move( float x, float y );
      // Offsets the position by the given amounts (DP).
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const RenderableSprite& sprite )
{
    sgdd::JsonPrinter printer( stream );
    printer.open()
           .print( "x", sprite.getPositionX() )
           .print( "y", sprite.getPositionY() )
           .close();
    return stream;
}

// CONSTRUCTORS
inline
RenderableSprite::RenderableSprite()
{
}

inline
RenderableSprite::RenderableSprite( const sf::Texture& texture )
    : d_sprite( sf::Sprite( texture ) )
{
}

inline
RenderableSprite::RenderableSprite( const sf::Sprite& sprite )
    : d_sprite( sprite )
{
}

inline
RenderableSprite::RenderableSprite( const RenderableSprite& other )
    : d_sprite( other.d_sprite )
{
}

inline
RenderableSprite::~RenderableSprite()
{
}

// OPERATORS
inline
RenderableSprite& RenderableSprite::operator=( const RenderableSprite& other )
{
    d_sprite = other.d_sprite;
}

// ACCESSOR FUNCTIONS
inline
const sf::Sprite& RenderableSprite::sprite() const
{
    return d_sprite;
}

// MEMBER FUNCTIONS
inline
void RenderableSprite::setPosition( float x, float y )
{
    const sgds::World& w = sgds::World::inst();
    d_sprite.setPosition( w.dpToPX( x ), w.dpToPX( y ) );
}

inline
float RenderableSprite::getPositionX() const
{
    const sgds::World& w = sgds::World::inst();
    return w.pxToDP( d_sprite.getPosition().x );
}

inline
float RenderableSprite::getPositionY() const
{
    const sgds::World& w = sgds::World::inst();
    return w.pxToDP( d_sprite.getPosition().y );
}

inline
void RenderableSprite::move( float x, float y )
{
    const sgds::World& w = sgds::World::inst();
    d_sprite.move( w.dpToPX( x ), w.dpToPX( y ) );
}

inline
float RenderableSprite::getWidth() const
{
    const sgds::World& w = sgds::World::inst();
    return w.pxToDP( d_sprite.getGlobalBounds().width );
}

inline
float RenderableSprite::getHeight() const
{
    const sgds::World& w = sgds::World::inst();
    return w.pxToDP( d_sprite.getGlobalBounds().height );
}

} // End nspc sgdr

} // End nspc StevensDev

#endif
