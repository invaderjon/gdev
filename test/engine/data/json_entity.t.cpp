// json_entity.t.cpp
#include "engine/data/json_entity.h"
#include <gtest/gtest.h>

TEST( JsonEntityTest, Construction )
{
    using namespace StevensDev::sgdd;

    JsonEntity entity;

    EXPECT_TRUE( entity.isNull() );
    EXPECT_EQ( JsonEntity::EMPTY, entity.type() );
}

TEST( JsonEntityTest, DataTypes )
{
    using namespace StevensDev::sgdd;

    JsonEntity entity( 5 );
    EXPECT_TRUE( entity.isInt() );
    EXPECT_EQ( JsonEntity::INTEGRAL, entity.type() );
    EXPECT_EQ( 5, entity.asInt() );

    entity = JsonEntity( 3.0 );
    EXPECT_TRUE( entity.isDouble() );
    EXPECT_EQ( JsonEntity::DECIMAL, entity.type() );
    EXPECT_EQ( 3.0, entity.asDouble());

    entity = JsonEntity( false );
    EXPECT_TRUE( entity.isBoolean() );
    EXPECT_EQ( JsonEntity::BOOLEAN, entity.type() );
    EXPECT_FALSE( entity.asBoolean() );
}