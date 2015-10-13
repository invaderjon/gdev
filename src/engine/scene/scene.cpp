// scene.cpp
#include "scene.h"

namespace StevensDev
{

namespace sgds
{

Scene Scene::d_instance = Scene();

void Scene::tick()
{
    assert( d_renderer != nullptr );
    unsigned int i;

    // pre-tick cycle
    for ( i = 0; i < d_tickables.size(); ++i )
    {
        d_tickables[i]->preTick();
    }

    // tick cycle
    for ( i = 0; i < d_tickables.size(); ++i )
    {
        d_tickables[i]->tick( d_clock.getElapsedTime().asSeconds() );
    }

    // draw
    d_renderer->draw();

    // post-tick cycle
    for ( i = 0; i < d_tickables.size(); ++i )
    {
        d_tickables[i]->postTick();
    }

    d_clock.restart();
}

} // End nspc sgds

} // End nspc StevensDev