cmake_minimum_required( VERSION 2.8 )
project( gdev )

# PRE-REQS
find_package( Threads REQUIRED )

# PROJECT INFORMATION
set( PROJECT_VERSION_MAJOR 0 )
set( PROJECT_VERSION_MINOR 0 )
set( PROJECT_VERSION_PATCH 0 )
set( PROJECT_VERSION
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )

# DIRECTORIES
set( INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include )
set( EXT_DIR ${PROJECT_SOURCE_DIR}/ext )
set( LIB_DIR ${PROJECT_SOURCE_DIR}/libs )
set( SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )
set( TEST_DIR ${PROJECT_SOURCE_DIR}/test )
set( OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin )

# FILE CONFIGURATIONS
set( CONFIGURED build )
configure_file( build.cmake.in ${SOURCE_DIR}/build.h )
configure_file( configured.cmake.in ${SOURCE_DIR}/build.cpp )

# INCLUDES
include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)

# SOURCES
file( GLOB SOURCE_GLOB ${SOURCE_DIR}/main/*.cpp )
set( SOURCE_FILES game.m.cpp )
list( APPEND SOURCE_FILES ${SOURCE_GLOB} )

# BUILD
add_executable( gdev ${SOURCE_FILES} )

# TESTS
if ( BUILD_TESTS )
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

    # SOURCES
    set( TEST_FILES test.m.cpp )
    file( GLOB TEST_GLOB ${TEST_DIR}/*.cpp )
    list( APPEND TEST_FILES ${TEST_GLOB} )

    # BUILD
    add_executable(
        tests
        ${TEST_FILES}
    )

    target_link_libraries( tests gtest )

    # MAKE TESTS
    include( CTest )
    enable_testing()

    add_test( unit ${PROJECT_BINARY_DIR}/tests )
endif()