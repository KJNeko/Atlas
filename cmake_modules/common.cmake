# /cmake_modules/common.cmake

message(DEBUG "Entering ${CMAKE_CURRENT_LIST_FILE}")
message(DEBUG "Platform: ${CMAKE_CXX_PLATFORM_ID}")
message(DEBUG "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(DEBUG "Compiler: ${CMAKE_CXX_COMPILER}")

include(qt)

if ((${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") OR (${CMAKE_CXX_PLATFORM_ID} STREQUAL "MINGW"))
    include(gcc)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    include(clang)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message()
    include(msvc)
endif()

if((WIN32))
    message(DEBUG "Compiling for Windows")
    include(win32)
elseif (APPLE)
    message(DEBUG "Compiling for Apple")
elseif (UNIX)
    message(DEBUG "Compiling for Unix")
    include(linux)
else()
    message(DEBUG "Unknown Platform")
endif ()

include(profiling)
message(DEBUG "Leaving ${CMAKE_CURRENT_LIST_FILE}")
