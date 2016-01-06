// renderer.cpp
#include "renderer.h"
#include <SFML/Window/Event.hpp>

void StevensDev::sgdr::Renderer::draw()
{

    // draw sprites
    d_window.clear( sf::Color::Black );

    unsigned int i;
    for ( i = 0; i < d_sprites.size(); ++i )
    {
        d_window.draw( d_sprites[i]->sprite() );
    }
    d_window.display();

    // check for window events
    sf::Event e;
    if ( d_window.pollEvent( e ) )
    {
        switch ( e.type )
        {
            case sf::Event::Closed:
                d_window.close();
                break;

            default:
                break;
        }
    }
}

void StevensDev::sgdr::Renderer::setupWindow( int width, int height )
{
    assert( width > 0 );
    assert( height > 0 );
    d_window.create( sf::VideoMode( static_cast<unsigned int>( width ),
                                    static_cast<unsigned int>( height ) ),
                     "Game" );
    d_window.setFramerateLimit( 120 );
}

bool StevensDev::sgdr::Renderer::loadTexture( const std::string& name,
                                              const std::string& path )
{
    // load the image
    sf::Image image;
    if ( !image.loadFromFile( path ) )
    {
        return false;
    }

    // determine size
    sf::IntRect rect;
    rect.left = 0;
    rect.top = 0;
    rect.width = image.getSize().x;
    rect.height = image.getSize().y;
    sf::Texture texture;

    // load texture
    if ( !texture.loadFromImage( image, rect ) )
    {
        return false;
    }

    d_textures[name] = texture;
    return true;
}
