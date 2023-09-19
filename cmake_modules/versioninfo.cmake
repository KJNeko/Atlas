# /cmake_modules/versioninfo.cmake

function(SetVersionInfo)
    if (DEFINED ATLAS_GIT_BRANCH)
        message("-- Set git branch string to ${ATLAS_GIT_BRANCH}")
    else ()
        #Get the git branch us currently
        execute_process(COMMAND git rev-parse --abbrev-ref HEAD WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE ATLAS_GIT_BRANCH OUTPUT_STRIP_TRAILING_WHITESPACE)
        message("-- Set git branch string to: ${ATLAS_GIT_BRANCH}")
    endif ()

    if (DEFINED ATLAS_GIT_REVISION)
        message("-- Set git revision to: ${ATLAS_GIT_REVISION}")
    else ()
        execute_process(COMMAND git log -1 --format=%H WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE ATLAS_GIT_REVISION OUTPUT_STRIP_TRAILING_WHITESPACE)
        message("-- Set git revision to ${ATLAS_GIT_REVISION}")
    endif ()

    if (DEFINED ATLAS_GIT_TAG)
        message("-- Git tag set to: ${ATLAS_GIT_TAG}")
    else ()
        execute_process(COMMAND git describe --abbrev=0 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE ATLAS_GIT_TAG OUTPUT_STRIP_TRAILING_WHITESPACE)
        message("-- Git tag set to: ${ATLAS_GIT_TAG}")
    endif ()

    if (DEFINED ATLAS_GIT_REVISION_BRIEF)
        message("-- Set git revision to: ${ATLAS_GIT_REVISION_BRIEF}")
    else ()
        execute_process(COMMAND git log -1 --format=%h WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE ATLAS_GIT_REVISION_BRIEF OUTPUT_STRIP_TRAILING_WHITESPACE)
        message("-- Set git revision to ${ATLAS_GIT_REVISION_BRIEF}")
    endif ()

    target_compile_definitions(Atlas PRIVATE ATLAS_GIT_BRANCH=${ATLAS_GIT_BRANCH})
    target_compile_definitions(Atlas PRIVATE ATLAS_GIT_TAG=${ATLAS_GIT_TAG})
    target_compile_definitions(Atlas PRIVATE ATLAS_GIT_REVISION=${ATLAS_GIT_REVISION})
    target_compile_definitions(Atlas PRIVATE ATLAS_GIT_REVISION_BRIEF=${ATLAS_GIT_REVISION_BRIEF})
endfunction()