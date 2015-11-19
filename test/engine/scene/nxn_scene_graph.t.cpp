// nxn_scene_graph.t.cpp
#include <engine/scene/nxn_scene_graph.h>
#include <engine/scene/test_collider.h>
#include <gtest/gtest.h>

TEST( NxNSceneGraphTest, Construction )
{
    using namespace StevensDev::sgds;

    NxNSceneGraph graph;

    NxNSceneGraph sized( 1, 10 );

    NxNSceneGraph copy( graph );

    graph = copy;
}

TEST( NxNSceneGraphTest, ColliderManagement )
{
    using namespace StevensDev::sgds;
    using namespace StevensDev::sgdt;

    NxNSceneGraph graph;

    TestCollider collider;

    graph.addCollider( &collider );
    graph.removeCollider( &collider );
}

TEST( NxNSceneGraphTest, CollisionDetection )
{
    using namespace StevensDev::sgds;
    using namespace StevensDev::sgdt;
    using namespace StevensDev::sgdc;

    NxNSceneGraph graph( 10.0f, 100 );

    RectangleBounds ra( 0.0f, 0.0f, 1.0f, 1.0f );
    RectangleBounds rb( 0.5f, 0.5f, 2.0f, 2.0f );
    RectangleBounds rc( 1.0f, 0.0f, 1.0f, 1.0f );
    RectangleBounds rd( 2.0f, 2.0f, 1.0f, 1.0f );

    TestCollider a( ra );
    TestCollider b( rb );
    TestCollider c( rc );
    TestCollider d( rd );

    DynamicArray<ICollider*> results;

    graph.addCollider( &a );
    graph.addCollider( &b );
    graph.addCollider( &c );
    graph.addCollider( &d );

    results = graph.find( 0.0f, 0.0f, 1.0f, 1.0f );
    EXPECT_EQ( 3, results.size() );

    results = graph.find( ra );
    EXPECT_EQ( 3, results.size() );

    results = graph.find( &a );
    EXPECT_EQ( 3, results.size() );

    results = graph.find( 0.0f, 0.0f, 1.0f, 1.0f, 1 );
    EXPECT_EQ( 0, results.size() );

    results = graph.find( ra, 1 );
    EXPECT_EQ( 0, results.size() );
    
    graph.removeCollider( &a );
}