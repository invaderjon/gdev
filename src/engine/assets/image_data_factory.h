// image_data_factory.h
#ifndef INCLUDED_IMAGE_DATA_FACTORY
#define INCLUDED_IMAGE_DATA_FACTORY

#include <engine/assets/idata_factory.h>
#include <engine/containers/map.h>
#include <SFML/Graphics/Image.hpp>

namespace StevensDev
{

namespace sgda
{

class ImageDataFactory : public IDataFactory<sf::Image>
{
  private:
    // MEMBERS
    sgdm::AllocatorGuard<sf::Image> d_alloc;
      // Allocates images.

  public:
    // CONSTRUCTORS
    ImageDataFactory();
      // Constructs a new image data factory.

    ImageDataFactory( sgdm::IAllocator<sf::Image>* allocator );
      // Constructs a new image data factory using the given allocator.

    ImageDataFactory( const ImageDataFactory& factory );
      // Constructs a copy of the factory.

    ~ImageDataFactory();
      // Destructs the factory.

    // OPERATORS
    ImageDataFactory& operator=( const ImageDataFactory& factory );
      // Makes this a copy of the given factory.

    // MEMBER FUNCTIONS
    virtual sf::Image* get( const std::string& path );
      // Loads an image at the given path.

    virtual void release( sf::Image* data );
      // Releases an image.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const ImageDataFactory& factory )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
ImageDataFactory::ImageDataFactory() : d_alloc()
{
}

inline
ImageDataFactory::ImageDataFactory( sgdm::IAllocator<sf::Image>* allocator )
    : d_alloc( allocator )
{
}

inline
ImageDataFactory::ImageDataFactory( const ImageDataFactory& factory )
    : d_alloc( factory.d_alloc )
{
}

inline
ImageDataFactory::~ImageDataFactory()
{

}

// OPERATORS
inline
ImageDataFactory&
ImageDataFactory::operator=( const ImageDataFactory& factory )
{
    d_alloc = factory.d_alloc;

    return *this;
}

// MEMBER FUNCTIONS
inline
void ImageDataFactory::release( sf::Image* data )
{
    d_alloc.release( data, 1 );
}

} // End nspc sgda

} // End nspc StevensDev

#endif
