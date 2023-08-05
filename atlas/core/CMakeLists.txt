# /atlas/core/CMakeLists.txt

project(AtlasCore)

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(
    ${PROJECT_NAME}
        ${SOURCES}
)
