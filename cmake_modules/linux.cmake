# /cmake_modules/linux.cmake

if(UNIX AND (NOT APPLE))
    function(PlatformPreSetup)
    endfunction()

    function(PlatformPostSetup)
    endfunction()
endif ()    # if(UNIX AND (NOT APPLE))