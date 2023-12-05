
file(GLOB_RECURSE SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp")

file(GLOB_RECURSE UI_SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/ui/*.cpp")

#WIN32
if (WIN32)
	add_executable(Atlas ${UI_SOURCES} ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
else ()
	add_executable(Atlas ${UI_SOURCES} ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
endif ()

target_include_directories(Atlas PUBLIC ${CMAKE_SOURCE_DIR}/atlas)

if (ATLAS_LINK_FMT EQUAL 1)
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

add_custom_command(TARGET Atlas POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/atlas/ui/qss ${CMAKE_BINARY_DIR}/bin/themes COMMENT " Copying data from ${CMAKE_SOURCE_DIR}/atlas/ui/qss to ${CMAKE_BINARY_DIR}/bin/themes")

PlatformPostSetup()
CompilerPostSetup()