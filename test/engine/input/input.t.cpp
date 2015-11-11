// input.t.cpp
#include <engine/input/input.h>
#include <gtest/gtest.h>

TEST( InputTest, Singleton )
{
    using namespace StevensDev::sgdi;

    Input& inst = Input::inst();
}

TEST( InputTest, TickPhases )
{
    using namespace StevensDev::sgdi;

    Input& inst = Input::inst();

    inst.preTick();
    inst.tick( 1.0f );
    inst.postTick();
}