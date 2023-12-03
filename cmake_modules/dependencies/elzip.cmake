if (WIN32)
	//option(MZ_ZLIB "Enables ZLIB compression" OFF)
	option(MZ_FORCE_FETCH_LIBS "Enables fetching third-party libraries always" ON)
	add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/elzip) # Path to the 11zip
endif ()