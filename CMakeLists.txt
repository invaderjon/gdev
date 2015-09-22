cmake_minimum_required( VERSION 2.8 )
project( gdev )

# IGNORE NEW VERSION POLICY
if ( POLICY CMP0048 )
    cmake_policy( SET CMP0048 OLD )
endif()

# PRE-REQS
find_package( Threads REQUIRED )

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
set( LIB_DIR ${PROJECT_SOURCE_DIR}/libs )
set( SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )
set( TEST_DIR ${PROJECT_SOURCE_DIR}/test )
set( OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin )

# FILE CONFIGURATIONS
set( CONFIGURED build )
configure_file( build.cmake.in ${SOURCE_DIR}/engine/build.gen.h )
configure_file( configured.cmake.in ${SOURCE_DIR}/engine/build.gen.cpp )

# CMAKE CONFIGURATION
set( CMAKE_CXX_FLAGS "-std=c++0x" )

# INCLUDES
include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)

# ENGINE SOURCES
set( ENGINE_FILES
    src/engine/build.gen.cpp
    src/engine/build.gen.h
    src/engine/memory/iallocator.cpp
    src/engine/memory/iallocator.h
    src/engine/memory/default_allocator.cpp
    src/engine/memory/default_allocator.h
    src/engine/memory/counting_allocator.cpp
    src/engine/memory/counting_allocator.h
    src/engine/containers/dynamic_array.cpp
    src/engine/containers/dynamic_array.h
    src/engine/containers/map.cpp
    src/engine/containers/map.h
    src/engine/memory/mem.cpp
    src/engine/memory/mem.h
    src/engine/util/json_printer.cpp
    src/engine/util/json_printer.h
)

set( SOURCE_FILES
    game.m.cpp
    ${ENGINE_FILES} )

# BUILD GAME
add_executable( gdev ${SOURCE_FILES} )

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
    set( TEST_FILES
        test.m.cpp
        ${ENGINE_FILES}
        test/trival_tests.cpp
        test/engine/memory/default_allocator.t.cpp
        test/engine/memory/counting_allocator.t.cpp
        test/engine/containers/dynamic_array.t.cpp
        test/engine/containers/map.t.cpp
        test/engine/util/json_printer.t.cpp
    )

    # BUILD
    add_executable( tests ${TEST_FILES} )

    target_link_libraries( tests gtest )

    # MAKE TESTS
    include( CTest )
    enable_testing()

    add_test( unit ${PROJECT_BINARY_DIR}/tests )
endif()