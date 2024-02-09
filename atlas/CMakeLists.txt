
#WIN32
if (WIN32)
	add_executable(Atlas ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
else ()
	add_executable(Atlas ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
endif ()

set_target_properties(Atlas PROPERTIES AUTORCC ON)

target_precompile_headers(Atlas PUBLIC
		<memory>
		<iostream>
		<filesystem>
		<string>
		<source_location>
		<exception>

		<QObject>
		<QSettings>
		<QWidget>
		<QString>
		<QVariant>
		<QAbstractItemDelegate>
		<QImage>
		<QPixmap>
		<QtCore/QModelIndex>
		<QDialog>

		<sqlite3.h>
)

target_include_directories(Atlas PUBLIC ${CMAKE_SOURCE_DIR}/atlas)

SetVersionInfo()

set(CMAKE_AUTORCC ON)

add_subdirectory(core)
add_subdirectory(ui)
add_subdirectory(designer)

DefineVersionInfo(Atlas)

target_link_libraries(Atlas PUBLIC AtlasUI AtlasCore)

set_target_properties(Atlas PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})
target_compile_Features(Atlas PRIVATE cxx_constexpr)

#Set all HAVE_X flags
if (HAVE_STD_FORMAT AND NOT DEFINED ATLAS_IGNORE_STD_FORMAT)
	target_compile_definitions(Atlas PRIVATE HAVE_STD_FORMAT=1)
endif ()

if (HAVE_STD_BACKTRACE)
	target_compile_definitions(Atlas PRIVATE HAVE_STD_BACKTRACE=1)
endif ()

if (DEFINED ATLAS_EMBED_FULL_SOURCE_LOCATION)
	target_compile_definitions(Atlas PRIVATE ATLAS_EMBED_FULL_SOURCE_LOCATION=1)
endif ()

#Make Atlas depend on AtlasDesigner
add_dependencies(Atlas AtlasDesigner)

add_custom_command(TARGET Atlas POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/atlas/ui/qss ${CMAKE_BINARY_DIR}/bin/themes COMMENT " Copying data from ${CMAKE_SOURCE_DIR}/atlas/ui/qss to ${CMAKE_BINARY_DIR}/bin/themes")

PlatformPostSetup()
CompilerPostSetup()