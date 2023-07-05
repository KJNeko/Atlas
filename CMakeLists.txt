cmake_minimum_required(VERSION 3.21)
project(AtlasGameManager LANGUAGES CXX C)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_BUILD_TYPE)

if (WIN32)
    string(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ";.dll")

    if (DEFINED ENV{QT_PATH})
        set(QT_PATH $ENV{QT_PATH})
        message("Setting QT path from ENV")
    endif ()
endif ()


option(ATLAS_PROFILE_ENABLE "" OFF)

if (${ATLAS_PROFILE_ENABLE} STREQUAL "ON")
    option(TRACY_ENABLE "" ON)
    option(TRACY_ON_DEMAND "" OFF)
    option(TRACY_NO_BROADCAST "" ON)
    option(TRACY_NO_VSYNC_CAPTURE "" ON)
    option(TRACY_NO_FRAME_IMAGE "" ON)
else ()
    option(TRACY_ENABLE "" OFF)
endif ()

add_subdirectory(dependencies/tracy)

option(SPDLOG_FMT_EXTERNAL "" ON)
add_subdirectory(dependencies/fmt)
#add_subdirectory(dependencies/catch2)
add_subdirectory(dependencies/spdlog)

#lz4
include(dependencies/lz4_build.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cmake)

find_package(SQLite3 3.41.2 REQUIRED)
message("include = ${SQLite3_INCLUDE_DIR}")
message("lib = ${SQLite3_LIBRARY}")

if (WIN32)
    set(QT_PATH "C:/Qt/6.4.3/mingw_64")
endif ()


if (DEFINED QT_PATH)
    list(APPEND CMAKE_PREFIX_PATH ${QT_PATH})
endif ()


find_package(Qt6 COMPONENTS Widgets Core Concurrent Network Test Charts REQUIRED)

if (WIN32)
    find_program(TOOL_WINDEPLOYQT windeployqt REQUIRED NO_CACHE)
endif ()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #These two flags added with older gcc versons and Qt causes a compiler segfault -Wmismatched-tags -Wredundant-tags
    # STL has some warnings that prevent -Werror level compilation "-Wnull-dereference"
    set(FGL_WARNINGS "-Wall -Wundef -Wextra -Wpessimizing-move -Wpedantic -Weffc++ -pedantic-errors -Wnoexcept -Wuninitialized -Wunused -Wunused-parameter -Winit-self -Wconversion -Wuseless-cast -Wextra-semi -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wformat-signedness -Wno-format-zero-length -Wmissing-include-dirs -Wshift-overflow=2 -Walloc-zero -Walloca -Wsign-promo -Wconversion -Wduplicated-branches -Wduplicated-cond -Wshadow -Wshadow=local -Wvirtual-inheritance -Wno-virtual-move-assign -Wunsafe-loop-optimizations -Wnormalized -Wpacked -Wredundant-decls -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wzero-as-null-pointer-constant -Wconditionally-supported -Wwrite-strings -Wunused-const-variable=2 -Wdouble-promotion -Wpointer-arith -Wcast-align=strict -Wcast-qual -Wconversion -Wsign-conversion -Wimplicit-fallthrough=1 -Wmisleading-indentation -Wdangling-else -Wdate-time -Wformat=2 -Wformat-overflow=2 -Wformat-signedness -Wformat-truncation=2 -Wswitch-default -Wstringop-overflow=4 -Warray-bounds=2 -Wattribute-alias=2 -Wcatch-value=2 -Wplacement-new=2 -Wtrampolines -Winvalid-imported-macros -Winvalid-imported-macros")

    set(FGL_CONFIG "-std=c++20 -fmax-errors=3 -fconcepts-diagnostics-depth=4")
    set(FGL_DEBUG "-Og -g -fstrict-aliasing -fno-omit-frame-pointer -fstack-check -ftrapv -fverbose-asm -femit-class-debug-always")
    #Generates system specific stuff (IE requires AVX)
    set(FGL_SYSTEM_SPECIFIC "-march=native -fgcse -fgcse-las -fgcse-sm -fdeclone-ctor-dtor -fdevirtualize-speculatively -ftree-loop-im -fivopts -ftree-loop-ivcanon -fira-hoist-pressure -fsched-pressure -fsched-spec-load -fipa-pta -flto=auto -s -ffat-lto-objects -fno-enforce-eh-specs -fstrict-enums -funroll-loops")
    #Generates safe optimization flags
    set(FGL_SYSTEM_SAFE "-O3 -fdevirtualize-at-ltrans -s")
    set(FGL_FLAGS_DEBUG "${FGL_WARNINGS} ${FGL_CONFIG} ${FGL_DEBUG}")
    set(FGL_FLAGS_SYSTEM "${FLG_CONFIG} -DNDEBUG ${FGL_SYSTEM_SAFE} ${FGL_SYSTEM_SPECIFIC}")
    set(FGL_FLAGS_RELEASE "${FGL_CONFIG} -DNDEBUG -s ${FGL_SYSTEM_SAFE} ${FGL_WARNINGS} -Werror")
    set(FGL_FLAGS_RELWITHDEBINFO "${FGL_CONFIG} -DNDEBUG -g ${FGL_SYSTEM_SAFE} ${FGL_SYSTEM_SPECIFIC}")
    set(FGL_FLAGS "${FGL_FLAGS_${UPPER_BUILD_TYPE}}")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(FGL_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /Zc:__cplusplus")
else ()
    set(FGL_FLAGS ${CMAKE_CXX_FLAGS})
endif ()

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/atlas/core/*.cpp")

file(GLOB_RECURSE UI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/atlas/ui/*.cpp")

#Build exe
message("FGL_FLAGS: ${FGL_FLAGS}")

add_executable(Atlas ${UI_SOURCES} ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/atlas/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/atlas/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
target_include_directories(Atlas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/atlas)
target_link_libraries(Atlas PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent Qt6::Network Qt6::Charts SQLite::SQLite3 fmt::fmt spdlog::spdlog TracyClient lz4)
set_target_properties(Atlas PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})
if (WIN32)
    target_compile_definitions(Atlas PRIVATE UNICODE=1)
endif ()

#Post setup (Windows only)
if (WIN32)
    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${TOOL_WINDEPLOYQT} --compiler-runtime --no-translations --no-system-d3d-compiler --no-opengl-sw
            $<TARGET_FILE:Atlas>
            COMMENT "Running windeployqt")

    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Atlas> $<TARGET_FILE_DIR:Atlas>
            COMMAND_EXPAND_LISTS)

    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SQLite3_LIBRARY} $<TARGET_FILE_DIR:Atlas>)

    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/3rdparty/qwebp.dll
            ${CMAKE_BINARY_DIR}/bin/imageformats
            COMMENT "Copying pre-compiled qwebp.dll for Qt 6.4.3")

endif ()

add_custom_command(TARGET Atlas POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/atlas/ui/qss $<TARGET_FILE_DIR:Atlas>/data/themes COMMENT "Adding qss files")

file(GLOB_RECURSE TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

add_executable(AtlasTests ${TESTS} ${SOURCES} ${UI_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/atlas/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
target_include_directories(AtlasTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/atlas)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/catch2)
target_link_libraries(AtlasTests PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent Qt6::Network Qt6::Charts Qt6::Test SQLite::SQLite3 fmt::fmt spdlog::spdlog Catch2::Catch2WithMain TracyClient lz4)
set_target_properties(AtlasTests PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})
if (WIN32)
    target_compile_definitions(AtlasTests PRIVATE UNICODE=1)
endif ()
