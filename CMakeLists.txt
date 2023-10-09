# /CMakeLists.txt

cmake_minimum_required(VERSION 3.21)
project(AtlasGameManager LANGUAGES CXX C)
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MESSAGE_LOG_LEVEL DEBUG CACHE STRING "CMake messaging level")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/data)

#Enable cmake_modules
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
include(common)

message("-- CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_BUILD_TYPE)

PlatformPreSetup()
CompilerPreSetup()
message("-- FGL_FLAGS: ${FGL_FLAGS}")

#Verify after setting QT_PATH
if (DEFINED QT_PATH)
	message("-- QT_PATH defined as ${QT_PATH}.")
	list(APPEND CMAKE_PREFIX_PATH ${QT_PATH})
else ()
	message("-- QT_PATH not defined.")
endif ()

add_subdirectory(dependencies/tracy)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cmake)


if (NOT HAS_STD_FORMAT)
	add_subdirectory(dependencies/fmt)
	option(SPDLOG_FMT_EXTERNAL "" ON)
else ()
	option(SPDLOG_FMT_EXTERNAL "" OFF)
endif ()

add_subdirectory(dependencies/spdlog)

#lz4
include(dependencies/lz4_build.cmake)

#blurhash
add_subdirectory(dependencies/blurhash)

find_package(SQLite3 3.31.1 REQUIRED)
message("-- include = ${SQLite3_INCLUDE_DIR}")
message("-- lib = ${SQLite3_LIBRARY}")

find_package(Qt6 COMPONENTS Widgets Core Concurrent Network Test Charts REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


file(GLOB_RECURSE SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/atlas/core/*.cpp")

file(GLOB_RECURSE UI_SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/atlas/ui/*.cpp")

#Build exe
message("-- FGL_FLAGS: ${FGL_FLAGS}")

if (WIN32)
	add_executable(Atlas WIN32 ${UI_SOURCES} ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/atlas/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/atlas/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
else ()
	add_executable(Atlas ${UI_SOURCES} ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/atlas/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/atlas/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
endif ()

target_include_directories(Atlas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/atlas)

if (NOT HAS_STD_FORMAT)
	target_link_libraries(Atlas PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent Qt6::Network Qt6::Charts SQLite::SQLite3 fmt::fmt spdlog::spdlog TracyClient lz4 BlurhashCXX)
else ()
	target_link_libraries(Atlas PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent Qt6::Network Qt6::Charts SQLite::SQLite3 spdlog::spdlog TracyClient lz4 BlurhashCXX)
endif ()

set_target_properties(Atlas PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})
target_compile_Features(Atlas PRIVATE cxx_constexpr)

#Set all HAVE_X flags
if (HAVE_STD_FORMAT AND NOT DEFINED ATLAS_IGNORE_STD_FORMAT)
	target_compile_definitions(Atlas PRIVATE HAVE_STD_FORMAT=1)
endif ()

if (HAVE_STD_BACKTRACE)
	target_compile_definitions(Atlas PRIVATE HAVE_STD_BACKTRACE=1)
endif ()

if (${UPPER_BUILD_TYPE} STREQUAL "RELEASE")
	target_compile_definitions(Atlas PRIVATE ATLAS_SANITIZE_SOURCE_LOC="${CMAKE_SOURCE_DIR}")
endif ()

SetVersionInfo()
PlatformPostSetup()
CompilerPostSetup()

add_custom_command(TARGET Atlas POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/atlas/ui/qss ${CMAKE_BINARY_DIR}/bin/themes COMMENT " Copying data from ${CMAKE_SOURCE_DIR}/atlas/ui/qss to ${CMAKE_BINARY_DIR}/bin/themes")

#file(GLOB_RECURSE TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp ")

#add_executable(AtlasTests ${TESTS} ${SOURCES} ${UI_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/atlas/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
#target_include_directories(AtlasTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/atlas)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/catch2)
#target_link_libraries(AtlasTests PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent Qt6::Network Qt6::Charts Qt6::Test SQLite::SQLite3 fmt::fmt spdlog::spdlog Catch2::Catch2WithMain TracyClient lz4)
#set_target_properties(AtlasTests PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})
#if (WIN32)
#    target_compile_definitions(AtlasTests PRIVATE UNICODE=1)
#endif ()
