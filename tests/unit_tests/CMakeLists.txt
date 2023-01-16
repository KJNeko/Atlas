project(Hydrus95_UNIT_TESTS CXX)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


file(GLOB_RECURSE TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(Hydrus95_UNIT_TESTS ${TEST_SOURCES} ${SOURCES})
target_include_directories(Hydrus95_UNIT_TESTS PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(Hydrus95_UNIT_TESTS PUBLIC ${REQUIRED_LIBS} GTest::gtest_main)