// rectangle_bounds.h
#ifndef INCLUDED_RECTANGLE_BOUNDS
#define INCLUDED_RECTANGLE_BOUNDS

#include <engine/data/json_printer.h>
#include <iostream>

namespace StevensDev
{

namespace sgds
{

class RectangleBounds
{
  private:
    float d_x;
      // The bounds x position.

    float d_y;
      // The bounds y position.

    float d_width;
      // The bounds width.

    float d_height;
      // The bounds height.

  public:
    // CONSTRUCTORS
    RectangleBounds();
      // Constructs new bounds that are defined by a 1.0 x 1.0 rectangle at
      // 0, 0.

    RectangleBounds( float x, float y, float width, float height );
      // Constructs new bounds defined by the rectangle defined by the given
      // dimensions and position.

    RectangleBounds( const RectangleBounds& other );
      // Creates a copy of the given bounds.

    ~RectangleBounds();
      // Destructs the bounds.

    // OPERATORS
    RectangleBounds& operator=( const RectangleBounds& bounds );
      // Makes this a copy of the specified bounds.

    // ACCESSOR FUNCTIONS
    float x() const;
      // Gets the x coordinate of the bounds.

    float y() const;
      // Gets the y coordinate of the bounds.

    float width() const;
      // Gets the width of the bounds.

    float height() const;
      // Gets the height of the bounds.

    float left() const;
      // Gets the position of the left side of the bounds.

    float top() const;
      // Gets the position of the top side of the bounds.

    float right() const;
      // Gets the position of the right side of the bounds.

    float bottom() const;
      // Gets the position of the top side of the bounds.

    // MUTATOR FUNCTIONS
    void setX( float x );
      // Sets the x position of the bounds.

    void setY( float y );
      // Sets the y position of the bounds.

    void setWidth( float width );
      // Sets the width of the bounds.
      //
      // Width must be greater than zero.

    void setHeight( float height );
      // Sets the height of the bounds.
      //
      // Height must be greater than zero.

    void setDimensions( float width, float height );
      // Sets the size of the bounds.
      //
      // Width must be greater than zero.
      // Height must be greater than zero.

    void setPosition( float x, float y );
      // Sets the position of the bounds.

    // MEMBER FUNCTIONS
    bool doesCollide( const RectangleBounds& candidate ) const;
      // Checks if the given bounds collides with  the object.,
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream, const RectangleBounds& bounds )
{
    sgdd::JsonPrinter p( stream );
    p.open()
        .print( "x", bounds.x() )
        .print( "y", bounds.y() )
        .print( "width", bounds.width() )
        .print( "height", bounds.height() )
        .close();

    return stream;
}

// CONSTRUCTORS
inline
RectangleBounds::RectangleBounds() : d_x( 0.0f ), d_y( 0.0f ),
                                     d_width( 1.0f ), d_height( 1.0f )
{
}

inline
RectangleBounds::RectangleBounds( float x, float y,
                                  float width, float height )
    : d_x( x ), d_y( y ), d_width( width ), d_height( height )
{
}

inline
RectangleBounds::RectangleBounds( const RectangleBounds& other )
    : d_x( other.d_x ), d_y( other.d_y ),
      d_width( other.d_width ), d_height( other.d_height )
{
}

inline
RectangleBounds::~RectangleBounds()
{
}

// OPERATORS
inline
RectangleBounds& RectangleBounds::operator=( const RectangleBounds& bounds )
{
    d_x = bounds.x();
    d_y = bounds.y();
    d_width = bounds.width();
    d_height = bounds.height();

    return *this;
}

// ACCESSOR FUNCTIONS
inline
float RectangleBounds::x() const
{
    return d_x;
}

inline
float RectangleBounds::y() const
{
    return d_y;
}

inline
float RectangleBounds::left() const
{
    return d_x;
}

inline
float RectangleBounds::top() const
{
    return d_y;
}

inline
float RectangleBounds::right() const
{
    return d_x + d_width;
}

inline
float RectangleBounds::bottom() const
{
    return d_y + d_height;
}

inline
float RectangleBounds::width() const
{
    return d_width;
}

inline
float RectangleBounds::height() const
{
    return d_height;
}

// MUTATOR FUNCTIONS
inline
void RectangleBounds::setX( float x )
{
    d_x = x;
}

inline
void RectangleBounds::setY( float y )
{
    d_y = y;
}

inline
void RectangleBounds::setWidth( float width )
{
    d_width = width;
}

inline
void RectangleBounds::setHeight( float height )
{
    d_height = height;
}

inline
void RectangleBounds::setDimensions( float width, float height )
{
    d_width = width;
    d_height = height;
}

inline
void RectangleBounds::setPosition( float x, float y )
{
    d_x = x;
    d_y = y;
}

// MEMBER FUNCTIONS
inline
bool RectangleBounds::doesCollide( const RectangleBounds& c ) const
{
    return left() <= c.right() &&
           right() >= c.left() &&
           top() <= c.bottom() &&
           bottom() >= c.top();
}

} // End nspc sgds

} // End nspc StevensDev

#endif
