cmake_minimum_required(VERSION 3.21)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

project(zlib C)

set(VERSION "1.2.13")

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)
enable_testing()

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stddef.h HAVE_STDDEF_H)

#
# Check to see if we have large file support
#
set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
# We add these other definitions here because CheckTypeSize.cmake
# in CMake 2.4.x does not automatically do so and we want
# compatibility with CMake 2.4.x.
if (HAVE_SYS_TYPES_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
endif ()
if (HAVE_STDINT_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
endif ()
if (HAVE_STDDEF_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
endif ()
check_type_size(off64_t OFF64_T)
if (HAVE_OFF64_T)
    add_definitions(-D_LARGEFILE64_SOURCE=1)
endif ()
set(CMAKE_REQUIRED_DEFINITIONS) # clear variable

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if (NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif ()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if (MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
endif ()

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    # If we're doing an out of source build and the user has a zconf.h
    # in their source tree...
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zconf.h)
        message(STATUS "Renaming")
        message(STATUS "    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zconf.h")
        message(STATUS "to 'zconf.h.included' because this file is included with zlib")
        message(STATUS "but CMake generates it automatically in the build directory.")
        file(RENAME ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zconf.h ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zconf.h.included)
    endif ()
endif ()

set(ZLIB_PC ${CMAKE_CURRENT_BINARY_DIR}/zlib.pc)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zlib.pc.cmakein
        ${ZLIB_PC} @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zconf.h.cmakein
        ${CMAKE_CURRENT_BINARY_DIR}/zconf.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR})


#============================================================================
# zlib
#============================================================================

set(ZLIB_PUBLIC_HDRS
        ${CMAKE_CURRENT_BINARY_DIR}/zconf.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zlib.h
        )
set(ZLIB_PRIVATE_HDRS
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/crc32.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/deflate.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/gzguts.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/inffast.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/inffixed.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/inflate.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/inftrees.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/trees.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zutil.h
        )
set(ZLIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/adler32.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/compress.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/crc32.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/deflate.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/gzclose.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/gzlib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/gzread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/gzwrite.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/inflate.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/infback.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/inftrees.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/inffast.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/trees.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/uncompr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zutil.c
        )

if (NOT MINGW)
    set(ZLIB_DLL_SRCS
            ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/win32/zlib1.rc # If present will override custom build rule below.
            )
endif ()

# parse the full version number from zlib.h and include in ZLIB_FULL_VERSION
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zlib.h _zlib_h_contents)
string(REGEX REPLACE ".*#define[ \t]+ZLIB_VERSION[ \t]+\"([-0-9A-Za-z.]+)\".*"
        "\\1" ZLIB_FULL_VERSION ${_zlib_h_contents})

if (MINGW)
    # This gets us DLL resource information when compiling on MinGW.
    if (NOT CMAKE_RC_COMPILER)
        set(CMAKE_RC_COMPILER windres.exe)
    endif ()

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/zlib1rc.obj
            COMMAND ${CMAKE_RC_COMPILER}
            -D GCC_WINDRES
            -I ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib
            -I ${CMAKE_CURRENT_BINARY_DIR}
            -o ${CMAKE_CURRENT_BINARY_DIR}/zlib1rc.obj
            -i ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/win32/zlib1.rc)
    set(ZLIB_DLL_SRCS ${CMAKE_CURRENT_BINARY_DIR}/zlib1rc.obj)
endif (MINGW)

add_library(zlib SHARED ${ZLIB_SRCS} ${ZLIB_DLL_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
add_library(zlibstatic STATIC ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
set_target_properties(zlib PROPERTIES DEFINE_SYMBOL ZLIB_DLL)
set_target_properties(zlib PROPERTIES SOVERSION 1)

if (NOT CYGWIN)
    # This property causes shared libraries on Linux to have the full version
    # encoded into their final filename.  We disable this on Cygwin because
    # it causes cygz-${ZLIB_FULL_VERSION}.dll to be created when cygz.dll
    # seems to be the default.
    #
    # This has no effect with MSVC, on that platform the version info for
    # the DLL comes from the resource file win32/zlib1.rc
    set_target_properties(zlib PROPERTIES VERSION ${ZLIB_FULL_VERSION})
endif ()

if (UNIX)
    # On unix-like platforms the library is almost always called libz
    set_target_properties(zlib zlibstatic PROPERTIES OUTPUT_NAME z)
    if (NOT APPLE)
        set_target_properties(zlib PROPERTIES LINK_FLAGS "-Wl,--version-script,\"${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib/zlib.map\"")
    endif ()
elseif (BUILD_SHARED_LIBS AND WIN32)
    # Creates zlib1.dll when building shared library version
    set_target_properties(zlib PROPERTIES SUFFIX "1.dll")
endif ()