

file(GLOB_RECURSE SOURCES "*.cpp")

qt_add_plugin(AtlasDesigner)
target_sources(AtlasDesigner PRIVATE ${SOURCES})

target_link_libraries(AtlasDesigner PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::UiPlugin)

set_target_properties(AtlasDesigner PROPERTIES AUTOMOC ON)
set_target_properties(AtlasDesigner PROPERTIES AUTOUIC ON)
set_target_properties(AtlasDesigner PROPERTIES AUTORCC ON)

target_include_directories(AtlasDesigner PRIVATE ${CMAKE_SOURCE_DIR}/atlas)

#Set output directory to be in the library directory
set_target_properties(AtlasDesigner PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

#make lib folder
add_custom_command(TARGET AtlasDesigner POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory
		$<TARGET_FILE_DIR:Atlas>/lib
)

#Copy file to binary directory
add_custom_command(TARGET AtlasDesigner POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_BINARY_DIR}/lib/libAtlasDesigner.so
		$<TARGET_FILE_DIR:Atlas>/lib/libAtlasDesigner.so
)
