// world.h
//
// This class is used to defined the coordinate system that is mapped to the
// screen. This is used to define things like viewing space, world size,
// hypothetical resolution, and screen to world mappings.
//
// This does not provide any methods for actually managing the state of the
// world for such functionality see WorldMap instead.
#ifndef INCLUDED_WORLD
#define INCLUDED_WORLD

#include <engine/data/json_printer.h>
#include <engine/scene/rectangle_bounds.h>

namespace StevensDev
{

namespace sgdw
{

// Definitions:
//
// World Unit (WU)
// A world unit (WU) is a equal to a predefined number of DP.
// This provides more convenient measuring useful for things like precise
// snapping to grid and the production of more meaningful numbers. The exact
// conversion of DP to WU is an arbitrary number defined the game and can
// be as large or as small as desired. Although if the value is either of the
// two extremes it may result in floating point errors.
//
// Dot Point (DP)
// This is equal to one point on a hypothetical ideal screen. This
// maps the actual window pixels to the hypothetical screen retaining the
// ideal resolution and size as much as possible. This directly converts
// to pixels and is used to maintain consistent appearance across screen
// resolutions. It is through this value that multi-resolution textures are
// supported so can resources scale to a large range of screen resolutions.
// The exact DP to PX conversion rate is defined per-resolution.
// DISCLAIMER:
// The idea for this unit is based on the Android Platform DP and DPI and I
// take no credit for the idea for this hypothetical unit.
//
// Pixel (PX)
// This is exactly equal to one screen pixel and is should only be
// used when managing DP to the actual graphics (sprites).
//
// World Space
// The world space is the coordinate system through which the world is
// defined. The world space may be several times larger than view space. This
// is the only space that may be manually re-sized at run time. Specifically
// the world space will usually be altered when loading levels. The x and y
// values of the world space serve as a global offset and have no meaning
// beyond that.
//
// View Space
// The visible region of the world space defined in WU. The vew space must
// never be larger than the World Space. They can however be equal. Unlike
// the other spaces the view space also has relevant x and y coordinates
// which define it's position in the world space. As such the view space can
// be moved manually at run time. THe view space can also be resized but must
// maintain it's aspect ratio. This allows for visual effects such as zooming.
//
// Ideal Screen Space
// The hypothetical ideal viewing space that the unit DP is defined by. This
// serves to define a resolution and ratio that the screen space should strive
// to map to.
//
// Working Screen Space
// The working screen space is the best match hypothetical viewing space that
// the real screen space actually maps to. This can be calculated by
// multiplying the dimensions of the real screen space by the DPI constant.
//
// Real Screen Space
// This is the actual screen space in pixels. Its dimensions are equivalent
// to the window's dimensions.
//
// NOTE:
// WU to PX conversions are not provided because they shouldn't be done.
// If you find yourself going directly from one to the other you're doing
// something wrong.

class World
{
  private:
    // GLOBALS
    static World d_instance;
      // The singleton instance.

    // MEMBERS
    sgds::RectangleBounds d_worldSpace;
      // The world space.

    sgds::RectangleBounds d_viewSpace;
      // The view space.
      // This is only used internally to track the original view size.

    sgds::RectangleBounds d_workingViewSpace;
      // The scaled view space.
      // This is the actuall view space that is used during rendering.

    float d_dpPerWU;
    // The dpi to WU conversion ratio.
    //
    // This must be greater than zero.

    float d_pxPerDP;
      // The pixel to DP conversion ratio.
      //
      // This must be greater than zero.

    float d_viewScale;
      // The view space scale.
      //
      // The view space scale to apply. The view scale is inversely
      // proportional to zoom.
      // This must be greater than zero.

    // CONSTRUCTORS
    World();
      // Constructs a new world with no spatial information.
      //
      // Without such information the use of said world is incredibly limited.

    World( const sgds::RectangleBounds& worldSpace,
           const sgds::RectangleBounds& viewSpace,
           float dpPerWU, float pxPerDP );
      // Constructs a new world with the given coordinate system
      // specifications.
      //
      // Undefined behavior when:
      // viewSpace is larger than worldSpace.
      // viewSpace is not inside of worldSpace.
      // dpPerWU is less than or equal to zero.
      // pxPerDP is less than or equal to zero.
      // worldSpace is of width or height equal to zero.

    World( const World& world );
      // Constructs a copy of the given world.

    // OPERATORS
    World& operator=( const World& world );
      // Makes this a copy of another world.

