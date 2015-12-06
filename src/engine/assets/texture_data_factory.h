// texture_data_factory.h
#ifndef INCLUDED_TEXTURE_DATA_FACTORY
#define INCLUDED_TEXTURE_DATA_FACTORY

#include <engine/assets/idata_factory.h>
#include <engine/containers/map.h>
#include <SFML/Graphics/Texture.hpp>

namespace StevensDev
{

namespace sgda
{

class TextureDataFactory : public IDataFactory<sf::Texture>
{
  private:
    // MEMBERS
    sgdm::AllocatorGuard<sf::Texture> d_alloc;
      // Allocates images.

  public:
    // CONSTRUCTORS
    TextureDataFactory();
      // Constructs a new image data factory.

    TextureDataFactory( sgdm::IAllocator<sf::Texture>* allocator );
      // Constructs a new image data factory using the given allocator.

    TextureDataFactory( const TextureDataFactory& factory );
      // Constructs a copy of the factory.

    ~TextureDataFactory();
      // Destructs the factory.

    // OPERATORS
    TextureDataFactory& operator=( const TextureDataFactory& factory );
      // Makes this a copy of the given factory.

    // MEMBER FUNCTIONS
    virtual sf::Texture* get( const std::string& path );
      // Loads an image at the given path.

    virtual void release( sf::Texture* data );
      // Releases an image.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const TextureDataFactory& factory )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
TextureDataFactory::TextureDataFactory() : d_alloc()
{
}

inline
TextureDataFactory::TextureDataFactory( sgdm::IAllocator<sf::Texture>* allocator )
    : d_alloc( allocator )
{
}

inline
TextureDataFactory::TextureDataFactory( const TextureDataFactory& factory )
    : d_alloc( factory.d_alloc )
{
}

inline
TextureDataFactory::~TextureDataFactory()
{

}

// OPERATORS
inline
TextureDataFactory&
TextureDataFactory::operator=( const TextureDataFactory& factory )
{
    d_alloc = factory.d_alloc;

    return *this;
}

// MEMBER FUNCTIONS
inline
void TextureDataFactory::release( sf::Texture* data )
{
    d_alloc.release( data, 1 );
}

} // End nspc sgda

} // End nspc StevensDev

#endif
