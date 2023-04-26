cmake_minimum_required(VERSION 3.2.2)
project(Atlas)

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.4.3/mingw_64")

add_subdirectory(src)
add_subdirectory(3rdparty/qtimageformats)

#Copy qwebp.dll
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/plugins/imageformats/qwebp.dll
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/imageformats)

message(${CMAKE_CURRENT_BINARY_DIR}/bin/imageformats)


