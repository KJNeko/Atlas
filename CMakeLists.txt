cmake_minimum_required(VERSION 3.21)
project(AtlasGameManager LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_BUILD_TYPE)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND DEFINED ATLAS_PROFILE_ENABLE)
    option(TRACY_ENABLE "" ON)
    option(TRACY_ON_DEMAND "" OFF)
    option(TRACY_NO_BROADCAST "" ON)
    option(TRACY_NO_VSYNC_CAPTURE "" ON)
    option(TRACY_NO_FRAME_IMAGE "" ON)
else ()
    option(TRACY_ENABLE "" OFF)
endif ()

add_subdirectory(dependencies/tracy)
add_subdirectory(dependencies/gtest)
add_subdirectory(dependencies/spdlog)
add_subdirectory(dependencies/fmt)



if (WIN32)
    if (DEFINED SQL_PATH)
        set(SQLite3_INCLUDE_DIR ${SQL_PATH}\\include)
        set(SQLite3_LIBRARY ${SQL_PATH}\\lib\\sqlite3.dll)
        set(SQLite3_FOUND true)
        message("FOUND SQL_PATH")
    elseif (DEFINED ENV{SQLITE3_PATH})
        set(SQLite3_INCLUDE_DIR $ENV{SQLITE3_PATH}\\include)
        set(SQLite3_LIBRARY $ENV{SQLITE3_PATH}\\lib\\sqlite3.dll)
        set(SQLite3_FOUND true)
        message("FOUND ENV SQLITE3_PATH")
    else ()
        message("Must define SQL_PATH with -DSQL_PATH=?? or add to path as SQLITE3_PATH")
    endif ()
    message("SQlite3_INCLUDE_DIR = ${SQLite3_INCLUDE_DIR}")
    message("SQlite3_LIBRARY = ${SQLite3_LIBRARY}")

    if(DEFINED ENV{QT_PATH})
        set(QT_PATH $ENV{QT_PATH})
        message("Setting QT path from ENV")
    endif()
endif ()

#set(QT_PATH "C:/Qt/6.4.3/mingw_64")

if (DEFINED QT_PATH)
    list(APPEND CMAKE_PREFIX_PATH ${QT_PATH})
endif ()

find_package(SQLite3 REQUIRED)
find_package(Qt6 COMPONENTS Widgets Core Concurrent Network REQUIRED)

if (WIN32)
    find_program(TOOL_WINDEPLOYQT windeployqt REQUIRED NO_CACHE)
endif ()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (${CMAKE_COMPILER_IS_GNUCXX})
    #sqlite_modern is stupid so we have to disable -Wundef
    #These two flags added with older gcc versons and Qt causes a compiler segfault -Wmismatched-tags -Wredundant-tags
    set(FGL_WARNINGS "-Wall -Wextra -Wnull-dereference -Wpedantic -Weffc++ -pedantic-errors -Wnoexcept -Wuninitialized -Wunused -Wunused-parameter -Winit-self -Wconversion -Wuseless-cast -Wextra-semi -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wformat-signedness -Wno-format-zero-length -Wmissing-include-dirs -Wshift-overflow=2 -Walloc-zero -Walloca -Wsign-promo -Wconversion -Wduplicated-branches -Wduplicated-cond -Wshadow -Wshadow=local -Wvirtual-inheritance -Wno-virtual-move-assign -Wunsafe-loop-optimizations -Wnormalized -Wpacked -Wredundant-decls -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wzero-as-null-pointer-constant -Wconditionally-supported -Wwrite-strings -Wunused-const-variable=2 -Wdouble-promotion -Wpointer-arith -Wcast-align=strict -Wcast-qual -Wconversion -Wsign-conversion -Wimplicit-fallthrough=1 -Wmisleading-indentation -Wdangling-else -Wdate-time -Wformat=2 -Wformat-overflow=2 -Wformat-signedness -Wformat-truncation=2 -Wswitch-default -Wstringop-overflow=4 -Warray-bounds=2 -Wattribute-alias=2 -Wcatch-value=2 -Wplacement-new=2 -Wtrampolines -Winvalid-imported-macros -Winvalid-imported-macros")

    set(FGL_CONFIG "-std=c++20 -fmax-errors=3 -fconcepts-diagnostics-depth=4")
    set(FGL_DEBUG "-Og -fverbose-asm -g -fstrict-aliasing -fno-omit-frame-pointer -fstack-check -ftrapv -fverbose-asm -femit-class-debug-always")
    #Generates system specific stuff (IE requires AVX)
    set(FGL_SYSTEM_SPECIFIC "-march=native -fgcse -fgcse-las -fgcse-sm -fdeclone-ctor-dtor -fdevirtualize-speculatively -ftree-loop-im -fivopts -ftree-loop-ivcanon -fira-hoist-pressure -fsched-pressure -fsched-spec-load -fipa-pta -flto=auto -s -ffat-lto-objects -fno-enforce-eh-specs -fstrict-enums -funroll-loops")
    #Generates safe optimization flags
    set(FGL_SYSTEM_SAFE "-O3 -fdevirtualize-at-ltrans -s")
    set(FGL_FLAGS_DEBUG "${FGL_WARNINGS} ${FGL_CONFIG} ${FGL_DEBUG}")
    set(FGL_FLAGS_SYSTEM "${FLG_CONFIG} -DNDEBUG ${FGL_SYSTEM_SAFE} ${FGL_SYSTEM_SPECIFIC}")
    set(FGL_FLAGS_RELEASE "${FGL_CONFIG} -DNDEBUG -s ${FGL_SYSTEM_SAFE}")
    set(FGL_FLAGS_RELWITHDEBINFO "${FGL_CONFIG} -DNDEBUG -g ${FGL_SYSTEM_SAFE} ${FGL_SYSTEM_SPECIFIC}")
    set(FGL_FLAGS "${FGL_FLAGS_${UPPER_BUILD_TYPE}}")
