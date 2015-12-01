// config_data_factory.h
#ifndef INCLUDED_CONFIG_DATA_FACTORY
#define INCLUDED_CONFIG_DATA_FACTORY

#include "engine/assets/idata_factory.h"
#include "engine/data/json_entity.h"
#include "engine/data/json_parser.h"

namespace StevensDev
{

namespace sgda
{

class ConfigDataFactory : public IDataFactory<sgdd::JsonEntity>
{
  private:
    // MEMBERS
    sgdm::AllocatorGuard<sgdd::JsonEntity> d_alloc;
      // Allocates json objects.

  public:
    // CONSTRUCTORS
    ConfigDataFactory();
      // Constructs a new config data factory.

    ConfigDataFactory( sgdm::IAllocator<sgdd::JsonEntity>* allocator );
      // Constructs a new config data factory using the given allocator.

    ConfigDataFactory( const ConfigDataFactory& factory );
      // Constructs a copy of the factory.

    ~ConfigDataFactory();
      // Destructs the factory.

    // OPERATORS
    ConfigDataFactory& operator=( const ConfigDataFactory& factory );
      // Makes this a copy of the given factory.

    // MEMBER FUNCTIONS
    virtual sgdd::JsonEntity* get( const std::string& path );
      // Loads and returns the configuration at the given path.

    virtual void release( sgdd::JsonEntity* data );
      // Unloads the configuration.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const ConfigDataFactory factory )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
ConfigDataFactory::ConfigDataFactory() : d_alloc()
{
}

inline
ConfigDataFactory::ConfigDataFactory(
    sgdm::IAllocator<sgdd::JsonEntity>* allocator ) : d_alloc( allocator )
{
}

inline
ConfigDataFactory::ConfigDataFactory( const ConfigDataFactory& factory )
    : d_alloc( factory.d_alloc )
{
}

inline
ConfigDataFactory::~ConfigDataFactory()
{
}

// OPERATORS
inline
ConfigDataFactory& ConfigDataFactory::operator=(
    const ConfigDataFactory& factory )
{
    d_alloc = factory.d_alloc;

    return *this;
}

// MEMBER FUNCTIONS
inline
sgdd::JsonEntity* ConfigDataFactory::get( const std::string& path )
{
    return sgdd::JsonParser::fromFile( path, &d_alloc );
}

inline
void ConfigDataFactory::release( sgdd::JsonEntity* data )
{
    d_alloc.release( data, 1 );
}

} // End nspc sgda

} // End nspc StevensDev

#endif
