cmake_minimum_required(VERSION 3.21)
project(lz4 C)

set(LZ4_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lz4/lib)

file(GLOB_RECURSE LZ4_SOURCES ${LZ4_DIR}/*.c)

add_library(lz4 STATIC ${LZ4_SOURCES})
target_include_directories(lz4 PUBLIC ${LZ4_DIR})
