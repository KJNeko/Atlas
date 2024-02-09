


# Exclude anything that begins with 'Designer'
file(GLOB_RECURSE ATLAS_UI_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

set(ATLAS_DESIGNER_FILES "")

message(STATUS "Atlas Designer files: ${ATLAS_DESIGNER_FILES}")

set(ATLAS_UI_LIBS Qt6::Core Qt6::Gui Qt6::Concurrent Qt6::Network Qt6::Widgets Qt6::Charts Qt6::UiPlugin TracyClient)

# Filter out designer files
list(FILTER ATLAS_UI_SRC EXCLUDE REGEX ".*Designer.*")

add_library(AtlasUI OBJECT ${ATLAS_UI_SRC})
target_link_libraries(AtlasUI PUBLIC ${ATLAS_UI_LIBS} AtlasDesigner)
target_include_directories(AtlasUI PUBLIC ${CMAKE_SOURCE_DIR}/atlas)

set_target_properties(AtlasUI PROPERTIES AUTOMOC ON)
set_target_properties(AtlasUI PROPERTIES AUTOUIC ON)
set_target_properties(AtlasUI PROPERTIES AUTORCC ON)

target_compile_definitions(AtlasUI PRIVATE ATLAS_CMAKE_SOURCE_LOCATION="${CMAKE_SOURCE_DIR}")
DefineVersionInfo(AtlasUI)


