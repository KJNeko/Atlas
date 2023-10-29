

if (WIN32)
	
	file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/atlas-updater/*.cpp)

	#set(QT_VERSION "6.4.3" CACHE STRING "Version of Qt being used.")
	#set(QT_PATH "C:/Qt/${QT_VERSION}/mingw_64" CACHE PATH "Prefix in which to find Qt.")


	#find_package(Qt6 COMPONENTS Widgets Core Concurrent Network Test Charts REQUIRED)
	
	#include_directories(${CMAKE_SOURCE_DIR}/dependencies/elzip) # Path to the 11zip
	
	#add_executable(AtlasUpdater WIN32 ${SRC_FILES})
	add_executable(AtlasUpdater ${SRC_FILES})
	target_include_directories(AtlasUpdater PUBLIC ${CMAKE_SOURCE_DIR}/atlas-updater)
	target_link_libraries(AtlasUpdater PRIVATE elzip)
endif ()