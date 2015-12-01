// image_data_factory.cpp
#include "image_data_factory.h"

namespace StevensDev
{

namespace sgda
{

// MEMBER FUNCTIONS
sf::Image* ImageDataFactory::get( const std::string& path )
{
    sf::Image* ptr = d_alloc.get( 1 );

    if ( !ptr->loadFromFile( path ) )
    {
        d_alloc.release( ptr, 1 );
        return nullptr;
    }

    return ptr;
}

} // End nspc sgda

} // End nspc StevensDev
