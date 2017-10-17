// collision_controller.cpp
#include <engine/scene/scene.h>
#include <engine/scene/nxn_scene_graph.h>
#include "collision_controller.h"

namespace StevensDev
{

namespace mgc
{

// MEMBER FUNCTIONS
void CollisionController::tick( float dtS )
{
    sgdc::DynamicArray<sgds::ICollider*> found =
        sgds::Scene::inst().sceneGraph().find( d_actor );

    for ( int i = 0; i < found.size(); ++i )
    {

    }
}

} // End nspc mgc

} // End nspc StevensDev