// image_data_factory.cpp
#include "texture_data_factory.h"

namespace StevensDev
{

namespace sgda
{

// MEMBER FUNCTIONS
sf::Texture* TextureDataFactory::get( const std::string& path )
{
    sf::Texture* ptr = d_alloc.get( 1 );

    if ( !ptr->loadFromFile( path ) )
    {
        d_alloc.release( ptr, 1 );
        return nullptr;
    }

    return ptr;
}

} // End nspc sgda

} // End nspc StevensDev
