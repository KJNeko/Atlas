if (WIN32)
	option(BUILD_SHARED_LIBS "Build shared library" ON) 
	option(MZ_ZLIB "Enables ZLIB compression" OFF)
	add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/elzip) # Path to the 11zip
endif ()