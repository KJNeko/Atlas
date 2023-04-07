cmake_minimum_required(VERSION 3.21)
project(AtlasGameManager LANGUAGES CXX)

enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILT_TYPE Release ... FORCE)
endif ()

#Sql
if (WIN32)
    if (DEFINED SQL_PATH)
        set(SQLite3_INCLUDE_DIR "${SQL_PATH}\\include")
        set(SQLite3_LIBRARY "${SQL_PATH}\\lib\\sqlite3.dll")
        set(SQLite3_FOUND true)
        message("FOUND SQL_PATH")
    elseif (DEFINED ENV{SQLITE3_PATH})
        set(SQLite3_INCLUDE_DIR $ENV{SQLITE3_PATH}\\include)
        set(SQLite3_LIBRARY $ENV{SQLITE3_PATH}\\lib\\sqlite3.dll)
        set(SQLite3_FOUND true)
        message("FOUND ENV SQLITE3_PATH")
    else ()
        message("Must define SQL_PATH with -DSQL_PATH=?? or add to path as SQLITE3_PATH")
    endif ()

    message("SQlite3_INCLUDE_DIR = ${SQLite3_INCLUDE_DIR}")
    message("SQlite3_LIBRARY = ${SQLite3_LIBRARY}")
endif ()

find_package(SQLite3 REQUIRED)

#Spdlog
if (WIN32)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/spdlog)
else ()
    find_package(spdlog REQUIRED)
endif ()

if (WIN32)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/fmt)
else ()
    find_package(fmt REQUIRED)
endif ()

#Qt
if (DEFINED QT_PATH)
    list(APPEND CMAKE_PREFIX_PATH ${QT_PATH})
endif ()
find_package(Qt6 COMPONENTS Widgets Core Concurrent Network Test REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if (WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL " Debug")
        find_program(TOOL_WINDEPLOYQT NAMES windeployqt)
    else ()
        find_program(TOOL_WINDEPLOYQT NAMES windeployqt)
    endif ()
endif ()

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} ")
string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_BUILD_TYPE)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    #Tracy (Profiler)
    option(TRACY_ENABLE "" ON)
    option(TRACY_ON_DEMAND "" OFF)
