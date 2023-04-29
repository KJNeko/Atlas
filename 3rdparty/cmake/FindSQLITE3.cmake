include(FindPackageHandleStandardArgs)

find_library(SQLITE3_LIBRARY NAMES sqlite3)
find_path(SQLITE3_INCLUDE_DIR NAMES sqlite3.h)

find_package_handle_standard_args(SQLITE3 REQUIRED_VARS SQLITE3_LIBRARY SQLITE3_INCLUDE_DIR)

if (SQLITE3_FOUND)
    mark_as_advanced(SQLITE3_INCLUDE_DIR)
    mark_as_advanced(SQLITE3_LIBRARY)
endif ()

if (SQLITE3_FOUND AND NOT TARGET SQLITE3)
    add_library(SQLITE3::SQLITE3 IMPORTED SHARED)
    set_property(TARGET SQLITE3::SQLITE3 PROPERTY IMPORTED_LOCATION ${SQLITE3_LIBRARY})
    target_include_directories(SQLITE3::SQLITE3 INTERFACE ${SQLITE3_INCLUDE_DIR})
endif ()

