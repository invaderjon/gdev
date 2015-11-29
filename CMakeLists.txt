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

find_library(
    LIB_GEL
    NAMES gel libgel
    NO_DEFAULT_PATH
    PATHS ${CMAKE_SOURCE_DIR}/lib
)

if ( LIB_GEL MATCHES NOTFOUND )
    message( FATAL_ERROR "The dependency gel must be located in ./libs " )
endif()

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
configure_file( build.cmake.in ${SOURCE_DIR}/engine/build.g.h )
configure_file( configured.cmake.in ${SOURCE_DIR}/engine/build.g.cpp )

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
#    src/engine/assets/data_manager.cpp
#    src/engine/assets/data_manager.h
    src/engine/assets/handle.cpp
    src/engine/assets/handle.h
    src/engine/assets/handle_manager.cpp
    src/engine/assets/handle_manager.h
#    src/engine/assets/idata_factory.cpp
#    src/engine/assets/idata_factory.h
    src/engine/build.g.cpp
    src/engine/build.g.h
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
    src/engine/events/event_bus.cpp
    src/engine/events/event_bus.h
    src/engine/events/event_dispatcher.cpp
    src/engine/events/event_dispatcher.h
    src/engine/events/ievent.cpp
    src/engine/events/ievent.h
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
    src/engine/scene/collision.cpp
    src/engine/scene/collision.h
    src/engine/scene/icollider.cpp
    src/engine/scene/icollider.h
    src/engine/scene/rectangle_bounds.cpp
    src/engine/scene/rectangle_bounds.h
    src/engine/scene/nxn_scene_graph.cpp
    src/engine/scene/nxn_scene_graph.h
    src/engine/scene/world.cpp
    src/engine/scene/world.h
#    src/engine/events/resource_event.cpp
#    src/engine/events/resource_event.h
)

# SFML DEPENDENT ENGINE SOURCES
set(
    ENGINE_SFML_DEP_FILES
#    src/engine/assets/resource_manager.cpp
#    src/engine/assets/resource_manager.h
#    src/engine/game/resource_manager.cpp
#    src/engine/game/resource_manager.h
    src/engine/input/input.cpp
    src/engine/input/input.h
    src/engine/rendering/renderable_sprite.cpp
    src/engine/rendering/renderable_sprite.h
    src/engine/rendering/renderer.cpp
    src/engine/rendering/renderer.h
    src/engine/scene/scene.cpp
    src/engine/scene/scene.h
    src/engine/util/input_utils.cpp
    src/engine/util/input_utils.h
)

# GAME SOURCES
set(
    GAME_FILES
)

# SFML DEPENDENT GAME SOURCES
set(
    GAME_SFML_DEP_FILES
    src/game/objects/actor.cpp
    src/game/objects/actor.h
    src/game/objects/iactor_factory.cpp
    src/game/objects/iactor_factory.h
    src/game/controllers/player_controller.cpp
    src/game/controllers/player_controller.h
)

#
set(
    GAME_SOURCE_FILES
    game.m.cpp
    ${GAME_FILES}
    ${GAME_SFML_DEP_FILES}
    ${ENGINE_FILES}
    ${ENGINE_SFML_DEP_FILES} )

# GAME RESOURCES (COPY)
file( GLOB RESOURCES "${RES_DIR}/*")
file( COPY ${RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}/res" )

# TRAVIS-CI GUARD
if( NOT DEFINED TRAVIS_CI )
    # BUILD GAME
    add_executable( gdev ${GAME_SOURCE_FILES} )

    target_link_libraries(
        gdev
        ${SFML_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${LIB_GEL}
    )
endif()

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
        ${GAME_FILES}
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
        test/engine/events/test_event.cpp
        test/engine/events/test_event.h
        test/engine/events/test_listener.cpp
        test/engine/events/test_listener.h
        test/engine/events/event_bus.t.cpp
        test/engine/events/event_dispatcher.t.cpp
        test/engine/memory/mem.t.cpp
        test/engine/memory/stack_guard.t.cpp
        test/engine/scene/test_collider.cpp
        test/engine/scene/test_collider.h
        test/engine/scene/test_tickable.cpp
        test/engine/scene/test_tickable.h
        test/engine/util/string_utils.t.cpp
        test/engine/scene/nxn_scene_graph.t.cpp
        test/engine/scene/rectangle_bounds.t.cpp
        test/engine/assets/handle.t.cpp test/engine/assets/handle_manager.t.cpp )

    # SFML DEPENDENT TESTS
    set(
        TEST_SFML_DEP_FILES
        test/engine/input/input.t.cpp
        test/engine/rendering/renderable_sprite.t.cpp
        test/engine/rendering/renderer.t.cpp
        test/engine/scene/scene.t.cpp
        test/engine/scene/test_input_controller.cpp
        test/engine/scene/test_input_controller.h
        test/game/controllers/player_controller.t.cpp )

    # TRAVIS-CI GUARD
    if( NOT DEFINED TRAVIS_CI )
        set(
            TEST_FILES
            ${TEST_FILES}
            ${GAME_SFML_DEP_FILES}
            ${ENGINE_SFML_DEP_FILES}
            ${TEST_SFML_DEP_FILES}
        )
        set( SFLML_LIBRARIES_FOR_TEST ${SFML_LIBRARIES} )
    endif()


    # BUILD
    add_executable( tests ${TEST_FILES}  )

    target_link_libraries(
        tests
        gtest
        ${CMAKE_THREAD_LIBS_INIT}
        ${SFLML_LIBRARIES_FOR_TEST}
        ${LIB_GEL}
    )

    # MAKE TESTS
    include( CTest )
    enable_testing()

    add_test( unit ${PROJECT_BINARY_DIR}/tests  )
endif()