elseif (${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
    #Tracy (Profiler)
    option(TRACY_ENABLE "" ON)
    option(TRACY_ON_DEMAND "" OFF)
else ()
    option(TRACY_ENABLE "" OFF)
endif ()

add_subdirectory(dependencies/tracy)

if (${CMAKE_COMPILER_IS_GNUCXX})
    #sqlite_modern is stupid so we have to disable -Wundef

    set(FGL_WARNINGS " -Wall -Wextra -Wnull-dereference -Wpedantic -Weffc++ -pedantic-errors -Wnoexcept -Wuninitialized -Wunused -Wunused-parameter -Winit-self -Wconversion -Wuseless-cast -Wextra-semi -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wformat-signedness -Wno-format-zero-length -Wmissing-include-dirs -Wshift-overflow=2 -Walloc-zero -Walloca -Wsign-promo -Wconversion -Wduplicated-branches -Wduplicated-cond -Wshadow -Wshadow=local -Wvirtual-inheritance -Wno-virtual-move-assign -Wunsafe-loop-optimizations -Wnormalized -Wpacked -Wredundant-decls -Wmismatched-tags -Wredundant-tags -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wzero-as-null-pointer-constant -Wconditionally-supported -Wwrite-strings -Wunused-const-variable=2 -Wdouble-promotion -Wpointer-arith -Wcast-align=strict -Wcast-qual -Wconversion -Wsign-conversion -Wimplicit-fallthrough=1 -Wmisleading-indentation -Wdangling-else -Wdate-time -Wformat=2 -Wformat-overflow=2 -Wformat-signedness -Wformat-truncation=2 -Wswitch-default -Wstringop-overflow=4 -Warray-bounds=2 -Wattribute-alias=2 -Wcatch-value=2 -Wplacement-new=2 -Wtrampolines -Winvalid-imported-macros -Winvalid-imported-macros")

    set(FGL_CONFIG "-std=c++23 -fmax-errors=3 -fconcepts-diagnostics-depth=4")

    set(FGL_DEBUG "-Og -fverbose-asm -g -fstrict-aliasing -fno-omit-frame-pointer -fstack-check -ftrapv -fverbose-asm -femit-class-debug-always")# -rdynamic")

    #Generates system specific stuff (IE requires AVX)
    set(FGL_SYSTEM_SPECIFIC "-march=native -fgcse -fgcse-las -fgcse-sm -fdeclone-ctor-dtor -fdevirtualize-speculatively -ftree-loop-im -fivopts -ftree-loop-ivcanon -fira-hoist-pressure -fsched-pressure -fsched-spec-load -fipa-pta -flto=auto -s -ffat-lto-objects -fno-enforce-eh-specs -fstrict-enums -funroll-loops")
    #Generates safe optimization flags
    set(FGL_SYSTEM_SAFE "-O3 -fdevirtualize-at-ltrans -flto=auto -s")

    set(FGL_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${FGL_WARNINGS} ${FGL_CONFIG} ${FGL_DEBUG}")
    set(FGL_FLAGS_SYSTEM "${CMAKE_CXX_FLAGS} ${FLG_CONFIG} -DNDEBUG ${FGL_SYSTEM_SAFE} ${FGL_SYSTEM_SPECIFIC}")
    set(FGL_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${FGL_CONFIG} -DNDEBUG -s ${FGL_SYSTEM_SAFE}")
    set(FGL_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} ${FGL_CONFIG} -DNDEBUG -g ${FGL_SYSTEM_SAFE} ${FGL_SYSTEM_SPECIFIC}")
    set(FGL_FLAGS ${FGL_FLAGS_${UPPER_BUILD_TYPE}})
else ()
    message("Note that you are compiling with an unsupported compiler (Only GCC is offically supported) You can still make bug reports but MENTION YOUR COMPILER AND BUILD SYSTEM")
endif ()

message("Compiling with: ${FGL_FLAGS}")

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/h95/*.cpp")

file(GLOB_RECURSE UI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.cpp")


set(HYDRUS_SOURCES ${SOURCES})
set(HYDRUS_UI ${UI_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/resources.qrc)
set(REQUIRED_LIBS PUBLIC Qt6::Widgets Qt6::Core Qt6::Concurrent Qt6::QWebpPlugin SQLite::SQLite3 spdlog::spdlog fmt Tracy::TracyClient)

#Lib for EXE and Python
add_library(AtlasObj OBJECT ${HYDRUS_SOURCES} ${HYDRUS_UI})
target_link_libraries(AtlasObj ${REQUIRED_LIBS})
target_include_directories(AtlasObj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(AtlasObj PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})

if (DEFINED DISABLE_FLYWEIGHT)
    target_compile_definitions(H95Obj PRIVATE HYDRUS95_DISABLE_FLYWEIGHT)
endif ()

#Build exe
add_executable(Atlas ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

if (${CMAKE_COMPILER_IS_GNUCXX})
    set_target_properties(Atlas PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})
endif ()

target_include_directories(Atlas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/dependencies/sqlite_modern_cpp/hdr)
endif ()

target_link_libraries(Atlas ${REQUIRED_LIBS} AtlasObj)

#Post setup (Windows only)
if (WIN32)
    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${TOOL_WINDEPLOYQT} --compiler-runtime
            $<TARGET_FILE:Atlas>
            COMMENT "Running windeployqt")

    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Atlas> $<TARGET_FILE_DIR:Atlas>
            COMMAND_EXPAND_LISTS)

    add_custom_command(TARGET Atlas POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SQLite3_LIBRARY} $<TARGET_FILE_DIR:Atlas>)
endif ()

add_custom_command(TARGET Atlas POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/qss $<TARGET_FILE_DIR:Atlas>/data/themes COMMENT "Compiling qss files")

#Test builds
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND NOT DEFINED NO_BUILD_TESTS)

    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    file(GLOB_RECURSE TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

    add_executable(AtlasTESTS ${TESTS})

    set_target_properties(AtlasTESTS PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})
    target_include_directories(AtlasTESTS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(AtlasTESTS ${REQUIRED_LIBS} Qt::Test GTest::gtest_main H95Obj)

    #Test post setup (Windows only)
    if (WIN32)
        add_custom_command(TARGET AtlasTESTS POST_BUILD
                COMMAND ${TOOL_WINDEPLOYQT} --compiler-runtime
                $<TARGET_FILE:AtlasTESTS>
                COMMENT "Running windeployqt")

        add_custom_command(TARGET AtlasTESTS POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:AtlasTESTS> $<TARGET_FILE_DIR:AtlasTESTS>
                COMMAND_EXPAND_LISTS COMMENT "Copying runtime DLLS")

        add_custom_command(TARGET AtlasTESTS POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy ${SQLite3_LIBRARY} $<TARGET_FILE_DIR:AtlasTESTS> COMMENT "Copying sqlite lib files")
    endif ()

    include(GoogleTest)
    gtest_discover_tests(AtlasTESTS)

endif ()