else ()
    set(FGL_FLAGS ${CMAKE_CXX_FLAGS})
endif ()

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/atlas/core/*.cpp")

file(GLOB_RECURSE UI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/atlas/ui/*.cpp")

#Build exe
message("FGL_FLAGS: ${FGL_FLAGS}")

add_executable(Atlas ${UI_SOURCES} ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/atlas/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/atlas/resources.qrc)
target_include_directories(Atlas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Atlas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite_modern_cpp/hdr)
target_link_libraries(Atlas PRIVATE Tracy::TracyClient)
target_link_libraries(Atlas PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent SQLite::SQLite3 fmt::fmt spdlog::spdlog)
set_target_properties(Atlas PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})

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
            COMMENT "Copying pre-cmopiled qwebp.dll for Qt 6.4.3")

endif ()

add_custom_command(TARGET Atlas POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/atlas/ui/qss $<TARGET_FILE_DIR:Atlas>/data/themes COMMENT "Adding qss files")

#Test builds
file(GLOB_RECURSE TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

add_executable(AtlasTESTS ${TESTS} ${SOURCES})

target_include_directories(AtlasTESTS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(AtlasTESTS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite_modern_cpp/hdr)
target_link_libraries(AtlasTESTS PUBLIC GTest::gtest_main Qt6::Core Tracy::TracyClient Qt6::Widgets Qt6::Concurrent SQLite::SQLite3 fmt::fmt spdlog::spdlog)

if (DEFINED ATLAS_TEST_UI)
    target_compile_definitions(AtlasTESTS PUBLIC ATLAS_TEST_UI="${ATLAS_TEST_UI}")
endif ()

#Test post setup (Windows only)
if (WIN32)
    add_custom_command(TARGET AtlasTESTS POST_BUILD
            COMMAND ${TOOL_WINDEPLOYQT} --compiler-runtime --no-translations  --no-system-d3d-compiler --no-opengl-sw
            $<TARGET_FILE:AtlasTESTS>
            COMMENT "Running windeployqt")

    add_custom_command(TARGET AtlasTESTS POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:AtlasTESTS> $<TARGET_FILE_DIR:AtlasTESTS>
            COMMAND_EXPAND_LISTS COMMENT "Copying runtime DLLS")

    add_custom_command(TARGET AtlasTESTS POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SQLite3_LIBRARY} $<TARGET_FILE_DIR:AtlasTESTS> COMMENT "Copying sqlite lib files")
endif ()

if (NOT DEFINED FORCE_DISCOVER_TESTS AND ENABLE_TRACY EQUAL OFF)
    include(GoogleTest)
    gtest_discover_tests(AtlasTESTS)
endif ()
