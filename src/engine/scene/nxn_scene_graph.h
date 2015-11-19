// nxn_scene_graph.h
#ifndef INCLUDED_NXN_SCENE_GRAPH
#define INCLUDED_NXN_SCENE_GRAPH

#include "../containers/dynamic_array.h"
#include "icollider.h"

namespace StevensDev
{

namespace sgds
{

// foreword:
// The coordinate system in the scene graph is:
//
// 0 _______
//  |        x+
//  |
//  |
// y+
//
// a rectangle defined as (x, y, width, height) is displayed as:
//
// 0 ____________x+
//  |
//  | (x,y) _
//  |      | |
//  |      |_|
//  |        (x + width, y + height)
//  |
// y+
//
//
// Restrictions of items in scene graph:
// - items must be partially on screen

class NxNSceneGraph
{
  private:
    struct Cell
    {
        RectangleBounds bounds;
          // The bounds of the cell.

        sgdc::DynamicArray<ICollider*> contents;
          // The list of items inside of the cell.
    };

    struct CellRegion
    {
        unsigned int left;
          // The left-most column in the region.

        unsigned int top;
          // The top-most row in the region.

        unsigned int right;
          // The right-most column in the region.

        unsigned int bottom;
          // The bottom-most row in the region.
    };

    sgdm::AllocatorGuard<Cell> d_cellAlloc;
      // Allocates the cells.

    Cell* d_cells;
      // The cells that partition the space.
      //
      // Row-Major order

    float d_size;
      // The width and height of the scene graph.

    unsigned int d_divs;
      // The number of divisions in the scene graph.

    void initializeCells();
      // Prepare the cells for usage.

    // HELPER FUNCTIONS
    unsigned int getIndex( unsigned int row, unsigned int col ) const;
      // Gets the index of the cell at the given row and column.

    CellRegion getCellRegion( const RectangleBounds& bounds ) const;
      // Get the region of cells that the given bounds collides with.

  public:
    // CONSTRUCTORS
    NxNSceneGraph();
      // Constructs a new blank scene graph of size 2.0 by 2.0 with 10
      // divisions such that the minimum position is -1.0 and the maximum
      // position is 1.0 and each division is 0.2 in width/height.

    NxNSceneGraph( float dimensions, unsigned int divisions );
      // Creates a new scene graph.
      //
      // Dimensions must be greater than zero
      // Divisions must be greater than zero.

    NxNSceneGraph( const NxNSceneGraph& graph );
      // Creates a copy of the given scene graph.

    ~NxNSceneGraph();
      // Destructs the scene graph.

    // OPERATORS
    NxNSceneGraph& operator=( const NxNSceneGraph& graph );
      // Makes this a copy of the specified scene graph.

    // MEMBER FUNCTIONS
    void addCollider( ICollider* collider );
      // Adds a new collider to the scene.
      //
      // This assumes that the collider is not in motion.

    void removeCollider( ICollider* collider );
      // Removes a collider from the scene.

    sgdc::DynamicArray<ICollider*> find( float x, float y, float width,
                                         float height );
      // Finds the colliders in the specified region.

    sgdc::DynamicArray<ICollider*> find( float x, float y, float width,
                                         float height, unsigned short flags );
      // Finds the colliders in the specified region with the given flags.

    sgdc::DynamicArray<ICollider*> find( const RectangleBounds& bounds );
      // Finds the colliders in the specified rectangular region.

    sgdc::DynamicArray<ICollider*> find( const RectangleBounds& bounds,
                                         unsigned short flags );
      // Finds the flags in the specified rectangular region with the given
      // flags.

    sgdc::DynamicArray<ICollider*> find( const ICollider* collider );
      // Gets the colliders that are colliding with the given object.
};

// FREE OPERATORS
inline
std::ostream& operator<<( std::ostream& stream, const NxNSceneGraph& graph )
{
    return stream << "{  }";
}

// CONSTRUCTORS
inline
NxNSceneGraph::NxNSceneGraph()
    : d_cellAlloc(), d_cells( nullptr ), d_size( 2.0f ), d_divs( 10 )
{
    initializeCells();
}

inline
NxNSceneGraph::NxNSceneGraph( float dimensions, unsigned int divisions )
    : d_cellAlloc(), d_cells( nullptr ), d_size( dimensions ),
      d_divs( divisions )
{
    initializeCells();
}

inline
NxNSceneGraph::NxNSceneGraph( const NxNSceneGraph& graph )
    : d_cellAlloc(), d_cells( nullptr ),
      d_size( graph.d_size ), d_divs( graph.d_divs )
{
    unsigned int i;

    initializeCells();
    for ( i = 0; i < d_divs * d_divs; ++i )
    {
        d_cells[i] = graph.d_cells[i];
    }
}

inline
NxNSceneGraph::~NxNSceneGraph()
{
    d_cellAlloc.release( d_cells, d_divs * d_divs );
}

// MEMBER FUNCTIONS
inline
sgdc::DynamicArray<ICollider*> NxNSceneGraph::find( float x, float y,
                                                    float width, float height )
{
    return find( x, y, width, height, 0 );
}

inline
sgdc::DynamicArray<ICollider*> NxNSceneGraph::find( float x, float y,
                                                    float width, float height,
                                                    unsigned short flags )
{
    return find( RectangleBounds( x, y, width, height ), flags );
}

inline
sgdc::DynamicArray<ICollider*> NxNSceneGraph::find(
    const RectangleBounds& bounds )
{
    return find( bounds, 0 );
}

inline
sgdc::DynamicArray<ICollider*> NxNSceneGraph::find(
    const ICollider* collider )
{
    return find( collider->bounds(), collider->flags() );
}

// HELPER FUNCTIONS
inline
unsigned int
NxNSceneGraph::getIndex( unsigned int row, unsigned int col ) const
{
    return row * d_divs + col;
}

} // End nspc sgds

} // End nspc StevensDev

#endif