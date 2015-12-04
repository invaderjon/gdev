// resource_database.cpp
#include "resource_database.h"
#include <fstream>

namespace StevensDev
{

namespace sgda
{


void ResourceDatabase::addPackageResources( const std::string& dbPath )
{
    std::ifstream ifs( dbPath );
    ResourceEntry entry;

    int lines;
    ifs >> lines;
    while ( lines-- )
    {
        ifs >> entry.id;
        ifs >> entry.path;
        entry.fileSize = 0;
        entry.lastModified = 0;
        entry.dateCreated = 0;
        entry.version = 1;

        d_entries[entry.id] = entry;
    }
}

} // End nspc sgda

} // End nspc StevensDev