// json_parser.t.cpp
#include "engine/data/json_entity.h"
#include "engine/data/json_parser.h"
#include "engine/memory/counting_allocator.h"
#include <gtest/gtest.h>

TEST( JsonParserTest, ParseEmptyJson )
{
    using namespace StevensDev::sgdd;
    using namespace StevensDev::sgdm;

    DefaultAllocator<JsonEntity> def;
    def.get( 1 );

    // global allocator, object with whitespace
    EXPECT_NO_THROW( StackGuard<JsonEntity> guard2(
        nullptr, JsonParser::fromString( "{ }" ) ) );

    // global allocator, array
    EXPECT_NO_THROW( StackGuard<JsonEntity> guard3(
        nullptr,
        JsonParser::fromString( "[]" ) ) );

    // global allocator, array with whitespace
    EXPECT_NO_THROW( StackGuard<JsonEntity> guard4(
        nullptr,
        JsonParser::fromString( "[ ]" ) ) );

    // custom allocator
    EXPECT_NO_THROW( StackGuard<JsonEntity> guard5(
        &def, JsonParser::fromString( "{}", &def ) ) );

    // custom allocator, with whitespace
    EXPECT_NO_THROW( StackGuard<JsonEntity> guard6(
        &def, JsonParser::fromString( "{ }", &def ) ) );
}

TEST( JsonParserTest, ParseArray )
{
    using namespace StevensDev::sgdd;
    using namespace StevensDev::sgdm;

    // test simple array
    std::string simple = "[ true, 0, -23.0, \".hack\", null ]";

    DefaultAllocator<JsonEntity> def;

    StackGuard<JsonEntity> guard(
        &def, JsonParser::fromString( simple, &def ) );

    EXPECT_TRUE( ( *guard )[0].asBoolean() );
    EXPECT_EQ( 0, ( *guard )[1].asInt() );
    EXPECT_EQ( -23.0, ( *guard )[2].asDouble() );
    EXPECT_STREQ( ".hack", ( *guard )[3].asString().c_str() );
    EXPECT_TRUE( ( *guard )[4].isNull() );
}

TEST( JsonParserTest, ParseObject )
{
    using namespace StevensDev::sgdd;
    using namespace StevensDev::sgdm;

    // test simple json object
    std::string simple =
        "{ \"name\": \"bowl\", \"alpha\": 1.0, \"translucent\": false }";

    DefaultAllocator<JsonEntity> def;

    StackGuard<JsonEntity> guard(
        &def, JsonParser::fromString( simple, &def ) );

    EXPECT_EQ( JsonEntity::OBJECT, guard->type() );
    EXPECT_STREQ( "bowl", ( *guard )["name"].asString().c_str() );
    EXPECT_EQ( 1.0, ( *guard )["alpha"].asDouble() );
    EXPECT_FALSE( ( *guard )["translucent"].asBoolean() );

    // test nested json object
    std::string nested =
        "{ \"player\": { \"model\": { \"id\": \"ellie_daxter_hoodie\" } } }";

    StackGuard<JsonEntity> guard2(
        &def, JsonParser::fromString( nested, &def ) );

    EXPECT_EQ( JsonEntity::OBJECT, guard->type() );
    EXPECT_TRUE( ( *guard2 )["player"].isObject() );
    EXPECT_TRUE( ( *guard2 )["player"]["model"].isObject() );
    EXPECT_STREQ( "ellie_daxter_hoodie",
                  ( *guard2 )["player"]["model"]["id"].asString().c_str() );

    EXPECT_NO_THROW(
        StackGuard<JsonEntity>( &def, JsonParser::fromString( "" ) ) );
}

TEST( JsonParserTest, ParseFile )
{
    using namespace StevensDev::sgdd;
    using namespace StevensDev::sgdm;

    DefaultAllocator<JsonEntity> def;

    JsonEntity* e = JsonParser::fromFile(
        "./res/config/actor/block/enemy.conf", &def );

    StackGuard<JsonEntity> guard( &def, e );
}

TEST( JsonParserTest, InvalidJson )
{
    using namespace StevensDev::sgdd;
    using namespace StevensDev::sgdm;

    EXPECT_THROW(
        StackGuard<JsonEntity>( nullptr, JsonParser::fromString( "{\"}" ) ),
        std::runtime_error );

    EXPECT_THROW(
        StackGuard<JsonEntity>( nullptr, JsonParser::fromString( "[ \" ]" ) ),
        std::runtime_error );

    EXPECT_THROW(
        StackGuard<JsonEntity>( nullptr, JsonParser::fromString( "{  " ) ),
        std::runtime_error );

    EXPECT_THROW(
        StackGuard<JsonEntity>( nullptr, JsonParser::fromString( "  }" ) ),
        std::runtime_error );

    EXPECT_THROW(
        StackGuard<JsonEntity>( nullptr, JsonParser::fromString( "[  }" ) ),
        std::runtime_error );

    EXPECT_THROW(
        StackGuard<JsonEntity>( nullptr, JsonParser::fromString( "   ]" ) ),
        std::runtime_error );

    EXPECT_THROW(
        StackGuard<JsonEntity>( nullptr,
                                JsonParser::fromString( "[ truw ]" ) ),
        std::runtime_error );

    EXPECT_THROW(
        StackGuard<JsonEntity>( nullptr,
                                JsonParser::fromString( "[ null2 ]" ) ),
        std::runtime_error );

    EXPECT_THROW(
        StackGuard<JsonEntity>( nullptr,
                                JsonParser::fromString( "[ 0, , 12 ]" ) ),
        std::runtime_error );
}