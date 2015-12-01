// resource_manager.cpp
#include <engine/data/json_parser.h>
#include <fstream>
#include "resource_manager.h"

namespace StevensDev
{

namespace sgda
{

// GLOBALS
ResourceManager ResourceManager::d_manager = ResourceManager();
ImageDataFactory ResourceManager::d_imageFactory = ImageDataFactory();
ConfigDataFactory ResourceManager::d_configFactory = ConfigDataFactory();

// MEMBER FUNCTIONS
void ResourceManager::postTick()
{
    if ( d_frame == ( unsigned int )-1 )
    {
        d_frame = 0;
    }

    if ( !( ++d_frame % 8 ) )
    {
        d_images.clean();
    }
    else if ( ( d_frame + 4 ) % 8 )
    {
        d_configs.clean();
    }
}

} // End nspc sgda

} // End nspc StevensDev
