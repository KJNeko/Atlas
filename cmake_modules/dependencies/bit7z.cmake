option(BIT7Z_AUTO_FORMAT "Enable extraction for all formats" ON)

if (UNIX)
	# This is to fix a problem with linux having ancient 7z versions

	#Check what version of 7z the system has
	execute_process(COMMAND 7z OUTPUT_VARIABLE _7Z_VERSION)
	#Check if the command executed properly
	if (NOT _7Z_VERSION)
		message(FATAL_ERROR "7z not found. Please install p7zip")
	endif ()

	string(REGEX MATCH "([0-9]+)\\.([0-9]+)" _7Z_VERSION "${_7Z_VERSION}")
	string(REGEX MATCH "([0-9]+)" _7Z_VERSION_MAJOR "${_7Z_VERSION}")
	string(REGEX MATCH "([0-9]+)$" _7Z_VERSION_MINOR "${_7Z_VERSION}")

	message("-- System 7z version: ${_7Z_VERSION_MAJOR}.${_7Z_VERSION_MINOR}")

	# Is it before 23.01?
	if (_7Z_VERSION_MAJOR LESS 23 OR (_7Z_VERSION_MAJOR EQUAL 23 AND _7Z_VERSION_MINOR LESS 1))
		# Yes. We need to do the fix
		option(BIT7Z_USE_LEGACY_IUNKNOWN "Use legacy IUNKNOWN" ON)
	else ()
		# No. We don't need to do the fix
		option(BIT7Z_USE_LEGACY_IUNKNOWN "Use legacy IUNKNOWN" OFF)
	endif ()

	option(BIT7Z_AUTO_FORMAT ON)
endif ()

add_subdirectory(${CMAKE_SOURCE_DIR}/dependencies/bit7z) # Path to the 11zip