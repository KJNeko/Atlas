

if (WIN32)
	
	file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/atlas-updater/*.cpp)

	#set(QT_VERSION "6.4.3" CACHE STRING "Version of Qt being used.")
	#set(QT_PATH "C:/Qt/${QT_VERSION}/mingw_64" CACHE PATH "Prefix in which to find Qt.")


	#find_package(Qt6 COMPONENTS Widgets Core Concurrent Network Test Charts REQUIRED)
	
	#include_directories(${CMAKE_SOURCE_DIR}/dependencies/elzip) # Path to the 11zip
	
	#add_executable(AtlasUpdater WIN32 ${SRC_FILES})
	
	add_executable(AtlasUpdater WIN32 ${SRC_FILES})

    set_target_properties(AtlasUpdater PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" COMPILE_FLAGS "${FGL_FLAGS} -static -static-libstdc++ -static-libgcc")
	#add_custom_command(TARGET AtlasUpdater POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_RUNTIME_DLLS:AtlasUpdater> COMMAND_EXPAND_LISTS)
	target_include_directories(AtlasUpdater PUBLIC ${CMAKE_SOURCE_DIR}/atlas-updater)
	target_link_libraries(AtlasUpdater PRIVATE bit7z)
endif ()