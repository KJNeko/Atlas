cmake_minimum_required(VERSION 3.21)
project(Atlas)

add_subdirectory(atlas)
if (WIN32)
    add_subdirectory(3rdparty/qtimageformats)
    message("Compiling qtimageformats for Windows")
endif()