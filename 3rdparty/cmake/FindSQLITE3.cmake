include(FindPackageHandleStandardArgs)

find_file(SQLite3_LIBRARY NAMES lib/sqlite3.dll)
mark_as_advanced(SQLite3_LIBRARY)
find_path(SQLite3_INCLUDE_DIR NAMES include/sqlite3.h)    
mark_as_advanced(SQLite3_INCLUDE_DIR)


find_package_handle_standard_args(SQLite3 REQUIRED_VARS SQLite3_LIBRARY SQLite3_INCLUDE_DIR)


if (SQLite3_FOUND AND NOT TARGET SQLite3)
        add_library(SQLite3::SQLite3 UNKNOWN IMPORTED)
        set_target_properties(SQLite3::SQLite3 PROPERTIES
            IMPORTED_LOCATION             "${SQLite3_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${SQLite3_INCLUDE_DIR}")
endif ()

