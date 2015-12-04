// resource_database.cpp
#include "resource_database.h"
#include <fstream>

namespace StevensDev
{

namespace sgda
{


void ResourceDatabase::addPackageResources( const std::string& dbPath,
                                            ResourceID offset )
{
    std::ifstream ifs( "res/res.db" );
    ResourceEntry entry;

    while ( !ifs.eof() )
    {
        ifs >> entry.id;
        ifs >> entry.path;
        entry.fileSize = 0;
        entry.lastModified = 0;
        entry.dateCreated = 0;
        entry.version = 1;

        d_entries[offset + entry.id] = entry;
    }
}

} // End nspc sgda

} // End nspc StevensDev