// renderable_sprite.h
#ifndef INCLUDED_RENDERABLE_SPRITE
#define INCLUDED_RENDERABLE_SPRITE
#include "engine/data/json_printer.h"
#include "engine/scene/world_view.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <engine/containers/dynamic_array.h>
#include <engine/containers/map.h>

namespace StevensDev
{

namespace sgdr
{

class RenderableSprite
{
  private:
    // CONSTANTS
    static const std::string DEFAULT_STATE;

    // MEMBERS
    sgdc::Map<sgdc::DynamicArray<sf::Sprite>> d_sprites;
      // The sprite set that is rendered.

    std::string d_state;
      // The current state.

    unsigned int d_frame;
      // The current frame.

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

    virtual ~RenderableSprite();
      // Deconstructs a renderable sprite.

    // OPERATORS
    RenderableSprite& operator=( const RenderableSprite& other );
      // Makes this a copy of another renderable sprite.

    // ACCESSOR FUNCTIONS
    virtual const sf::Sprite& sprite() const;
      // Gets the sprite drawn by this.

    // MUTATOR FUNCTIONS
    void setState( const std::string& state );
      // Changes the renderable sprite's state.

    // MEMBER FUNCTIONS
    void nextFrame();
      // Moves on to the next frame.

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
    : d_sprites(), d_state( DEFAULT_STATE ), d_frame( 0 )
{
    d_sprites[DEFAULT_STATE].push( sf::Sprite( texture ) );
}

inline
RenderableSprite::RenderableSprite( const sf::Sprite& sprite )
    : d_sprites(), d_state( DEFAULT_STATE ), d_frame( 0 )
{
    d_sprites[DEFAULT_STATE].push( sprite );
}

inline
RenderableSprite::RenderableSprite( const RenderableSprite& other )
    : d_sprites( other.d_sprites ), d_state( other.d_state ),
      d_frame( other.d_frame )
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
    d_sprites = other.d_sprites;
    d_state = other.d_state;
    d_frame = other.d_frame;

    return *this;
}

// ACCESSOR FUNCTIONS
inline
const sf::Sprite& RenderableSprite::sprite() const
{
    return d_sprites[d_state][d_frame];
}

// MUTATOR FUNCTIONS
inline
void RenderableSprite::setState( const std::string& state )
{
    assert( d_sprites.has( state ) );

    sf::Vector2f pos = d_sprites[d_state][d_frame].getPosition();
    d_sprites[state][d_frame].setPosition( pos );
    d_state = state;
}

// MEMBER FUNCTIONS
inline
void RenderableSprite::nextFrame()
{
    unsigned int next = ( d_frame + 1 ) % d_sprites[d_state].size();
    if ( next == d_frame )
    {
        return;
    }

    sf::Vector2f pos = d_sprites[d_state][d_frame].getPosition();
    d_sprites[d_state][next].setPosition( pos );
    d_frame = next;
}

inline
void RenderableSprite::setPosition( float x, float y )
{
    const sgds::WorldView& w = sgds::WorldView::inst();
    d_sprites[d_state][d_frame].setPosition( w.dpToPX( x ), w.dpToPX( y ) );
}

inline
float RenderableSprite::getPositionX() const
{
    const sgds::WorldView& w = sgds::WorldView::inst();
    return w.pxToDP( d_sprites[d_state][d_frame].getPosition().x );
}

inline
float RenderableSprite::getPositionY() const
{
    const sgds::WorldView& w = sgds::WorldView::inst();
    return w.pxToDP( d_sprites[d_state][d_frame].getPosition().y );
}

inline
void RenderableSprite::move( float x, float y )
{
    const sgds::WorldView& w = sgds::WorldView::inst();
    d_sprites[d_state][d_frame].move( w.dpToPX( x ), w.dpToPX( y ) );
}

inline
float RenderableSprite::getWidth() const
{
    const sgds::WorldView& w = sgds::WorldView::inst();
    return w.pxToDP( d_sprites[d_state][d_frame].getGlobalBounds().width );
}

inline
float RenderableSprite::getHeight() const
{
    const sgds::WorldView& w = sgds::WorldView::inst();
    return w.pxToDP( d_sprites[d_state][d_frame].getGlobalBounds().height );
}

} // End nspc sgdr

} // End nspc StevensDev

#endif
