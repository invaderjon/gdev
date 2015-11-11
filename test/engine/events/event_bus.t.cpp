// event_bus.t.cpp
#include "engine/events/event_bus.h"
#include "engine/events/test_event.h"
#include <gtest/gtest.h>

TEST( EventBusTest, Usage )
{
    using namespace StevensDev::sgde;
    using namespace StevensDev::sgdt;

    EventDispatcher& dispatcher = EventBus::dispatcher();
    TestEvent e;

    dispatcher.dispatch( &e );
}