    // GLOBAL HELPER FUNCTION
    static sgds::RectangleBounds calculateViewSpace(
        const sgds::RectangleBounds& worldSpace, float aspectRatio );
      // Calculates the largest view space for the given world space with
      // the given aspect ratio.

  public:
    // CONSTRUCTORS
    ~World();

    // ACCESSOR FUNCTIONS
    float left() const;
      // Gets the left-most coordinate in the world (WU).

    float top() const;
      // Gets the top-most coordinate in the world (WU).

    float right() const;
      // Gets the right-most coordinate in the world (WU).

    float bottom() const;
      // Gets the bottom-most coordinate in the world (WU).

    float width() const;
      // Gets the width of the world (WU).

    float height() const;
      // Gets the height of the world (WU).

    float viewLeft() const;
      // Gets the view space's left-most coordinate in the world (WU).

    float viewTop() const;
      // Gets the view space's top-most coordinate in the world (WU).

    float viewRight() const;
      // Gets the view space's right-most coordinate in the world (WU).

    float viewBottom() const;
      // Gets the view space's bottom-most coordinate in the world (WU).

    float viewWidth() const;
      // Gets the view space's width (WU).

    float viewHeight() const;
      // Gets the view space's height (WU).

    float viewScale() const;
      // Gets the view space's scale factor.
      //
      // The view scale is inversely proportional to zoom.
      // The view scale provides a convenient way to apply an easily
      // revertible zoom.
      // The view space scales towards its center so it doesn't look like it
      // has moved.

    const sgds::RectangleBounds& viewSpace() const;
      // Gets the view space bounds (WU).

    const sgds::RectangleBounds& worldSpace() const;
      // Gets the world space bounds (WU).

    // MUTATOR FUNCTIONS
    void resize( const sgds::RectangleBounds& worldSpace );
      // Re-sizes the world to the new world space. The view space will be
      // redefined in a manner similar to the way it is defined in the
      // initialize function.
      //
      // This function resets view scale to 1.0.
      // This functions resets the view space's position to the top left
      // corner of the world space.

    void setViewPosition( float x, float y, bool clip = false );
      // Moves the view space to the specified position in the world space.
      //
      // Clipping can optionally be enabled which prevents the view space from
      // leaving the world space.

    void moveView( float dx, float dy, bool clip = false );
      // Offsets the view space location by the specified offset.
      //
      // Clipping can optionally be enabled which prevents the view space from
      // leaving the world space.

    void scaleView( float dS, bool forceFit = false );
      // Scales the view space by the given factor.
      //
      // If forceFit is enabled the view space will be moved if necessary to
      // fit the newly scaled view space in the world space. Force fitting
      // will also prevent the scale from becoming too large to be able to fit
      // the view space in the world space.
      //
      // The scale must be greater than zero.
      // The view scale is inversely proportional to zoom.
      // The view scale provides a convenient way to apply an easily
      // revertible zoom.
      // The view space scales towards its center so it doesn't look like it
      // has moved.

    void setViewScale( float scale, bool forceFit = false );
      // Sets the view scale to the given value.
      //
      // If forceFit is enabled the view space will be moved if necessary to
      // fit the newly scaled view space in the world space. Force fitting
      // will also prevent the scale from becoming too large to be able to fit
      // the view space in the world space.
      //
      // The scale must be greater than zero.
      // The view scale is inversely proportional to zoom.
      // The view scale provides a convenient way to apply an easily
      // revertible zoom.
      // The view space scales towards its center so it doesn't look like it
      // has moved.

    // MEMBER FUNCTIONS
    float wuToDP( float wu ) const;
      // Converts world units to DP.

    float dpToWU( float points ) const;
      // Converts dot points to world units.

    float dpToPX( float points ) const;
      // Converts dot points to pixels.

    float pxToDP( float pixels ) const;
      // Converts pixels to dot points.

    // GLOBAL FUNCTIONS
    static World& inst();
      // Gets the singleton instance of the world.

