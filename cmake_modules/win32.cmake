function(PlatformPreSetup)
if (WIN32)
    find_program(TOOL_WINDEPLOYQT windeployqt REQUIRED NO_CACHE)

    set(QT_PATH "C:/Qt/6.4.3/mingw_64")

    string(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ";.dll")

    if (DEFINED ENV{QT_PATH})
        set(QT_PATH $ENV{QT_PATH} PARENT_SCOPE)
        message("Setting QT path from ENV")
    endif ()
endif ()
endfunction()

function(PlatformPostSetup)
    if (WIN32)
        target_compile_definitions(Atlas PRIVATE UNICODE=1)

        add_custom_command(TARGET Atlas POST_BUILD
                COMMAND ${TOOL_WINDEPLOYQT} --compiler-runtime --no-translations --no-system-d3d-compiler --no-opengl-sw
                $<TARGET_FILE:Atlas>
                COMMENT "Running windeployqt")

        add_custom_command(TARGET Atlas POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:Atlas> $<TARGET_FILE_DIR:Atlas>
                COMMAND_EXPAND_LISTS)

        add_custom_command(TARGET Atlas POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${SQLite3_LIBRARY} $<TARGET_FILE_DIR:Atlas>)

        add_custom_command(TARGET Atlas POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/3rdparty/qwebp.dll
                ${CMAKE_BINARY_DIR}/bin/imageformats
                COMMENT "Copying pre-compiled qwebp.dll for Qt 6.4.3")

    endif ()
endfunction()
