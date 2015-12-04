// world_view.cpp
#include "world_view.h"
#include <gel/math/gelmath.h>

namespace StevensDev
{

namespace sgds
{

// GLOBALS
WorldView WorldView::d_instance = WorldView();

// MUTATOR FUNCTIONS
void WorldView::moveView( float dx, float dy, bool clip )
{
    using namespace gel::math;

    sgds::RectangleBounds view( viewLeft() + dx, viewTop() + dy,
                                viewWidth(), viewHeight() );

    if ( clip )
    {
        // clip to inside the world
        view.setPosition( Math::min( Math::max( left(), view.left() ),
                              right() - view.width() ),
                          Math::min( Math::max( top(), view.top() ),
                              bottom() - view.height() ) );
    }

    d_workingViewSpace = view;
}

void WorldView::setViewScale( float scale, bool forceFit )
{
    assert( scale > 0.0f );

    // do not update if the scale hasn't changed
    if ( scale == d_viewScale )
    {
        return;
    }

    using namespace gel::math;

    float width;
    float height;

    // calculate new working view space size
    width = d_viewSpace.width() * scale;
    height = d_viewSpace.height() * scale;

    // check if should clamp scaling
    if ( forceFit && ( width > this->width() || height > this->height() ) )
    {
        scale = Math::min( this->width() / d_viewSpace.width(),
                           this->height() / d_viewSpace.height() );

        width = d_viewSpace.width() * scale;
        height = d_viewSpace.height() * scale;
    }

    // recenter
    float dx;
    float dy;

    // move view space to appropriate location
    dx = ( d_workingViewSpace.width() - width ) / 2.0f;
    dy = ( d_workingViewSpace.height() - height ) / 2.0f;

    // apply changes
    d_workingViewSpace.setWidth( width );
    d_workingViewSpace.setHeight( height );
    moveView( dx, dy, forceFit );
}

// GLOBAL FUNCTIONS
void WorldView::initialize( const sgds::RectangleBounds& screenSize,
                            const sgds::RectangleBounds& idealSize,
                            const sgds::RectangleBounds& worldSpace,
                            float dpPerWU )
{
    using namespace gel::math;

    float pxPerDP = Math::min( screenSize.width() / idealSize.width(),
                               screenSize.height() / idealSize.height() );

    sgds::RectangleBounds viewSpace =
        calculateViewSpace( worldSpace,
                            screenSize.width() / screenSize.height() );

    d_instance = WorldView( worldSpace, viewSpace, dpPerWU, pxPerDP );
}

// GLOBAL HELPER FUNCTIONS
sgds::RectangleBounds WorldView::calculateViewSpace(
    const sgds::RectangleBounds& worldSpace, float aspectRatio )
{
    float wsAR = worldSpace.width() / worldSpace.height();

    if ( aspectRatio == wsAR )
    {
        return worldSpace;
    }
    else if ( aspectRatio > wsAR )
    {
        return sgds::RectangleBounds(
            worldSpace.left(), worldSpace.top(),
            worldSpace.width(), worldSpace.width() / aspectRatio );
    }
    else
    {
        return sgds::RectangleBounds(
            worldSpace.left(), worldSpace.top(),
            worldSpace.height() * aspectRatio,
            worldSpace.height() );
    }
}

} // End nspc sgds

} // End nspc StevensDev