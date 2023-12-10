# /CMakeLists.txt

cmake_minimum_required(VERSION 3.21)
project(AtlasGameManager LANGUAGES CXX C)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MESSAGE_LOG_LEVEL DEBUG CACHE STRING "CMake messaging level")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/data)

option(BUILD_SHARED_LIBS "Build shared library" ON) 

#Enable cmake_modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
include(common)

message("-- CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_BUILD_TYPE)

PlatformPreSetup()
CompilerPreSetup()
message("-- FGL_FLAGS: ${FGL_FLAGS}")

include(dependencies)

find_package(SQLite3 3.31.1 REQUIRED)
message("-- include = ${SQLite3_INCLUDE_DIR}")
message("-- lib = ${SQLite3_LIBRARY}")

message("-- FGL_FLAGS: ${FGL_FLAGS}")

add_subdirectory(atlas)

if (WIN32)
    add_subdirectory(atlas-updater)
endif()


SetVersionInfo()
CompilerPostSetup()