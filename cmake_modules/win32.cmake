# /cmake_modules/win32.cmake

if (WIN32)

    set(
        NEEDED_QT_FOLDERS
            "${CMAKE_BINARY_DIR}/bin/data"
            "${CMAKE_BINARY_DIR}/bin/iconengines"
            "${CMAKE_BINARY_DIR}/bin/imageformats"
            "${CMAKE_BINARY_DIR}/bin/networkinformation"
            "${CMAKE_BINARY_DIR}/bin/platforms"
            "${CMAKE_BINARY_DIR}/bin/styles"
            "${CMAKE_BINARY_DIR}/bin/tls"
    )

    function(PlatformPreSetup)
        # set(QT_PATH "C:/Qt/6.4.3/mingw_64" )
        # set(QT_PATH "C:/msys64/clang64")
        find_program(
            TOOL_WINDEPLOYQT
                windeployqt windeployqt6
                REQUIRED NO_CACHE
                HINTS "${QT_PATH}/bin"
        )
        message("windeployqt found: ${TOOL_WINDEPLOYQT}")

        string(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ";.dll")

        if (DEFINED ENV{QT_PATH})
            set(QT_PATH $ENV{QT_PATH} PARENT_SCOPE)
            message("Setting QT path from ENV")
        endif ()
        set(TOOL_WINDEPLOYQT ${TOOL_WINDEPLOYQT} PARENT_SCOPE)
    endfunction()   # PlatformPreSetup

    function(PlatformPostSetup)
        target_compile_definitions(Atlas PRIVATE UNICODE=1)
        add_custom_target(build-time-make-directory ALL
            COMMAND ${CMAKE_COMMAND} -E make_directory ${NEEDED_QT_FOLDERS}
                COMMENT "Creating necessary folders"
        )
        if(($CMAKE_CXX_PLATFORM_ID} STREQUAL "GNU") OR (${CMAKE_CXX_PLATFORM_ID} STREQUAL "MSVC"))
        add_custom_command(TARGET Atlas POST_BUILD
                COMMAND ${TOOL_WINDEPLOYQT} --compiler-runtime --no-translations --no-system-d3d-compiler --no-opengl-sw
                $<TARGET_FILE:Atlas>
                COMMENT "Running windeployqt")
        endif()
        add_custom_command(TARGET Atlas POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:Atlas> $<TARGET_FILE_DIR:Atlas>
                COMMAND_EXPAND_LISTS)

        add_custom_command(TARGET Atlas POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${SQLite3_LIBRARY} $<TARGET_FILE_DIR:Atlas>)

        add_custom_command(TARGET Atlas POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/3rdparty/qwebp.dll
                ${CMAKE_BINARY_DIR}/bin/imageformats/qwebp.dll
                COMMENT "Copying pre-compiled qwebp.dll for Qt 6.4.3")

    endfunction()   # PlatformPostSetup

endif ()    # if (WIN32)
