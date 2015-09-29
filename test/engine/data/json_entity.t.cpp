// json_entity.t.cpp
#include "engine/data/json_entity.h"
#include "engine/data/json_parser.h"
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

TEST( JsonEntityTest, JsonPrint )
{
    using namespace StevensDev::sgdd;
    using namespace StevensDev::sgdm;

    // test simple array
    std::string arr = "[ true, 0, -23.2, \".hack\", null ]";
    std::string obj = "{ \"b\": \"too\", \"c\": \"many\", \"d\": \"cooks\" }";

    DefaultAllocator<JsonEntity> def;

    JsonEntity* jsonArr = JsonParser::fromString( arr );
    JsonEntity* jsonObj = JsonParser::fromString( obj );

    std::ostringstream ossArr;
    std::ostringstream ossObj;

    ossArr << *jsonArr;
    ossObj << *jsonObj;

    EXPECT_STREQ( arr.c_str(), ossArr.str().c_str() );
    EXPECT_STREQ( obj.c_str(), ossObj.str().c_str() );
    
}