    static void initialize( const sgds::RectangleBounds& screenSize,
                            const sgds::RectangleBounds& idealSize,
                            const sgds::RectangleBounds& worldSpace,
                            float dpPerWU );
      // Initializes the world for the given screen size, target size, and
      // world specs.
      // This automatically calculates the ideal px-to-dp ratio. This should
      // only be called one when the game is started.
      // This also calculates the largest view space that possible for the
      // given world size.
      //
      // Requirements:
      // All sizes must be valid.
      // dpPerWU msut be greater than zero.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream,
                          const World& world )
{
    sgdd::JsonPrinter p( stream );
    p.open().print( "left", world.left() )
            .print( "top", world.top() )
            .print( "right", world.right() )
            .print( "bottom", world.bottom() )
            .close();
    return stream;
}

// CONSTRUCTORS
inline
World::World() : d_worldSpace( 0.0f, 0.0f, 1.0f, 1.0f ),
                 d_viewSpace( 0.0f, 0.0f, 1.0f, 1.0f ),
                 d_workingViewSpace( d_viewSpace ),
                 d_pxPerDP( 1.0f ), d_dpPerWU( 250.0f ),
                 d_viewScale( 1.0f )
{
}

inline
World::World( const sgds::RectangleBounds& worldSpace,
              const sgds::RectangleBounds& viewSpace,
              float dpPerWU, float pxPerDP )
    : d_worldSpace( worldSpace ), d_viewSpace( viewSpace ),
      d_workingViewSpace( d_viewSpace ),
      d_dpPerWU( dpPerWU ), d_pxPerDP( pxPerDP ),
      d_viewScale( 1.0f )
{
    assert( viewSpace.width() <= worldSpace.width() );
    assert( viewSpace.height() <= worldSpace.height() );
    assert( viewSpace.doesCollide( worldSpace ) );
    assert( dpPerWU > 0.0f );
    assert( pxPerDP > 0.0f );
}

inline
World::World( const World& world )
    : d_worldSpace( world.d_worldSpace ), d_viewSpace( world.d_viewSpace ),
      d_workingViewSpace( world.d_workingViewSpace ),
      d_dpPerWU( world.d_dpPerWU ), d_pxPerDP( world.d_pxPerDP ),
      d_viewScale( world.d_viewScale )
{
}

inline
World::~World()
{
}

// OPERATORS
inline
World& World::operator=( const World& world )
{
    d_worldSpace = world.d_worldSpace;
    d_viewSpace = world.d_viewSpace;
    d_workingViewSpace = world.d_workingViewSpace;
    d_dpPerWU = world.d_dpPerWU;
    d_pxPerDP = world.d_pxPerDP;
    d_viewScale = world.d_viewScale;

    return *this;
}

// ACCESSOR FUNCTIONS
inline
float World::left() const
{
    return d_worldSpace.left();
}

inline
float World::top() const
{
    return d_worldSpace.top();
}

inline
float World::right() const
{
    return d_worldSpace.right();
}

inline
float World::bottom() const
{
    return d_worldSpace.bottom();
}

inline
float World::width() const
{
    return d_worldSpace.width();
}

inline
float World::height() const
{
    return d_worldSpace.height();
}

inline
float World::viewLeft() const
{
    return d_workingViewSpace.left();
}

inline
float World::viewTop() const
{
    return d_workingViewSpace.top();
}

inline
float World::viewRight() const
{
    return d_workingViewSpace.right();
}

inline
float World::viewBottom() const
{
    return d_workingViewSpace.bottom();
}

inline
float World::viewWidth() const
{
    return d_workingViewSpace.width();
}

inline
float World::viewHeight() const
{
    return d_workingViewSpace.height();
}

inline
float World::viewScale() const
{
    return d_viewScale;
}

inline
const sgds::RectangleBounds& World::viewSpace() const
{
    return d_workingViewSpace;
}

inline
const sgds::RectangleBounds& World::worldSpace() const
{
    return d_worldSpace;
}

// MUTATOR FUNCTIONS
inline
void World::resize( const sgds::RectangleBounds& worldSpace )
{
    d_worldSpace = worldSpace;
    d_viewSpace = calculateViewSpace( d_worldSpace,
                                      d_viewSpace.width() /
                                          d_viewSpace.height() ) ;
    d_workingViewSpace = d_viewSpace;
    d_viewScale = 1.0f;
}

inline
void World::setViewPosition( float x, float y, bool clip )
{
    moveView( x - viewLeft(), y - viewTop(), clip );
}

inline
void World::scaleView( float dS, bool forceFit )
{
    setViewScale( d_viewScale * dS, forceFit );
}

// MEMBER FUNCTIONS
inline
float World::wuToDP( float wu ) const
{
    return wu * d_dpPerWU;
}

inline
float World::dpToWU( float points ) const
{
    return points / d_dpPerWU;
}

inline
float World::dpToPX( float points ) const
{
    return points * d_pxPerDP;
}

inline
float World::pxToDP( float pixels ) const
{
    return pixels / d_pxPerDP;
}

// GLOBAL FUNCTIONS
inline
World& World::inst()
{
    return d_instance;
}

} // End nspc sgdw

} // End nspc StevensDev

#endif
