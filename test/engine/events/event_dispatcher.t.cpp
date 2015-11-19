// event_dispatcher.t.cpp
#include "engine/events/event_dispatcher.h"
#include "engine/events/test_event.h"
#include "engine/events/test_listener.h"
#include <gtest/gtest.h>

TEST( EventDispatcherTest, Construction )
{
    using namespace StevensDev::sgde;

    EventDispatcher d;

    EventDispatcher d2( d );

    d = d2;
}

TEST( EventDispatcherTest, Dispatching )
{
    using namespace StevensDev::sgde;
    using namespace StevensDev::sgdt;

    EventDispatcher d;

    TestListener listener;
    TestEvent event;

    std::function<void( const IEvent* )> callback = std::bind(
        &TestListener::receive, &listener, std::placeholders::_1 );

    // ensure added callbacks are called
    d.add( TestEvent::DEFAULT_TYPE, &callback );
    d.postTick();

    d.dispatch( &event );

    EXPECT_TRUE( listener.wasCalled() );

    EXPECT_STREQ( TestEvent::DEFAULT_TYPE.c_str(),
                  listener.receivedType().c_str() );

    // ensure removed callbacks are no longer called
    d.remove( TestEvent::DEFAULT_TYPE, & callback );
    d.postTick();

    listener.reset();

    d.dispatch( &event );

    EXPECT_FALSE( listener.wasCalled() );
}

TEST( EventDispatcher, Print )
{
    using namespace StevensDev::sgde;

    EventDispatcher d;

    std::ostringstream oss;

    oss << d;

    EXPECT_STREQ( "{  }", oss.str().c_str() );
}