# /cmake_modules/versioninfo.cmake

function(SetVersionInfo)

    find_package(Git)
    if (NOT Git_FOUND AND NOT DEFINED BYPASS_GIT_REQUIREMENT)
        message(FATAL_ERROR
                "HEY YOU! YEAH YOU! READ ME WITH YOUR EYES. Git was not found!
                DO **NOT** **EXPECT** **SUPPORT** if your sending us log
                information without filling these in manually or letting cmake find git.
                Go read the docs to figure out how to do this. Or fix your git install for cmake")
    endif ()

    if (DEFINED ATLAS_GIT_BRANCH)
        message("-- Set git branch string to ${ATLAS_GIT_BRANCH}")
    else ()
        #Get the git branch us currently
        execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE ATLAS_GIT_BRANCH OUTPUT_STRIP_TRAILING_WHITESPACE)
        message("-- Set git branch string to: ${ATLAS_GIT_BRANCH}")
    endif ()

    if (DEFINED ATLAS_GIT_REVISION)
        message("-- Set git revision to: ${ATLAS_GIT_REVISION}")
    else ()
        execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --format=%H WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE ATLAS_GIT_REVISION OUTPUT_STRIP_TRAILING_WHITESPACE)
        message("-- Set git revision to ${ATLAS_GIT_REVISION}")
    endif ()

    if (DEFINED ATLAS_GIT_TAG)
        message("-- Git tag set to: ${ATLAS_GIT_TAG}")
    else ()
        execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE ATLAS_GIT_TAG OUTPUT_STRIP_TRAILING_WHITESPACE)
        message("-- Git tag set to: ${ATLAS_GIT_TAG}")
    endif ()

    if (DEFINED ATLAS_GIT_REVISION_BRIEF)
        message("-- Set git revision to: ${ATLAS_GIT_REVISION_BRIEF}")
    else ()
        execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --format=%h WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE ATLAS_GIT_REVISION_BRIEF OUTPUT_STRIP_TRAILING_WHITESPACE)
        message("-- Set git revision to ${ATLAS_GIT_REVISION_BRIEF}")
    endif ()

    target_compile_definitions(Atlas PRIVATE ATLAS_GIT_BRANCH="${ATLAS_GIT_BRANCH}")
    target_compile_definitions(Atlas PRIVATE ATLAS_GIT_TAG="${ATLAS_GIT_TAG}")
    target_compile_definitions(Atlas PRIVATE ATLAS_GIT_REVISION="${ATLAS_GIT_REVISION}")
    target_compile_definitions(Atlas PRIVATE ATLAS_GIT_REVISION_BRIEF="${ATLAS_GIT_REVISION_BRIEF}")
    target_compile_definitions(Atlas PRIVATE ATLAS_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")
    target_compile_definitions(Atlas PRIVATE ATLAS_COMPILER_VER="${CMAKE_CXX_COMPILER_VERSION}")
    target_compile_definitions(Atlas PRIVATE ATLAS_PLATFORM_ID="${CMAKE_CXX_PLATFORM_ID}")
endfunction()