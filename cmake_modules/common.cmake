message(DEBUG "Entering ${CMAKE_CURRENT_LIST_FILE}")
message(DEBUG "Platform: ${CMAKE_CXX_PLATFORM_ID}")
message(DEBUG "Compiler: ${CMAKE_CXX_COMPILER_ID}")



if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    include(gcc)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    include(clang)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message()
    include(msvc)
endif()

if(WIN32)
    include(win32)
elseif (LINUX)
    include(linux)
else (APPLE)
endif ()

include(profiling)
message(DEBUG "Leaving ${CMAKE_CURRENT_LIST_FILE}")
