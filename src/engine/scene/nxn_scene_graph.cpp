// nxn_scene_graph.cpp
#include "nxn_scene_graph.h"
#include "../containers/map.h"
#include <sstream>
#include <cmath>

namespace StevensDev
{

namespace sgds
{

namespace
{

inline
sgdc::Map<ICollider*> addColliderToSet( sgdc::Map<ICollider*>& set,
                                        ICollider* collider )
{
    std::ostringstream oss;
    oss << (void*)collider;
    set[oss.str()] = collider;
    return set;
}

} // End nspc anonymous

// OPERATORS
NxNSceneGraph& NxNSceneGraph::operator=( const NxNSceneGraph& graph )
{
    if ( d_cells != nullptr )
    {
        d_cellAlloc.release( d_cells, d_divs * d_divs );
    }

    d_size = graph.d_size;
    d_divs = graph.d_divs;

    initializeCells();

    unsigned int i;
    for ( i = 0; i < d_divs * d_divs; ++i )
    {
        d_cells[i] = graph.d_cells[i];
    }

    return *this;
}

// MEMBER FUNCTIONS
void NxNSceneGraph::addCollider( ICollider* collider )
{
    CellRegion region = getCellRegion( collider->bounds() );
    unsigned int i;
    unsigned int j;

    // add the item to all intersecting cells
    for ( i = region.top; i <= region.bottom; ++i )
    {
        for ( j = region.left; j <= region.right; ++j )
        {
            d_cells[getIndex( i, j )].contents.push( collider );
        }
    }
}

void NxNSceneGraph::removeCollider( ICollider* collider )
{
    unsigned int i;

    // check all cells and let them sort out if it is actually removed
    for ( i = 0; i < d_divs * d_divs; ++i )
    {
        d_cells[i].contents.remove( collider );
    }
}

sgdc::DynamicArray<ICollider*> NxNSceneGraph::find(
    const RectangleBounds& bounds, unsigned short flags )
{
    sgdc::Map<ICollider*> colliding;
    CellRegion region = getCellRegion( bounds );
    ICollider* collider;
    unsigned int i;
    unsigned int j;
    unsigned int k;

    // check all cells in the feasible region
    for ( i = region.top; i <= region.bottom; ++i )
    {
        for ( j = region.left; j <= region.right; ++j )
        {
            const Cell& cell = d_cells[getIndex( i, j )];

            // check each item in each cell
            for ( k = 0; k < cell.contents.size(); ++k )
            {
                collider = cell.contents[k];

                if ( collider->canCollide( flags ) &&
                     collider->doesCollide( bounds ) )
                {
                    // store in hashmap to prevent duplicates
                    addColliderToSet( colliding, collider );
                }
            }
        }
    }

    return colliding.values();
}

// HELPER FUNCTIONS
void NxNSceneGraph::initializeCells()
{
    unsigned int i;
    unsigned int j;
    float cellSize = d_size / d_divs;
    float left;
    float top;

    d_cells = d_cellAlloc.get( d_divs * d_divs );

    for ( i = 0, top = 0.0f;
          i < d_divs;
          ++i, top += cellSize )
    {
        for ( j = 0, left = 0.0f;
              j < d_divs;
              ++j, left += cellSize )
        {
            d_cells[getIndex( i, j )].bounds =
                RectangleBounds( left, top, cellSize, cellSize );

            d_cells[getIndex( i, j )].contents =
                sgdc::DynamicArray<ICollider*>();
        }
    }

    // force span full width and height (reduce floating point error)
    for ( i = 0; i < d_divs; ++i )
    {
        // last column and row
        RectangleBounds& c = d_cells[getIndex( i, d_divs - 1 )].bounds;
        RectangleBounds& r = d_cells[getIndex( d_divs - 1, i )].bounds;

        c.setWidth( d_size - c.x() );
        r.setHeight( d_size - c.y() );
    }
}

NxNSceneGraph::CellRegion
NxNSceneGraph::getCellRegion( const RectangleBounds& bounds ) const
{
    // clip bounds to graph region
    RectangleBounds clipped = RectangleBounds(
        std::max( 0.0f, bounds.x() ),
        std::max( 0.0f, bounds.y() ),
        std::max( 0.0f, ( bounds.right() > d_size ) ?
                          d_size - bounds.x() :
                          bounds.width() ),
        std::max( 0.0f, ( bounds.bottom() > d_size ) ?
                          d_size - bounds.y() :
                          bounds.height() ) );

    // convert bounds to indices
    float cellSize = d_size / d_divs;
    CellRegion r;
    r.left = static_cast<unsigned int>(
        std::floor( clipped.left() / cellSize ) );
    r.top = static_cast<unsigned int>(
        std::floor( clipped.top() / cellSize ) );
    r.right = static_cast<unsigned int>(
        std::ceil( clipped.right() / cellSize ) );
    r.bottom = static_cast<unsigned int>(
        std::ceil( clipped.bottom() / cellSize ) );

    return r;
}

} // End nspc sgds

} // End nspc StevensDev