// ResourceManager.cpp
#include <stdexcept>
#include "resource_manager.h"
#include <sstream>

namespace StevensDev
{

namespace sgdg
{

// FAST LOAD FUNCTIONS
bool ResourceManager::fastLoadFont( std::string name )
{
    std::ostringstream oss;
    oss << "res/font/" << name << ".ttf";

    sf::Font font;
    if ( font.loadFromFile( oss.str()))
    {
        d_fonts[name] = font;
        return true;
    }

    return false;
}

// ACCESSOR FUNCTIONS
sf::Font& ResourceManager::getFont( std::string name )
{
    if ( !d_fonts.has( name ))
    {
        throw std::runtime_error( "Resource not found!" );
    }

    return d_fonts[name];
}

} // End nspc sgdg

} // End nspc StevensDev