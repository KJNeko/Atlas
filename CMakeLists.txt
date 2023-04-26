cmake_minimum_required(VERSION 3.2.2)
project(Atlas_dev)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.4.3/mingw_64")

add_subdirectory(src)
add_subdirectory(3rdparty/qtimageformats)

