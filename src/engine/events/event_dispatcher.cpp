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

    const sgdc::DynamicArray<RegisteredListener>& registered =
        d_registered[event->type()];

    unsigned int i;
    for ( i = 0; i < registered.size(); ++i )
    {
        ( registered[i].listener )( event );
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
    PendingAddition newItem;
    PendingRemoval oldItem;

    while ( d_pendingAddition.size() > 0 )
    {
        newItem = d_pendingAddition.pop();
        d_registered[newItem.first].push( newItem.second );
    }

    while ( d_pendingRemoval.size() > 0 )
    {
        oldItem = d_pendingRemoval.pop();
        int pos = findListener( oldItem.first, oldItem.second );

        if ( pos < 0 )
        {
            continue;
        }

        d_registered[oldItem.first].removeAt( ( unsigned int )pos );
    }
}

// HELPER FUNCTIONS
int EventDispatcher::findListener( const std::string& item,
                                            EventHandle handle )
{
    return findListener( d_registered[item], handle,
                         0, d_registered[item].size() );
}

int EventDispatcher::findListener(
    const sgdc::DynamicArray<RegisteredListener>& listeners,
    EventHandle handle, unsigned int start, unsigned int end )
{
    if ( start >= end )
    {
        return -1;
    }

    unsigned int middle = ( end + start ) / 2;

    if ( handle == listeners[middle].handle )
    {
        return middle;
    }
    else if ( handle < listeners[middle].handle )
    {
        return findListener( listeners, handle, start, middle );
    }
    else
    {
        return findListener( listeners, handle, middle + 1, end );
    }
}

} // End nspc sgde

} // End nspc StevensDev
