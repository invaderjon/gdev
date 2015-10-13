cmake_minimum_required( VERSION 2.8 )
project( gdev )

# IGNORE NEW VERSION POLICY
if ( POLICY CMP0048 )
    cmake_policy( SET CMP0048 OLD )
endif()

# PRE-REQS
list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules" )

message( STATUS "location: ${PROJECT_SOURCE_DIR}" )

set( SFML_ROOT ${PROJECT_SOURCE_DIR} )
find_package( Threads REQUIRED )
find_package( SFML REQUIRED system window graphics network audio )

message( STATUS "${SFML_LIBRARIES}" )

#find_package( OpenGL REQUIRED )

# PROJECT INFORMATION
set( PROJ_PREFIX GAME )
set( PROJ_VERSION_MAJOR 0 )
set( PROJ_VERSION_MINOR 1 )
set( PROJ_VERSION_PATCH 0 )
set( PROJ_VERSION
    ${PROJ_VERSION_MAJOR}.${PROJ_VERSION_MINOR}.${PROJ_VERSION_PATCH}
)

if ( CMAKE_BUILD_TYPE MATCHES Debug )
    set( PROJ_BUILD GAME_BUILD_DEBUG )
elseif ( CMAKE_BUILD_TYPE MATCHES RelWithDebInfo )
    set( PROJ_BUILD GAME_BUILD_STAGE )
else()
    set( PROJ_BUILD GAME_BUILD_RELEASE )
endif()

# DIRECTORIES
set( INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include )
set( EXT_DIR ${PROJECT_SOURCE_DIR}/ext )
set( LIB_DIR ${PROJECT_SOURCE_DIR}/lib )
set( SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )
set( RES_DIR ${PROJECT_SOURCE_DIR}/res )
set( TEST_DIR ${PROJECT_SOURCE_DIR}/test )
set( OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin )

# FILE CONFIGURATIONS
set( CONFIGURED build )
configure_file( build.cmake.in ${SOURCE_DIR}/engine/build.gen.h )
configure_file( configured.cmake.in ${SOURCE_DIR}/engine/build.gen.cpp )

# CMAKE CONFIGURATION
set( CMAKE_CXX_FLAGS "-std=c++0x" )

# INCLUDES AND LINK DIRECTORIES
include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)

# ENGINE SOURCES
set(
    ENGINE_FILES
    src/engine/build.gen.cpp
    src/engine/build.gen.h
    src/engine/data/json_printer.cpp
    src/engine/data/json_printer.h
    src/engine/containers/dynamic_array.cpp
    src/engine/containers/dynamic_array.h
    src/engine/containers/map.cpp
    src/engine/containers/map.h
    src/engine/data/json_entity.cpp
    src/engine/data/json_entity.h
    src/engine/data/json_parser.cpp
    src/engine/data/json_parser.h
    src/engine/memory/allocator_guard.cpp
    src/engine/memory/allocator_guard.h
    src/engine/memory/default_allocator.cpp
    src/engine/memory/default_allocator.h
    src/engine/memory/counting_allocator.cpp
    src/engine/memory/counting_allocator.h
    src/engine/memory/iallocator.cpp
    src/engine/memory/iallocator.h
    src/engine/memory/mem.cpp
    src/engine/memory/mem.h
    src/engine/memory/stack_guard.cpp
    src/engine/memory/stack_guard.h
    src/engine/scene/itickable.cpp
    src/engine/scene/itickable.h
    src/engine/util/game_utils.cpp
    src/engine/util/game_utils.h
    src/engine/util/string_utils.cpp
    src/engine/util/string_utils.h
)

# SFML DEPENDENT SOURCES
set(
    ENGINE_SFML_DEP_FILES
    src/engine/game/resource_manager.cpp
    src/engine/game/resource_manager.h
    src/engine/input/input.cpp
    src/engine/input/input.h
    src/engine/rendering/renderable_sprite.cpp
    src/engine/rendering/renderable_sprite.h
    src/engine/rendering/renderer.cpp
    src/engine/rendering/renderer.h
    src/engine/scene/scene.cpp
    src/engine/scene/scene.h
    src/engine/scene/test_input_controller.cpp
    src/engine/scene/test_input_controller.h
    src/engine/util/input_utils.cpp
    src/engine/util/input_utils.h
)

set(
    SOURCE_FILES
    game.m.cpp
    ${ENGINE_FILES}
    ${ENGINE_SFML_DEP_FILES}
)

# ENGINE RESOURCES (COPY)
file( GLOB RESOURCES "${RES_DIR}/*")
file( COPY ${RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/res" )

# BUILD GAME
add_executable( gdev ${SOURCE_FILES} )

target_link_libraries(
    gdev
    ${SFML_LIBRARIES}
)

# TESTS
if ( BUILD_TESTS OR CLION )
    # TEST INCLUDES
    include_directories(
        ${INCLUDE_DIR}
        ${SOURCE_DIR}
        ${TEST_DIR}
        ${EXT_DIR}/googletest
        ${EXT_DIR}/googletest/include
    )

    # SUB-DIRECTORIES
    add_subdirectory( ${EXT_DIR}/googletest )

    # TEST SOURCES
    set(
        TEST_FILES
        test.m.cpp
        ${ENGINE_FILES}
        test/trival_tests.cpp
        test/engine/memory/allocator_guard.t.cpp
        test/engine/memory/counting_allocator.t.cpp
        test/engine/memory/default_allocator.t.cpp
        test/engine/containers/dynamic_array.t.cpp
        test/engine/containers/map.t.cpp
        test/engine/data/json_entity.t.cpp
        test/engine/data/json_parser.t.cpp
        test/engine/data/json_printer.t.cpp
        test/engine/memory/mem.t.cpp
        test/engine/memory/stack_guard.t.cpp
        test/engine/util/string_utils.t.cpp
    )

    # BUILD
    add_executable( tests ${TEST_FILES} )

    target_link_libraries(
        tests
        gtest
    )

    # MAKE TESTS
    include( CTest )
    enable_testing()

    add_test( unit ${PROJECT_BINARY_DIR}/tests )
endif()