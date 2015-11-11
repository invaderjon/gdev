// event_dispatcher.cpp
#include "event_dispatcher.h"


namespace StevensDev
{

namespace sgde
{

void EventDispatcher::dispatch( const IEvent* event )
{
    assert( event != nullptr );

    // ensure listeners exist
    if ( !d_registered.has( event->type() ) )
    {
        return;
    }

    const sgdc::DynamicArray<const Listener*>& registered =
        d_registered[event->type()];

    unsigned int i;
    for ( i = 0; i < registered.size(); ++i )
    {
        ( *registered[i] )( event );
    }
}

void EventDispatcher::preTick()
{
    // does nothing
}

void EventDispatcher::tick( float dtS )
{
    // does nothing
}

void EventDispatcher::postTick()
{
    std::pair<std::string, const Listener*> item;

    while ( d_pendingAddition.size() > 0 )
    {
        item = d_pendingAddition.pop();
        d_registered[item.first].push( item.second );
    }

    while ( d_pendingRemoval.size() > 0 )
    {
        item = d_pendingRemoval.pop();
        d_registered[item.first].remove( item.second );
    }
}

} // End nspc sgde

} // End nspc StevensDev
