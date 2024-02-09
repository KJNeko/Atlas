
file(GLOB_RECURSE ATLAS_CORE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_library(AtlasCore OBJECT ${ATLAS_CORE_SRC})

set(ATLAS_CORE_LIBS Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent Qt6::Network SQLite::SQLite3 spdlog::spdlog TracyClient lz4 BlurhashCXX)

if (ATLAS_LINK_FMT EQUAL 1)
	list(APPEND ATLAS_CORE_LIBS fmt::fmt)
endif ()

#Setup AUTO_MOC only for core
set_target_properties(AtlasCore PROPERTIES AUTOMOC ON)
set_target_properties(AtlasCore PROPERTIES AUTOUIC ON)
set_target_properties(AtlasCore PROPERTIES AUTORCC ON)

target_link_libraries(AtlasCore PUBLIC ${ATLAS_CORE_LIBS})
target_include_directories(AtlasCore PUBLIC ${CMAKE_SOURCE_DIR}/atlas)

target_compile_definitions(AtlasCore PRIVATE ATLAS_CMAKE_SOURCE_LOCATION="${CMAKE_SOURCE_DIR}")
DefineVersionInfo(AtlasCore)
