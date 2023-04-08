cmake_minimum_required(VERSION 3.21)
project(AtlasGameManager LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} ")
string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_BUILD_TYPE)

find_package(Qt6 COMPONENTS Widgets Core Concurrent Test CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Tracy CONFIG REQUIRED)
find_package(SQLite3 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(DEFINED ATLAS_ENABLE_PROFILER)
    #Tracy (Profiler)
    option(TRACY_ENABLE "" ON)
    option(TRACY_ON_DEMAND "" OFF)
else()
    option(TRACY_ENABLE "" OFF)
endif()

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/h95/*.cpp")

file(GLOB_RECURSE UI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp")


set(HYDRUS_SOURCES ${SOURCES})
set(HYDRUS_UI ${UI_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/resources.qrc)
set(REQUIRED_LIBS PUBLIC Qt6::Widgets Qt6::Core Qt6::Concurrent SQLite::SQLite3 spdlog::spdlog fmt::fmt-header-only Tracy::TracyClient)

#Build obj in order to reduce total compile time when compiling the tests too
add_library(AtlasObj OBJECT ${HYDRUS_SOURCES} ${HYDRUS_UI})
target_link_libraries(AtlasObj ${REQUIRED_LIBS})
target_include_directories(AtlasObj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

#Build exe
add_executable(Atlas ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_include_directories(Atlas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/dependencies/sqlite_modern_cpp/hdr)
endif ()

target_link_libraries(Atlas ${REQUIRED_LIBS} AtlasObj)

#Post setup (Windows only)
if (WIN32)
    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${TOOL_WINDEPLOYQT} --compiler-runtime --no-translations --no-opengl-sw
            $<TARGET_FILE:Atlas>
            COMMENT "Running windeployqt")

    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Atlas> $<TARGET_FILE_DIR:Atlas>
            COMMAND_EXPAND_LISTS)

    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SQLite3_LIBRARY} $<TARGET_FILE_DIR:Atlas>)
endif ()

add_custom_command(TARGET Atlas POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/qss $<TARGET_FILE_DIR:Atlas>/data/themes COMMENT "Adding qss files")

#Test builds
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND NOT DEFINED NO_BUILD_TESTS)

    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    file(GLOB_RECURSE TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

    add_executable(AtlasTESTS ${TESTS})

    target_include_directories(AtlasTESTS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(AtlasTESTS PUBLIC ${REQUIRED_LIBS} Qt::Test GTest::gtest_main AtlasObj)

    #Test post setup (Windows only)
    if (WIN32)
        add_custom_command(TARGET AtlasTESTS POST_BUILD
                COMMAND ${TOOL_WINDEPLOYQT} --compiler-runtime --no-translations --no-opengl-sw
                $<TARGET_FILE:AtlasTESTS>
                COMMENT "Running windeployqt")

        add_custom_command(TARGET AtlasTESTS POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:AtlasTESTS> $<TARGET_FILE_DIR:AtlasTESTS>
                COMMAND_EXPAND_LISTS COMMENT "Copying runtime DLLS")

        add_custom_command(TARGET AtlasTESTS POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${SQLite3_LIBRARY} $<TARGET_FILE_DIR:AtlasTESTS> COMMENT "Copying sqlite lib files")
    endif ()

    include(GoogleTest)
    gtest_discover_tests(AtlasTESTS)

endif ()
